#include "server.h"

//��ʼ��ȫ�ֱ���
void InitMenber(void)
{
	InitializeCriticalSection(&csClientList);					//��ʼ���ٽ�ֵ
	hServerEvent = CreateEvent(NULL, TRUE, FALSE, NULL);		//�ֶ������¼�����ʼ��Ϊ���ź�״̬
	hThreadAccept = NULL;										//����ΪNULL
	hThreadHelp = NULL;											//����ΪNULL
	sServer = INVALID_SOCKET;									//����Ϊ��Ч�׽���
	bServerRunning = FALSE;										//������û������״̬
	csClientList.clear();										//�������
}

//��ʼ��socket
BOOL InitServer()
{
	int reVal;		//����ֵ

	//��ʼ��windows socket dll
	WSADATA wsData;
	reVal = WSAStartup(MAKEWORD(2, 2), &wsData);

	//�����׽���
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET) return FALSE;

	//�����׽��ַ�����ʽ
	unsigned long ul = 1;
	reVal = ioctlsocket(sServer, FIONBIO, (unsigned long*)&ul);
	if (reVal == SOCKET_ERROR) return FALSE;

	//���׽���
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(SERVER_PORT);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	reVal = bind(sServer, (struct sockaddr*)&serAddr, sizeof(serAddr));
	if (SOCKET_ERROR == reVal) return FALSE;

	//����
	reVal = listen(sServer, SOMAXCONN);
	if (reVal == SOCKET_ERROR) return FALSE;

	return TRUE;
}

//�����ͷ���Դ�̺߳ͽ��ܿͻ��������߳�
BOOL CreateHelpAndAcceptThread()
{
	bServerRunning = TRUE;

	//�����ͷ���Դ�߳�
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

	//�������ܿͻ��������߳�
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
//��������
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

//ֹͣ����
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
				Sleep(TIMEFOR_THREAD_EXIT);		//�߳�˯��
			}
		}
		else{
			Sleep(TIMEFOR_THREAD_EXIT);
		}
	}
	bServerRunning = FALSE;	
	ShowServerExitMsg();								//��ʾ�������˳���Ϣ
	Sleep(TIMEFOR_THREAD_EXIT);
	WaitForSingleObject(hServerEvent, INFINITE);		//�ȴ�������Դ�̷߳��͵���Ϣ

	return;
}

//�ͷ���Դ
void ExitServer(void)
{
	DeleteCriticalSection(&csClientList);		//�ͷ��ٽ�������
	CloseHandle(hServerEvent);					//�ͷ��¼�������
	closesocket(sServer);						//�ر�socket
	WSACleanup();								//ж��Windows socket dll
}

//���ܿͻ�������
DWORD _stdcall AcceptThread(void* pParam)
{

}

//���������
int main(int argc,char* argv[])
{
	//��ʼ��������
	if (!InitServer())
	{
		ExitServer();
		return SERVER_SETUP_FALL;
	}

	//��������
	if (!StartService())
	{
		ShowServerStartMsg();
		ExitServer();
		return SERVER_SETUP_FALL;
	}

	//ֹͣ����
	StopService();

	//�������˳�
	ExitServer();

	return 0;
}