#include "main.h"

//初始化成员变量
void InitMember(void)
{
	//初始化读写缓存
	ZeroMemory(bufRecv, sizeof(bufRecv));
	ZeroMemory(bufSend, sizeof(bufSend));

	//初始化
	sServer = INVALID_SOCKET;
	sClient = INVALID_SOCKET;
	bConning = false;
	retVal = 0;
}

//弹出消息处理
void ShowSocketMsg(char *str)
{
	MessageBox(NULL, TEXT("%s", str), TEXT("Error!"), MB_OKCANCEL);
}

//错误处理
int HandleSocketError(char *str)
{
	ShowSocketMsg(str);
	WSACleanup();
	return SERVER_API_ERROR;
}

BOOL WsaStartup(void)
{
	//初始化Windows sockets DLL
	WORD requse = MAKEWORD(2, 2);
	retVal = WSAStartup(requse, &wsaData);
	if (retVal != 0)
	{
		printf("can not find a usable windows sockets dll!");
		return SERVER_DLL_ERROR;
	}
	//确保Windows DLL支持1.1
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		ShowSocketMsg("Can not find a usable Windows Sockets dll!");
		WSACleanup();
		return SERVER_DLL_ERROR;
	}
	return TRUE;
}

//创建套接字
BOOL Scokets(void)
{
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		return HandleSocketError("Failed socket()!");
	}
	return TRUE;
}

//绑定套接字
BOOL Bind_socket(void)
{
	//服务器套接字地址
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(SERVER_PORT);
	addrServ.sin_addr.S_un.S_addr = INADDR_ANY;

	//绑定套接字
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(addrServ));
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sServer);
		return HandleSocketError("Failed bind()!");
	}
	return TRUE;
}

//监听
BOOL Listen_S(void)
{
	retVal = listen(sServer, 5);
	if (retVal==SOCKET_ERROR)
	{
		closesocket(sServer);
		return HandleSocketError("Filed listen()");
	}
	printf("Server succeed!\n");
	printf("Waiting for new cilent()!\n");
	return TRUE;
}

//接受请求
void Accept(void)
{
	sockaddr_in Cclient;
	int CclientLen = sizeof(Cclient);
	sClient = accept(sServer, (sockaddr FAR*)&Cclient, &CclientLen);
	if (INVALID_SOCKET)
	{
		closesocket(sServer);
		HandleSocketError("flied accept()!");
	}
	else
	{
		bConning = true;
	}
}

//接受数据
BOOL RecvLine(SOCKET s,char* buf)
{
	BOOL retVal = TRUE;				//返回值
	BOOL bLineEnd = FALSE;			//行结束
	int nReadLen = 0;				//读入字节数
	int nDataLen = 0;				//数据长度

	while (!bLineEnd&&bConning)
	{
		nReadLen = recv(s, buf + nDataLen, 1, 0);
		//错误处理
		if (SOCKET_ERROR==nReadLen)
		{
			int nErrCode = WSAGetLastError();	//错误代码
			if (WSAENOTCONN==nErrCode)
			{
				ShowSocketMsg("The socket is not connected!");
			}
			else if (WSAESHUTDOWN==nErrCode)
			{
				ShowSocketMsg("The socket has been shup down!");
			}
			else if (WSAETIMEDOUT==nErrCode)
			{
				ShowSocketMsg("The socket has been dropped!");
			}
			else if (WSAECONNRESET==nErrCode)
			{
				ShowSocketMsg("ERROR!!!");
			}
			else{}

			retVal = FALSE;		//读数据失败
			break;
		}
		//客户端关闭
		if (0==nReadLen)
		{
			retVal = FALSE;
			break;
		}
		//读入数据
		if ('\n'==*(buf+nDataLen))
		{
			bLineEnd = TRUE;	//接受数据结束
		}
		else
		{
			nDataLen += nReadLen;
		}
	}
	return retVal;
}

//发送数据
BOOL SendLine(SOCKET s, char* str)
{
	int retVal;		//返回值
	retVal = send(s, str, strlen(str), 0);	//一次发送

	//错误处理
	if (SOCKET_ERROR==retVal)
	{
		int nErrCode = WSAGetLastError();	//错误代码
		if (WSAENOTCONN == nErrCode)
		{
			ShowSocketMsg("The socket is not connected!");
		}
		else if (WSAESHUTDOWN == nErrCode)
		{
			ShowSocketMsg("The socket has been shup down!");
		}
		else if (WSAETIMEDOUT == nErrCode)
		{
			ShowSocketMsg("The socket has been dropped!");
		}else{}

		return FALSE;	
	}
	return TRUE;
}

//退出
int ExitClient(int nExit)
{
	closesocket(sServer);
	closesocket(sClient);
	WSACleanup();
	return nExit;
}
//主函数入口
int main(int argc, char* argv[])
{
	WsaStartup();
	Scokets();
	Bind_socket();
	Listen_S();
	Accept();
	RecvLine(sClient, bufRecv);
	SendLine(sClient, bufSend);
	CopyMemory(bufSend, "hello client", sizeof(bufSend));
	if (!SendLine(sClient,bufSend))
	{
		return ExitClient(SERVER_API_ERROR);
	}
}