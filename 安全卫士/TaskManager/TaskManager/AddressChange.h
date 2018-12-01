#pragma once


// CAddressChange 对话框

class CAddressChange : public CDialogEx
{
	DECLARE_DYNAMIC(CAddressChange)

public:
	CAddressChange(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddressChange();

// 对话框数据
	enum { IDD = IDD_AddressChange };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_VA;
	CString m_RVA;
	CString m_Offect;
	CString m_ExInfo;
	CString m_Section;
	CString m_Bytes;
	afx_msg void OnBnClickedButton2();

	static DWORD RVAtoFOA(DWORD dwRVA);	//地址转换函数
	afx_msg void OnEnChangeEdit3();
};
