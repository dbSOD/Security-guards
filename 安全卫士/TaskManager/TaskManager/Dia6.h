#pragma once
#include "afxcmn.h"


// CDia6 �Ի���

class CDia6 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia6)

public:
	CDia6(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia6();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrl;
	CImageList ImageList;	  //���ڼ���ͼ���ImageList����
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStartService();
	static CString  m_ServiceId;	//����������Ƶ��ַ���
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFlushService();
	void ShowServices();	//��������
};
