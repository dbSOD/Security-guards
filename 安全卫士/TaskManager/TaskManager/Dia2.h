#pragma once
#include "afxcmn.h"


// CDia2 对话框

class CDia2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia2)

public:
	CDia2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia2();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	static CListCtrl m_ListCtrl;
	static BOOL CALLBACK EnumWinProc(HWND hWnd, LPARAM lParam);	//枚举窗口的回调函数

	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReFresh();	//刷新窗口
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton8();
};
