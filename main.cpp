#include "server.h"

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

//错误处理
int HandleSocketError(char *str)
{
	ShowSocketMsg(str);
	WSACleanup();
	return SERVER_API_ERROR;
}

//弹出消息处理
int ShowSocketMsg(char *str)
{
	MessageBox(NULL, TEXT("%s", str), TEXT("Error!"), MB_OKCANCEL);
}

bool WsaStartup(void)
{
	//初始化Windows sockets DLL
	WORD requse = MAKEWORD(2, 2);
	retVal = WSAStartup(requse, &wsaData);
	if (retVal!=0)
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
}

//创建套接字
bool Scokets(void)
{
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET==sServer)
	{
		 return HandleSocketError("Failed socket()!");
	}
}

//绑定套接字
bool Bind_socket(void)
{

}
