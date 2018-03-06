#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <winsock.h>
#include <iostream>
using namespace std;

#define SERVER_SETUP_FALL			1			//启动服务器失败
#define SERVER_PORT					5556		//服务器TCP端口
#define TIMEFOR_THREAD_EXIT			5000		//主程睡眠时间
#define TIMEFOR_THREAD_SLEEP		500			//等待客户端请求线程睡眠时间

///////////////////////////////////////////////////////
//全局变量

HANDLE hThreadAccept;				//接受客户端连接线程句柄
HANDLE hThreadHelp;					//释放资源线程句柄
SOCKET sServer;						//监听套接字
BOOL bServerRunning;				//服务器工作状态
HANDLE hServerEvent;				//服务器退出事件对象
CLIENTLIST clientlist;				//管理连接的链表
CRITICAL_SECTION csClientList;		//保护链表的临界区对象	

//客户端类
class CClient
{
public:
	CClient(const SOCKET sClient,const sockaddr_in &addrClient);
	virtual ~CClient();

private:

};

CClient::CClient(const SOCKET sClient, const sockaddr_in &addrClient)
{
}

CClient::~CClient()
{
}

#endif // !SERVER_H
