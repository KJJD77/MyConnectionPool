#include "MyConnectionPool.h"
#include <iostream>
#include <fstream>
#include <functional>
// 返回同一个连接池对象
MyConnectionPool *MyConnectionPool::getConnectionPool()
{
    static MyConnectionPool pool;
    return &pool;
}

// 从配置文件中加载配置项
bool MyConnectionPool::loadConfigFile()
{
    FILE *fp = fopen("../mysql.conf", "r");
    if (fp == nullptr)
    {
        std::cerr << "mysql.conf文件打开失败" << std::endl;
        return false;
    }
    char buffer[1024] = {0};
    while (fgets(buffer, sizeof(buffer), fp) != nullptr)
    {
        std::string line(buffer);
        std::string::size_type pos = 0;
        while ((pos = line.find_first_of(" ")) != std::string::npos)
        {
            line.erase(pos, 1);
        }

        if (line.find("ip") != std::string::npos)
        {
            _ip = line.substr(line.find("=") + 1);
            _ip.erase(_ip.find_last_not_of("\n") + 1);
            // std::cerr << _ip << std::endl;
        }
        else if (line.find("port") != std::string::npos)
        {
            _port = atoi(line.substr(line.find("=") + 1).c_str());
        }
        else if (line.find("dbname") != std::string::npos)
        {
            _dbname = line.substr(line.find("=") + 1);
            _dbname.erase(_dbname.find_last_not_of("\n") + 1);
        }
        else if (line.find("username") != std::string::npos)
        {
            _username = line.substr(line.find("=") + 1);
            _username.erase(_username.find_last_not_of("\n") + 1);
        }
        else if (line.find("password") != std::string::npos)
        {
            _password = line.substr(line.find("=") + 1);
            _password.erase(_password.find_last_not_of("\n") + 1);
        }
        else if (line.find("initSize") != std::string::npos)
        {
            _initSize = atoi(line.substr(line.find("=") + 1).c_str());
        }
        else if (line.find("maxSize") != std::string::npos)
        {
            _maxSize = atoi(line.substr(line.find("=") + 1).c_str());
        }
        else if (line.find("maxIdleTime") != std::string::npos)
        {
            _maxIdleTime = atoi(line.substr(line.find("=") + 1).c_str());
        }
    }
    return true;
}

// 构造函数私有化
MyConnectionPool::MyConnectionPool()
{
    if (!loadConfigFile())
        return;
    // 创建初始数量的连接
    for (int i = 0; i < _initSize; ++i)
    {
        Connection *p = new Connection();
        p->connect(_ip, _port, _username, _password, _dbname);
        p->refreshAliveTime();
        _connectionQue.push(p);
        _connectionCnt++;
    }

    // 开启生产者线程
    std::thread produce(std::bind(&MyConnectionPool::produceConnectionTask, this));
    produce.detach();

    // 开启扫描者线程
    std::thread scanner(std::bind(&MyConnectionPool::scannerConnectionTask, this));
    scanner.detach();
}
void MyConnectionPool::produceConnectionTask()
{
    for (;;)
    {
        std::unique_lock<std::mutex> lock(_queueMutex);
        while (!_connectionQue.empty())
        {
            cv.wait(lock);
        }
        if (_connectionCnt < _maxSize)
        {
            Connection *p = new Connection();
            p->connect(_ip, _port, _username, _password, _dbname);
            _connectionQue.push(p);
            p->refreshAliveTime();
            _connectionCnt++;
        }

        // 通知消费者线程，可以消费连接了
        cv.notify_all();
    }
}

// 给外部提供接口，从连接池中获取一个可用的空闲连接
std::shared_ptr<Connection> MyConnectionPool::getConnection()
{
    std::unique_lock<std::mutex> lock(_queueMutex);
    while (_connectionQue.empty())
    {
        // sleep
        if (cv.wait_for(lock, std::chrono::milliseconds(_connectionTimeout)) == std::cv_status::timeout)
        {
            if (_connectionQue.empty())
            {
                LOG("获取空闲连接超时了...获取连接失败!");
                return nullptr;
            }
        }
    }

    /*
shared_ptr智能指针析构时，会把connection资源直接delete掉，相当于
调用connection的析构函数，connection就被close掉了。
这里需要自定义shared_ptr的释放资源的方式，把connection直接归还到queue当中
*/
    std::shared_ptr<Connection> sp(_connectionQue.front(), [&](Connection *cont)
                                   {
                                       std::unique_lock<std::mutex> lock(_queueMutex);
                                       _connectionQue.push(cont);
                                       cont->refreshAliveTime(); });

    _connectionQue.pop();
    cv.notify_all(); // 消费完连接以后，通知生产者线程检查一下，如果队列为空了，赶紧生产连接

    return sp;
}

// 扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
void MyConnectionPool::scannerConnectionTask()
{
    for (;;)
    {
        // 通过sleep模拟定时效果
        std::this_thread::sleep_for(std::chrono::seconds(_maxIdleTime));

        // 扫描整个队列，释放多余的连接
        std::unique_lock<std::mutex> lock(_queueMutex);
        while (_connectionCnt > _initSize)
        {
            Connection *cont = _connectionQue.front();
            if (cont->getAliveTime() >= (_maxIdleTime * 1000))
            {
                _connectionQue.pop();
                _connectionCnt--;
                delete (cont);
            }
            else
            {
                break;
            }
        }
    }
}