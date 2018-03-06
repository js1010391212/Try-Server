#include "server.h"

//初始化全局变量
void InitMenber(void)
{
	InitializeCriticalSection(&csClientList);					//初始化临界值
	hServerEvent = CreateEvent(NULL, TRUE, FALSE, NULL);		//手动设置事件，初始化为无信号状态
	hThreadAccept = NULL;										//设置为NULL
	hThreadHelp = NULL;											//设置为NULL
	sServer = INVALID_SOCKET;									//设置为无效套接字
	bServerRunning = FALSE;										//服务器没有运行状态
	csClientList.clear();										//清空链表
}

//初始化socket
BOOL InitServer()
{
	int reVal;		//返回值

	//初始化windows socket dll
	WSADATA wsData;
	reVal = WSAStartup(MAKEWORD(2, 2), &wsData);

	//创建套接字
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET) return FALSE;

	//设置套接字非阻塞式
	unsigned long ul = 1;
	reVal = ioctlsocket(sServer, FIONBIO, (unsigned long*)&ul);
	if (reVal == SOCKET_ERROR) return FALSE;

	//绑定套接字
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(SERVER_PORT);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	reVal = bind(sServer, (struct sockaddr*)&serAddr, sizeof(serAddr));
	if (SOCKET_ERROR == reVal) return FALSE;

	//监听
	reVal = listen(sServer, SOMAXCONN);
	if (reVal == SOCKET_ERROR) return FALSE;

	return TRUE;
}

//创建释放资源线程和接受客户端请求线程
BOOL CreateHelpAndAcceptThread()
{
	bServerRunning = TRUE;

	//创建释放资源线程
	unsigned long ulThreadId;
	hThreadHelp = CreateThread(NULL, 0, HelperThread, NULL, 0, &ulThreadId);
	if (NULL==hThreadHelp)
	{
		bServerRunning = FALSE;
		return FALSE;
	}
	else
	{
		CloseHandle(hThreadHelp);
	}

	//创建接受客户端请求线程
	hThreadAccept = CreateThread(NULL, 0, AcceptThread, NULL, 0, &ulThreadId);
	if (NULL==hThreadAccept)
	{
		bServerRunning = FALSE;
		return FALSE;
	}
	else
	{
		CloseHandle(hThreadAccept);
	}
	return TRUE;
}
//启动服务
BOOL StartService(void)
{
	BOOL reVal=TRUE;
	ShowTipMsg(TRUE);

	char cInput;
	do 
	{
		cin >> cInput;
		if ('s'==cInput||'S'==cInput)
		{
			if (CreateHelpAndAcceptThread())
			{
				ShowServerStartMsg();
			}
			else{
				reVal = FALSE;
			}
			break;
		}
		else{
			ShowTipMsg(TRUE);
		}
	} while (cInput!='s'&&cInput!='S');

	return reVal;
}

//停止服务
void StopService(void)
{
	BOOL reVal = TRUE;
	ShowTipMsg(FALSE);

	char cInput;
	for (;bServerRunning;)
	{
		cin >> cInput;
		if (cInput=='E'||cInput=='e')
		{
			if (IDOK==MessageBox(NULL,"Are You Sure?","Server",MB_OKCANCEL))
			{
				break;
			}
			else{
				Sleep(TIMEFOR_THREAD_EXIT);		//线程睡眠
			}
		}
		else{
			Sleep(TIMEFOR_THREAD_EXIT);
		}
	}
	bServerRunning = FALSE;	
	ShowServerExitMsg();								//显示服务器退出信息
	Sleep(TIMEFOR_THREAD_EXIT);
	WaitForSingleObject(hServerEvent, INFINITE);		//等待清理资源线程发送的消息

	return;
}

//释放资源
void ExitServer(void)
{
	DeleteCriticalSection(&csClientList);		//释放临界区对象
	CloseHandle(hServerEvent);					//释放事件对象句柄
	closesocket(sServer);						//关闭socket
	WSACleanup();								//卸载Windows socket dll
}

//接受客户端请求
DWORD _stdcall AcceptThread(void* pParam)
{

}

//主程序入口
int main(int argc,char* argv[])
{
	//初始化服务器
	if (!InitServer())
	{
		ExitServer();
		return SERVER_SETUP_FALL;
	}

	//启动服务
	if (!StartService())
	{
		ShowServerStartMsg();
		ExitServer();
		return SERVER_SETUP_FALL;
	}

	//停止服务
	StopService();

	//服务器退出
	ExitServer();

	return 0;
}