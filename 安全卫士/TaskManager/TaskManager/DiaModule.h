#pragma once
#include "afxcmn.h"


// CDiaModule 对话框

class CDiaModule : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaModule)

public:
	CDiaModule(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CDiaModule();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
	BOOL ModuleList(DWORD dwPid);	//遍历模块
};
