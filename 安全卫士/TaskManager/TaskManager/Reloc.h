#pragma once
#include "afxcmn.h"


// CReloc �Ի���

class CReloc : public CDialogEx
{
	DECLARE_DYNAMIC(CReloc)

public:
	CReloc(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReloc();

// �Ի�������
	enum { IDD = IDD_Reloc };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrlUp;
	CListCtrl m_ListCtrlDown;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
