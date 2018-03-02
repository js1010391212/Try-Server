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
	return true;
}

//创建套接字
bool Scokets(void)
{
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET==sServer)
	{
		 return HandleSocketError("Failed socket()!");
	}
	return true;
}

//绑定套接字
bool Bind_socket(void)
{
	//服务器套接字地址
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port =htons(SERVER_PORT);
	addrServ.sin_addr.S_un.S_addr = INADDR_ANY;

	//绑定套接字
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(addrServ));
	if (SOCKET_ERROR==retVal)
	{
		closesocket(sServer);
		return HandleSocketError("Failed bind()!");
	}
	return true;
}
