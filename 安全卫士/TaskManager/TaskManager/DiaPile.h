#pragma once
#include "afxcmn.h"


// CDiaPile �Ի���

class CDiaPile : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaPile)

public:
	CDiaPile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaPile();

// �Ի�������
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
	BOOL PileList(DWORD dwPid);		//������
};
