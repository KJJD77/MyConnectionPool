cmake_minimum_required(VERSION 3.10)

# 项目名称
project(MyConnectionPool)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加源文件
set(SOURCES
    main.cpp
    Connection.cpp
    MyConnectionPool.cpp
)

# 手动指定 MySQL 的头文件和库路径
include_directories(/usr/include/mysql) # MySQL 头文件路径
link_directories(/usr/lib/x86_64-linux-gnu) # MySQL 库路径

# 添加可执行文件
add_executable(MyConnectionPool ${SOURCES})

# 链接 MySQL 客户端库
target_link_libraries(MyConnectionPool mysqlclient)