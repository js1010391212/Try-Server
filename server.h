#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <winsock.h>
#include <iostream>
using namespace std;

#define SERVER_SETUP_FALL			1			//����������ʧ��
#define SERVER_PORT					5556		//������TCP�˿�
#define TIMEFOR_THREAD_EXIT			5000		//����˯��ʱ��
#define TIMEFOR_THREAD_SLEEP		500			//�ȴ��ͻ��������߳�˯��ʱ��

///////////////////////////////////////////////////////
//ȫ�ֱ���

HANDLE hThreadAccept;				//���ܿͻ��������߳̾��
HANDLE hThreadHelp;					//�ͷ���Դ�߳̾��
SOCKET sServer;						//�����׽���
BOOL bServerRunning;				//����������״̬
HANDLE hServerEvent;				//�������˳��¼�����
CLIENTLIST clientlist;				//�������ӵ�����
CRITICAL_SECTION csClientList;		//����������ٽ�������	

//�ͻ�����
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
