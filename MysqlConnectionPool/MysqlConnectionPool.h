#pragma once
#include<string>
#include<queue>
#include "Connection.h"
#include<mutex>
#include<atomic>
#include<thread>
#include<memory>
#include<functional>
#include<condition_variable>
/*
实现连接池模块
*/

class ConnectionPool
{
public:
	// 获取连接池对象实例
	static ConnectionPool* getConnectionPool();
	shared_ptr<Connection> getConnection();  //给外部提供接口,从连接池中获取一个可用的空闲连接

private:
	ConnectionPool();   // 单例1 构造函数私有化

	void produceConnectionTask();   //运行在独立的线程中，专门负责生产新连接

	bool loadConfigFile();  //从配置文件中加载配置项

	//启动一个新的定时线程,扫描超过maxIdleTime时间的空闲连接,进行多余的连接回收
	void scannerConnectionTask();


	std::string _ip;             //数据库ip地址
	unsigned short _port;        //数据库端口号 3306
	std::string _username;       //数据库登录用户名
	std::string _password;       //数据库登录密码
	std::string _dbname;         //数据库名称
	int _initSize;               //连接池的初始连接量
	int _maxSize;                //连接池最大连接量
	int _maxIdleTime;            //连接池最大空闲时间
	int _connectionTimeout;      //连接池获取的超时时间



	queue<Connection*> _connectionQue;  //存储myqsl连接的队列
	mutex _queueMutex;                  //维护连接的队列安全互斥锁
	atomic_int _connectionCnt;          //记录连接所创建的connection连接的总数量
	condition_variable cv;              //设置条件变量,用于连接生产线程和消费线程的通信

};