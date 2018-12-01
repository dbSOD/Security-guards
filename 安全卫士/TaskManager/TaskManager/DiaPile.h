#pragma once
#include "afxcmn.h"


// CDiaPile 对话框

class CDiaPile : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaPile)

public:
	CDiaPile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaPile();

// 对话框数据
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
	BOOL PileList(DWORD dwPid);		//遍历堆
};
