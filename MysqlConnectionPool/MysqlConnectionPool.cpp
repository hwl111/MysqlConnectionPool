// MysqlConnectionPool.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "MysqlConnectionPool.h"
#include "public.h"

ConnectionPool::ConnectionPool() {
	// 连接池构造
	//加载配置项
	if (!loadConfigFile())
	{
		return;
	}

	//创建初始数量连接
	for (int i = 0; i < _initSize; ++i)
	{
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveTime();     //刷新一下开始空闲的起始时间
		_connectionQue.push(p);
		_connectionCnt++;
	}
	//启动一个新线程,做为连接生产者
	thread produce(std::bind(&ConnectionPool::produceConnectionTask, this)); //给成员方法绑定当前对象
	produce.detach();


	//启动一个新的定时线程,扫描超过maxIdleTime时间的空闲连接,进行多余的连接回收
	thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
	scanner.detach();
}

//线程安全的懒汉单例函数接口
ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool pool; //lock和unlock
	return &pool;
}

shared_ptr<Connection> ConnectionPool::getConnection()
{
	unique_lock<mutex>lock(_queueMutex);
	while (_connectionQue.empty())
	{
		//sleep
		if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeout)))
		{
			if (_connectionQue.empty())
			{
				LOG("获取连接超时,获取连接失败");
				return nullptr;
			}
		}  
	}
	/*
	shared_ptr智能指针析构时会把connection资源delete
	相当于调用connection析构函数,connection就被close
	这里需要自定义shared_ptr的释放资源的方式,把connection直接归还到queue当中
	*/

	shared_ptr<Connection> sp(_connectionQue.front(),
		[&](Connection* pcon)
		{
			//这里在服务器应用线程中调用的,要考虑线程队列的安全
			unique_lock<mutex>lock(_queueMutex);
			pcon->refreshAliveTime();
			_connectionQue.push(pcon);
		}
	);
	_connectionQue.pop();
	if (_connectionQue.empty())
	{
		//谁消费队列最后一个connection,通知其他线程可以消费
		cv.notify_all();
	}

	return sp;
}

//从配置文件中加载配置项
bool ConnectionPool::loadConfigFile()
{
	FILE* pf = fopen("mysql.ini", "r");
	if (pf == nullptr)
	{
		LOG("mysql.ini file is not exist");
		return false;
	}
	while (!feof(pf))
	{
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		std::string str = line;
		int idx = str.find('=', 0);
		if (idx == -1) //无效配置项
		{
			continue;
		}
		int endidx = str.find('\n', idx);
		std::string key = str.substr(0, idx);
		string value = str.substr(idx + 1, endidx - idx - 1);
		
		if (key == "ip")
			_ip = value;
		if (key == "port")
			_port = atoi(value.c_str());
		if (key == "username")
			_username = value;
		if (key == "password")
			_password = value;
		if (key == "dbname")
			_dbname = value;
		if(key == "initSize")
			_initSize = atoi(value.c_str());
		if(key == "maxSize")
			_maxSize = atoi(value.c_str());
		if(key == "maxIdleTime")
			_maxIdleTime = atoi(value.c_str());
		if(key == "connectionTimer")
			_connectionTimeout = atoi(value.c_str());
	}
	return true;
}


//启动一个新的定时线程,扫描超过maxIdleTime时间的空闲连接,进行多余的连接回收
void ConnectionPool::scannerConnectionTask()
{
	for (;;)
	{
		//通过sleep模拟睡眠效果
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));
		
		//扫描整个队列,释放多余的连接
		unique_lock<mutex> lock(_queueMutex);
		while (_connectionCnt > _initSize)
		{
			Connection* p = _connectionQue.front();
			if (p->getAliveeTime() >= (_maxIdleTime * 1000))
			{
				_connectionQue.pop();
				_connectionCnt--;
				delete p;  //调用~connection()释放连接
			}
			else
			{
				break;
				//队头连接没超过_maxIdleTime,其他连接也没超过
			}
		}

	}

}

//运行在独立的线程中，专门负责生产新连接
void ConnectionPool :: produceConnectionTask()
{
	for (;;)
	{
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQue.empty())
		{
			cv.wait(lock);  //队列不为空,此处生产者线程进入等待状态

		}
		//连接数量没有到达上限,继续创建新的连接
		if (_connectionCnt < _maxSize)
		{
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->refreshAliveTime();     //刷新一下开始空闲的起始时间
			_connectionQue.push(p);
			_connectionCnt++;
		}

		cv.notify_all(); //通知消费者线程可以消费连接了
	}

}