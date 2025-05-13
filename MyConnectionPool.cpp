#include "MyConnectionPool.h"
#include <iostream>
#include <fstream>
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
        p->connect(_ip,_port,_username,_password,_dbname);
        _connectionQue.push(p);
        _connectionCnt++;
    }

}