#pragma once
#include "afxcmn.h"
#include<TlHelp32.h>



// CDiaThread �Ի���

class CDiaThread : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaThread)

public:
	CDiaThread(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaThread();

// �Ի�������
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
	void ThreadsList(DWORD dwPid);	//�����߳�

	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStopThread();
	static CString m_ThreadId;
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReFreshThread();
	afx_msg void OnReStartThread();
	afx_msg void OnKillThread();
};
