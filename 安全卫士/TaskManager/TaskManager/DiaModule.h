#pragma once
#include "afxcmn.h"


// CDiaModule �Ի���

class CDiaModule : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaModule)

public:
	CDiaModule(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CDiaModule();

// �Ի�������
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
	BOOL ModuleList(DWORD dwPid);	//����ģ��
};
