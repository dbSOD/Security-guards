#pragma once
#include "afxcmn.h"


// CExport 对话框

class CExport : public CDialogEx
{
	DECLARE_DYNAMIC(CExport)

public:
	CExport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExport();

// 对话框数据
	enum { IDD = IDD_Export };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_Name;		//名称 这是个RVA
	CString m_Offset;	//输出表偏移
	CString m_Charact;	//特征值
	CString m_Base;		//基址
	CString m_NameSrtring;	//名称字串 
	CString m_NumberOfFun;	//函数数量
	CString m_NumberOfName; //函数名称数量
	CString m_AddressOfFun;
	CString m_AddressOfName;
	CString m_AddressOfOrdinal;
	CListCtrl m_ListCtrl;
};
