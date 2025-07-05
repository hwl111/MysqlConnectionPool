#include <iostream>
#include"Connection.h"
#include "MysqlConnectionPool.h"

int main()
{
    /*
    连接测试代码
    Connection conn;
    char sql[1024] = { 0 };
    sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
        "zhang san", 20, "male");
    conn.connect("127.0.0.1", 3306, "root", "songaptx4869", "chat");
    conn.update(sql);

    return 0;
    */
    //ConnectionPool* cp = ConnectionPool::getConnectionPool();
    //cp->loadConfigFile();

    clock_t begin = clock();

    for (int i = 0; i < 1000; i++)
    {
        //未使用连接池
        Connection conn;
        char sql[1024] = { 0 };
        sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
            "zhang san", 20, "male");
        conn.connect("127.0.0.1", 3306, "root", "songaptx4869", "chat");
        conn.update(sql);
    }

    clock_t end = clock();
    std::cout <<"未使用连接池(单线程) "<< (end - begin) << "ms" << endl;

    clock_t start = clock();

    ConnectionPool* cp = ConnectionPool::getConnectionPool();   //获取连接池
    for (int i = 0; i < 1000; i++)
    {
        shared_ptr<Connection> sp = cp->getConnection();
        char sql[1024] = { 0 };
        sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
            "zhang san", 20, "male");
        sp->update(sql);
    }
    clock_t finish = clock();
    std::cout << "使用连接池(单线程) "<<(finish - start) << "ms" << endl;

    //多线程测试

    clock_t begin_thread = clock();


    thread thread_t1([]() {
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; i++)
        {
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
                "zhang san", 20, "male");
            shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);
        }
        });
    thread thread_t2([]() {
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; i++)
        {
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
                "zhang san", 20, "male");
            shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);
        }
        });
    thread thread_t3([]() {
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; i++)
        {
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
                "zhang san", 20, "male");
            shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);
        }
        });
    thread thread_t4([]() {
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; i++)
        {
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
                "zhang san", 20, "male");
            shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);
        }
        });


    thread_t1.join();
    thread_t2.join();
    thread_t3.join();
    thread_t4.join();
    clock_t end_thread = clock();
    std::cout << "四线程,使用连接池 " << (end_thread - begin_thread) << "ms" << endl;

    clock_t start_thread = clock();

    thread t1([]() {
        for (int i = 0; i < 250; i++)
        {
            Connection conn;
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
                "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "songaptx4869", "chat");
            conn.update(sql);
        }
        });
    thread t2([]() {
        for (int i = 0; i < 250; i++)
        {
            Connection conn;
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
                "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "songaptx4869", "chat");
            conn.update(sql);
        }
        });
    thread t3([]() {
        for (int i = 0; i < 250; i++)
        {
            Connection conn;
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
                "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "songaptx4869", "chat");
            conn.update(sql);
        }
        });
    thread t4([]() {
        for (int i = 0; i < 250; i++)
        {
            Connection conn;
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // 注意末尾补全括号
                "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "songaptx4869", "chat");
            conn.update(sql);
        }
        });
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    clock_t finish_thread = clock();
    std::cout << "四线程,未使用连接池 " << (finish_thread - start_thread) << "ms" << endl;

    return 0;
}
