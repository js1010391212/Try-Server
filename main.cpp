#include "main.h"

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

//������Ϣ����
void ShowSocketMsg(char *str)
{
	MessageBox(NULL, TEXT("%s", str), TEXT("Error!"), MB_OKCANCEL);
}

//������
int HandleSocketError(char *str)
{
	ShowSocketMsg(str);
	WSACleanup();
	return SERVER_API_ERROR;
}

BOOL WsaStartup(void)
{
	//��ʼ��Windows sockets DLL
	WORD requse = MAKEWORD(2, 2);
	retVal = WSAStartup(requse, &wsaData);
	if (retVal != 0)
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
	return TRUE;
}

//�����׽���
BOOL Scokets(void)
{
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		return HandleSocketError("Failed socket()!");
	}
	return TRUE;
}

//���׽���
BOOL Bind_socket(void)
{
	//�������׽��ֵ�ַ
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(SERVER_PORT);
	addrServ.sin_addr.S_un.S_addr = INADDR_ANY;

	//���׽���
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(addrServ));
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sServer);
		return HandleSocketError("Failed bind()!");
	}
	return TRUE;
}

//����
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

//��������
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

//��������
BOOL RecvLine(SOCKET s,char* buf)
{
	BOOL retVal = TRUE;				//����ֵ
	BOOL bLineEnd = FALSE;			//�н���
	int nReadLen = 0;				//�����ֽ���
	int nDataLen = 0;				//���ݳ���

	while (!bLineEnd&&bConning)
	{
		nReadLen = recv(s, buf + nDataLen, 1, 0);
		//������
		if (SOCKET_ERROR==nReadLen)
		{
			int nErrCode = WSAGetLastError();	//�������
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

			retVal = FALSE;		//������ʧ��
			break;
		}
		//�ͻ��˹ر�
		if (0==nReadLen)
		{
			retVal = FALSE;
			break;
		}
		//��������
		if ('\n'==*(buf+nDataLen))
		{
			bLineEnd = TRUE;	//�������ݽ���
		}
		else
		{
			nDataLen += nReadLen;
		}
	}
	return retVal;
}

//��������
BOOL SendLine(SOCKET s, char* str)
{
	int retVal;		//����ֵ
	retVal = send(s, str, strlen(str), 0);	//һ�η���

	//������
	if (SOCKET_ERROR==retVal)
	{
		int nErrCode = WSAGetLastError();	//�������
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

//�˳�
int ExitClient(int nExit)
{
	closesocket(sServer);
	closesocket(sClient);
	WSACleanup();
	return nExit;
}
//���������
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