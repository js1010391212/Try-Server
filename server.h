#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <winsock.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#define MAX_NUM_BUF						255				//最大缓存
#define SERVER_DLL_ERROR				1				//调用Windows socket dll 失败
#define SERVER_API_ERROR                1
#define SERVER_PORT						4399			//服务器端口


///////////////////////////////////////////////////////////////
//全局变量
	char bufRecv[MAX_NUM_BUF];				//读缓冲区
	char bufSend[MAX_NUM_BUF];				//写缓冲区
	WSADATA wsaData;			
	SOCKET sServer;				//服务器监听套接字
	SOCKET sClient;				//接受客户端套接字
	bool bConning;				//与客户端连接状态
	int retVal;
#endif // !SERVER_H
