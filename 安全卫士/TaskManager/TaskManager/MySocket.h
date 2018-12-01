#pragma once

// CMySocket ÃüÁîÄ¿±ê

class CMySocket : public CSocket
{
public:
	CMySocket();
	virtual ~CMySocket();
	virtual void OnReceive(int nErrorCode);
};


