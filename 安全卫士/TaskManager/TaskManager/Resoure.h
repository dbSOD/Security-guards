#pragma once
#include "afxcmn.h"


// CResoure �Ի���

class CResoure : public CDialogEx
{
	DECLARE_DYNAMIC(CResoure)

public:
	CResoure(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResoure();

// �Ի�������
	enum { IDD = IDD_Resoure };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeCtrl;
	virtual BOOL OnInitDialog();
	CString m_FirstEdit;
	CString m_SecondEidt;
	afx_msg void OnClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_ThirdEidt;
	CString m_FouthEdit;
	CString m_RVAEdit;
	CString m_FOAEdit;
	CString m_SizeEdit;
	afx_msg void OnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	DWORD pRhead;	//��Դ��ͷ
};
