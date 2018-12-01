#pragma once
#include "afxcmn.h"


// CResoure 对话框

class CResoure : public CDialogEx
{
	DECLARE_DYNAMIC(CResoure)

public:
	CResoure(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResoure();

// 对话框数据
	enum { IDD = IDD_Resoure };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeCtrl;
	virtual BOOL OnInitDialog();
	CString m_FirstEdit;
	CString m_SecondEidt;
	afx_msg void OnClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_ThirdEidt;
	CString m_FouthEdit;
	CString m_RVAEdit;
	CString m_FOAEdit;
	CString m_SizeEdit;
	afx_msg void OnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	DWORD pRhead;	//资源表头
};
