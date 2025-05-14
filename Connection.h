#pragma once

#include "public.h"
#include <mysql/mysql.h>
#include <string>
#include <ctime>

// 数据库操作类
class Connection
{
public:
    // 初始化数据库连接
    Connection();
    
    // 释放数据库连接资源
    ~Connection();

    // 连接数据库
    bool connect(std::string ip, unsigned short port, std::string user, std::string password,
                 std::string dbname);

    // 更新操作 insert、delete、update
    bool update(std::string sql);

    // 查询操作 select
    MYSQL_RES *query(std::string sql);
    
    //刷新存活时间起始点
    void refreshAliveTime();

    //获取当前存活时间
    clock_t getAliveTime() const;
private:
    MYSQL *_conn; // 表示和MySQL Server的一条连接
    clock_t _alivetime;
};