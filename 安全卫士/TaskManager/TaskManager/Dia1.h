#pragma once
#include "afxcmn.h"
#include <TlHelp32.h>
#include "TaskManager.h"


// CDia1 �Ի���

class CDia1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia1)

public:
	CDia1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia1();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void ProcessList();	//�������̲�ˢ�µ��б�ؼ�

	DECLARE_MESSAGE_MAP()


public:
	CListCtrl m_ListCtrl;	//��listview�Ŀؼ�����
	virtual BOOL OnInitDialog();   
	CImageList ImageList;	  //���ڼ���ͼ���ImageList����
	static CString  m_ProcessId;	//�������ID���ַ���
	


	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRefresh();
	afx_msg void OnEixt();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnThreadList();
	afx_msg void OnLookModule();
	afx_msg void OnLookPile();
};


