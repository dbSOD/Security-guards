#pragma once

// CMySocket ����Ŀ��

class CMySocket : public CSocket
{
public:
	CMySocket();
	virtual ~CMySocket();
	virtual void OnReceive(int nErrorCode);
};


