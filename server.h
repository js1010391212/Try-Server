#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <winsock.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#define MAX_NUM_BUF						255				//��󻺴�
#define SERVER_DLL_ERROR				1				//����Windows socket dll ʧ��
#define SERVER_API_ERROR                1
#define SERVER_PORT						4399			//�������˿�


///////////////////////////////////////////////////////////////
//ȫ�ֱ���
	char bufRecv[MAX_NUM_BUF];				//��������
	char bufSend[MAX_NUM_BUF];				//д������
	WSADATA wsaData;			
	SOCKET sServer;				//�����������׽���
	SOCKET sClient;				//���ܿͻ����׽���
	bool bConning;				//��ͻ�������״̬
	int retVal;
#endif // !SERVER_H
