#pragma once
#include "afxcmn.h"


// CSection 对话框

class CSection : public CDialogEx
{
	DECLARE_DYNAMIC(CSection)

public:
	CSection(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSection();

// 对话框数据
	enum { IDD = IDD_Section };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
};
