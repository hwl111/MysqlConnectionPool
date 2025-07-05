#include <iostream>
#include"Connection.h"
#include "MysqlConnectionPool.h"

int main()
{
    /*
    ���Ӳ��Դ���
    Connection conn;
    char sql[1024] = { 0 };
    sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
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
        //δʹ�����ӳ�
        Connection conn;
        char sql[1024] = { 0 };
        sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
            "zhang san", 20, "male");
        conn.connect("127.0.0.1", 3306, "root", "songaptx4869", "chat");
        conn.update(sql);
    }

    clock_t end = clock();
    std::cout <<"δʹ�����ӳ�(���߳�) "<< (end - begin) << "ms" << endl;

    clock_t start = clock();

    ConnectionPool* cp = ConnectionPool::getConnectionPool();   //��ȡ���ӳ�
    for (int i = 0; i < 1000; i++)
    {
        shared_ptr<Connection> sp = cp->getConnection();
        char sql[1024] = { 0 };
        sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
            "zhang san", 20, "male");
        sp->update(sql);
    }
    clock_t finish = clock();
    std::cout << "ʹ�����ӳ�(���߳�) "<<(finish - start) << "ms" << endl;

    //���̲߳���

    clock_t begin_thread = clock();


    thread thread_t1([]() {
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; i++)
        {
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
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
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
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
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
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
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
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
    std::cout << "���߳�,ʹ�����ӳ� " << (end_thread - begin_thread) << "ms" << endl;

    clock_t start_thread = clock();

    thread t1([]() {
        for (int i = 0; i < 250; i++)
        {
            Connection conn;
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
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
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
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
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
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
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",  // ע��ĩβ��ȫ����
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
    std::cout << "���߳�,δʹ�����ӳ� " << (finish_thread - start_thread) << "ms" << endl;

    return 0;
}
