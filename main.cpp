#include "Connection.h"
#include "MyConnectionPool.h"
#include <iostream>

int main()
{
    /*     Connection conn;
        if (!conn.connect("localhost", 3306, "root","*****", "test"))
        {
            std::cerr << "连接数据库失败\n";
            return 1;
        }
        char sql[1024]={0};
        sprintf(sql,"INSERT INTO user (name,age,sex) VALUES ('%s', %d, '%s')", "zhangsan", 20, "男");
        conn.update(sql); */
    // 测试连接池
    MyConnectionPool *pool = MyConnectionPool::getConnectionPool();
    return 0;
}