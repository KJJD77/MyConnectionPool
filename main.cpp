#include "Connection.h"
#include "MyConnectionPool.h"
#include "public.h"
#include <iostream>

int main()
{

    // 此段是用来测试连接是否有效
    /*     Connection conn;
        if (!conn.connect("localhost", 3306, "root","*****", "test"))
        {
            std::cerr << "连接数据库失败\n";
            return 1;
        }
        char sql[1024]={0};
        sprintf(sql,"INSERT INTO user (name,age,sex) VALUES ('%s', %d, '%s')", "zhangsan", 20, "男");
        conn.update(sql); */

    // 测试单线程的非连接池
    /*     auto begin = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            Connection conn;
            if (!conn.connect("localhost", 3306, "root", "Jjd040830", "test"))
            {
                std::cerr << "连接数据库失败\n";
                return 1;
            }
            char sql[1024] = {0};
            sprintf(sql, "INSERT INTO user (name,age,sex) VALUES ('%s', %d, '%s')", "zhangsan", 20, "男");
            conn.update(sql);
        }
        auto end = std::chrono::high_resolution_clock::now();                                       // 结束计时
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); // 计算毫秒数
        std::cout << duration << '\n'; */

    // 测试单线程连接池
    /*     auto begin = std::chrono::high_resolution_clock::now();
        MyConnectionPool *cp = MyConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; ++i)
        {
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                    "zhang san", 20, "male");
            std::shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);
        }
        auto end = std::chrono::high_resolution_clock::now();                                       // 结束计时
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); // 计算毫秒数
        std::cout << duration << '\n'; */

    // 多线程连接测试
    /*     auto begin = std::chrono::high_resolution_clock::now(); // 开始计时
        std::thread t1([]()
                       {
                //ConnectionPool *cp = ConnectionPool::getConnectionPool();
               for (int i = 0; i < 250; i++)
                {
                    Connection conn;
                    if (!conn.connect("localhost", 3306, "root", "Jjd040830", "test"))
                    {
                        std::cerr << "连接数据库失败\n";
                        return 1;
                    }
                    char sql[1024] = {0};
                    sprintf(sql, "INSERT INTO user (name,age,sex) VALUES ('%s', %d, '%s')", "zhangsan", 20, "男");
                    conn.update(sql);
                } });

        std::thread t2([]()
                       {
                //ConnectionPool *cp = ConnectionPool::getConnectionPool();
               for (int i = 0; i < 250; i++)
                {
                    Connection conn;
                    if (!conn.connect("localhost", 3306, "root", "Jjd040830", "test"))
                    {
                        std::cerr << "连接数据库失败\n";
                        return 1;
                    }
                    char sql[1024] = {0};
                    sprintf(sql, "INSERT INTO user (name,age,sex) VALUES ('%s', %d, '%s')", "zhangsan", 20, "男");
                    conn.update(sql);
                } });

        std::thread t3([]()
                       {
                //ConnectionPool *cp = ConnectionPool::getConnectionPool();
               for (int i = 0; i < 250; i++)
                {
                    Connection conn;
                    if (!conn.connect("localhost", 3306, "root", "Jjd040830", "test"))
                    {
                        std::cerr << "连接数据库失败\n";
                        return 1;
                    }
                    char sql[1024] = {0};
                    sprintf(sql, "INSERT INTO user (name,age,sex) VALUES ('%s', %d, '%s')", "zhangsan", 20, "男");
                    conn.update(sql);
                } });

        std::thread t4([]()
                       {
                //ConnectionPool *cp = ConnectionPool::getConnectionPool();
               for (int i = 0; i < 250; i++)
                {
                    Connection conn;
                    if (!conn.connect("localhost", 3306, "root", "Jjd040830", "test"))
                    {
                        std::cerr << "连接数据库失败\n";
                        return 1;
                    }
                    char sql[1024] = {0};
                    sprintf(sql, "INSERT INTO user (name,age,sex) VALUES ('%s', %d, '%s')", "zhangsan", 20, "男");
                    conn.update(sql);
                } });
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        auto end = std::chrono::high_resolution_clock::now();                                       // 结束计时
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); // 计算毫秒数
        std::cout << duration << '\n'; */

    // 多线程连接池测试
    auto begin = std::chrono::high_resolution_clock::now(); // 开始计时
    std::thread t1([]()
                   {
      MyConnectionPool *cp = MyConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; ++i)
        {
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                    "zhang san", 20, "male");
            std::shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);
        } });

    std::thread t2([]()
                   {
      MyConnectionPool *cp = MyConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; ++i)
        {
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                    "zhang san", 20, "male");
            std::shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);
        } });

    std::thread t3([]()
                   {
      MyConnectionPool *cp = MyConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; ++i)
        {
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                    "zhang san", 20, "male");
            std::shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);
        } });

    std::thread t4([]()
                   {
      MyConnectionPool *cp = MyConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; ++i)
        {
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                    "zhang san", 20, "male");
            std::shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);
        } });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    auto end = std::chrono::high_resolution_clock::now();                                       // 结束计时
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); // 计算毫秒数
    std::cout << duration << '\n';
    return 0;
}