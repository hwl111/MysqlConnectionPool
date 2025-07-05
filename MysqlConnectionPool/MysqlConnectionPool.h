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
ʵ�����ӳ�ģ��
*/

class ConnectionPool
{
public:
	// ��ȡ���ӳض���ʵ��
	static ConnectionPool* getConnectionPool();
	shared_ptr<Connection> getConnection();  //���ⲿ�ṩ�ӿ�,�����ӳ��л�ȡһ�����õĿ�������

private:
	ConnectionPool();   // ����1 ���캯��˽�л�

	void produceConnectionTask();   //�����ڶ������߳��У�ר�Ÿ�������������

	bool loadConfigFile();  //�������ļ��м���������

	//����һ���µĶ�ʱ�߳�,ɨ�賬��maxIdleTimeʱ��Ŀ�������,���ж�������ӻ���
	void scannerConnectionTask();


	std::string _ip;             //���ݿ�ip��ַ
	unsigned short _port;        //���ݿ�˿ں� 3306
	std::string _username;       //���ݿ��¼�û���
	std::string _password;       //���ݿ��¼����
	std::string _dbname;         //���ݿ�����
	int _initSize;               //���ӳصĳ�ʼ������
	int _maxSize;                //���ӳ����������
	int _maxIdleTime;            //���ӳ�������ʱ��
	int _connectionTimeout;      //���ӳػ�ȡ�ĳ�ʱʱ��



	queue<Connection*> _connectionQue;  //�洢myqsl���ӵĶ���
	mutex _queueMutex;                  //ά�����ӵĶ��а�ȫ������
	atomic_int _connectionCnt;          //��¼������������connection���ӵ�������
	condition_variable cv;              //������������,�������������̺߳������̵߳�ͨ��

};