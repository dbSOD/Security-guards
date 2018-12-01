#include "stdafx.h"
#include "TCPClient.h"
#include <process.h>


CTCPClient::CTCPClient()
{
	IsAviable=FALSE;
}


CTCPClient::~CTCPClient()
{
	WSACleanup();					    //释放套接字资源
}


DWORD ThreadProc(LPVOID prama)
{
	((CTCPClient*)prama)->m_ServerMD5.clear();
	SOCKET sockClient = ((CTCPClient*)prama)->m_sock;
	int    nRecvSize = 0;
	while (true)
	{
		char buf[2500];
		//接收数据
		nRecvSize = recv(sockClient, buf, 2500, 0);
		if (nRecvSize == SOCKET_ERROR)
		{
			//printf("服务端已退出！\n");
			//system("pause");
			exit(1);
		}
		else
		{
			//接受成功
			//接受buffer
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
	//1.初始化SOCKET
	InitWinSock();
	//2.创建套接字
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//绑定

	//3.连接服务器
	nErrCode = connect(sock,			//套接字
		(sockaddr*)&addr,				//IP定址
		sizeof(sockaddr_in));			//IP定址的大小
	if (SOCKET_ERROR == nErrCode)
	{
		//printf("请先启动服务端！\n");
		closesocket(sock);			    //关闭套接字
		WSACleanup();				    //释放套接字资源
		return FALSE;
	}
 	char ScanType[15] = { 0 };			//要获取的数据类型
	//4.发送消息
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
		//printf("发送消息失败!%d\n", GetLastError());
	}
	//创建线程处理服务端发送回来的数据
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (LPVOID)this, 0, 0);
	return TRUE;
}

vector<CString> CTCPClient::RetVector()
{
	while (!IsAviable);
	IsAviable = FALSE;
	return m_ServerMD5;
}
