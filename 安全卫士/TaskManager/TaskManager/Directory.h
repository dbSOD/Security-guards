#pragma once


// CDirectory 对话框

class CDirectory : public CDialogEx
{
	DECLARE_DYNAMIC(CDirectory)

public:
	CDirectory(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDirectory();

// 对话框数据
	enum { IDD = IDD_Directory };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_ExportRVA;		//导出表RVA
	CString m_ExportSize;		//导出表Size
	CString m_ImportRVA;		//导入表RVA
	CString m_ImportSize;		//导入表Size
	CString m_ResourceRVA;		//资源表RVA
	CString m_ResourceSize;		//资源表Size
	CString m_RelocRVA;			//重定位表RVA
	CString m_RelocSize;		//重定位表Size
	CString m_TLSRVA;			//TLS表RVA
	CString m_TLSSize;			//TLS表Size
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton5();
	CString m_DelayRVA;			//延迟载入表RVA
	CString m_DelaySize;		//延迟载入表Size
	afx_msg void OnBnClickedButton10();
};
