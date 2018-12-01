#pragma once
#include "afxcmn.h"


// CDia6 对话框

class CDia6 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia6)

public:
	CDia6(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia6();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrl;
	CImageList ImageList;	  //用于加载图标的ImageList对象
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStartService();
	static CString  m_ServiceId;	//保存服务名称的字符串
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFlushService();
	void ShowServices();	//遍历服务
};
