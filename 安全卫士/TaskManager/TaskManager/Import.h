#pragma once
#include "afxcmn.h"


// CImport �Ի���

class CImport : public CDialogEx
{
	DECLARE_DYNAMIC(CImport)

public:
	CImport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImport();

// �Ի�������
	enum { IDD = IDD_Import };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrlUp;
	CListCtrl m_ListCtrlDown;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
