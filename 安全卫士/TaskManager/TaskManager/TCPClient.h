#pragma once
#include <vector>
#include<WinSock2.h>
#pragma comment(lib,"WS2_32.lib")
using namespace std;

class CTCPClient
{
public:
	CTCPClient();
	~CTCPClient();

	BOOL InitWinSock();

	BOOL SendMyMessage(BOOL IsMD5);
	SOCKET m_sock;
	vector<CString> m_ServerMD5;
	vector<CString>  RetVector();
	BOOL IsAviable;

};

