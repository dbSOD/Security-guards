#pragma once
#include "afxcmn.h"


// CDia2 �Ի���

class CDia2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia2)

public:
	CDia2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia2();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	static CListCtrl m_ListCtrl;
	static BOOL CALLBACK EnumWinProc(HWND hWnd, LPARAM lParam);	//ö�ٴ��ڵĻص�����

	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReFresh();	//ˢ�´���
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton8();
};
