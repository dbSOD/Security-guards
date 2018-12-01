#pragma once
#include "afxcmn.h"
#include <TlHelp32.h>
#include "TaskManager.h"


// CDia1 对话框

class CDia1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia1)

public:
	CDia1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void ProcessList();	//遍历进程并刷新到列表控件

	DECLARE_MESSAGE_MAP()


public:
	CListCtrl m_ListCtrl;	//绑定listview的控件遍历
	virtual BOOL OnInitDialog();   
	CImageList ImageList;	  //用于加载图标的ImageList对象
	static CString  m_ProcessId;	//保存进程ID的字符串
	


	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRefresh();
	afx_msg void OnEixt();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnThreadList();
	afx_msg void OnLookModule();
	afx_msg void OnLookPile();
};


