#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDia4 对话框

class CDia4 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia4)

public:
	CDia4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia4();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ComBox;
	CListCtrl m_ListCtrl;
	afx_msg void OnBnClickedButton1();
	void ShowFileList(LPCTSTR FullPath);
	void UpdateDir(LPCTSTR FullPath);
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPopFileAttr();
	afx_msg void OnItemactivateList1(NMHDR *pNMHDR, LRESULT *pResult);
};
