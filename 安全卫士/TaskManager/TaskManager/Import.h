#pragma once
#include "afxcmn.h"


// CImport 对话框

class CImport : public CDialogEx
{
	DECLARE_DYNAMIC(CImport)

public:
	CImport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImport();

// 对话框数据
	enum { IDD = IDD_Import };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrlUp;
	CListCtrl m_ListCtrlDown;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
