#include "server.h"

//��ʼ����Ա����
void InitMember(void)
{
	//��ʼ����д����
	ZeroMemory(bufRecv, sizeof(bufRecv));
	ZeroMemory(bufSend, sizeof(bufSend));

	//��ʼ��
	sServer = INVALID_SOCKET;
	sClient = INVALID_SOCKET;
	bConning = false;
	retVal = 0;
}

//������
int HandleSocketError(char *str)
{
	ShowSocketMsg(str);
	WSACleanup();
	return SERVER_API_ERROR;
}

//������Ϣ����
int ShowSocketMsg(char *str)
{
	MessageBox(NULL, TEXT("%s", str), TEXT("Error!"), MB_OKCANCEL);
}

bool WsaStartup(void)
{
	//��ʼ��Windows sockets DLL
	WORD requse = MAKEWORD(2, 2);
	retVal = WSAStartup(requse, &wsaData);
	if (retVal!=0)
	{
		printf("can not find a usable windows sockets dll!");
		return SERVER_DLL_ERROR;
	}
	//ȷ��Windows DLL֧��1.1
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		ShowSocketMsg("Can not find a usable Windows Sockets dll!");
		WSACleanup();
		return SERVER_DLL_ERROR;
	}
}

//�����׽���
bool Scokets(void)
{
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET==sServer)
	{
		 return HandleSocketError("Failed socket()!");
	}
}

//���׽���
bool Bind_socket(void)
{

}
