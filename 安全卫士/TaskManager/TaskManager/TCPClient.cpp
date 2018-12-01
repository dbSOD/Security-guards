#include "stdafx.h"
#include "TCPClient.h"
#include <process.h>


CTCPClient::CTCPClient()
{
	IsAviable=FALSE;
}


CTCPClient::~CTCPClient()
{
	WSACleanup();					    //�ͷ��׽�����Դ
}


DWORD ThreadProc(LPVOID prama)
{
	((CTCPClient*)prama)->m_ServerMD5.clear();
	SOCKET sockClient = ((CTCPClient*)prama)->m_sock;
	int    nRecvSize = 0;
	while (true)
	{
		char buf[2500];
		//��������
		nRecvSize = recv(sockClient, buf, 2500, 0);
		if (nRecvSize == SOCKET_ERROR)
		{
			//printf("��������˳���\n");
			//system("pause");
			exit(1);
		}
		else
		{
			//���ܳɹ�
			//����buffer
			CString RecBuf;
			char(*p)[50] = (char(*)[50])buf;
			int i = 0;
			while (p[i][0])
			{
				//CHAR* 
				int num = MultiByteToWideChar(0, 0, p[i], -1, NULL, 0);
				wchar_t *wide = new wchar_t[num];
				MultiByteToWideChar(0, 0, p[i], -1, wide, num);
				RecBuf = wide;
				((CTCPClient*)prama)->m_ServerMD5.push_back(RecBuf);
				i++;
				delete[] wide;
			}
			((CTCPClient*)prama)->IsAviable = TRUE;
		}
	}
}

BOOL CTCPClient::InitWinSock()
{
	WSADATA   stcData;
	int       nResult = 0;
	nResult = WSAStartup(MAKEWORD(2, 2), &stcData);
	if (nResult == SOCKET_ERROR)
	{
		return FALSE;
	}return TRUE;
}

BOOL CTCPClient::SendMyMessage(BOOL IsMD5)
{
	int nErrCode = 0;
	//1.��ʼ��SOCKET
	InitWinSock();
	//2.�����׽���
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//��

	//3.���ӷ�����
	nErrCode = connect(sock,			//�׽���
		(sockaddr*)&addr,				//IP��ַ
		sizeof(sockaddr_in));			//IP��ַ�Ĵ�С
	if (SOCKET_ERROR == nErrCode)
	{
		//printf("������������ˣ�\n");
		closesocket(sock);			    //�ر��׽���
		WSACleanup();				    //�ͷ��׽�����Դ
		return FALSE;
	}
 	char ScanType[15] = { 0 };			//Ҫ��ȡ����������
	//4.������Ϣ
	if (IsMD5)
	{
		ScanType[0] = 'm';
	}
	else
	{
		ScanType[0] = 'w';
	}

	nErrCode = send(sock, ScanType, 15, 0);
	m_sock = sock;
	if (SOCKET_ERROR == nErrCode)
	{
		//printf("������Ϣʧ��!%d\n", GetLastError());
	}
	//�����̴߳������˷��ͻ���������
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (LPVOID)this, 0, 0);
	return TRUE;
}

vector<CString> CTCPClient::RetVector()
{
	while (!IsAviable);
	IsAviable = FALSE;
	return m_ServerMD5;
}
