#pragma once
#include "afxcmn.h"


// CSection �Ի���

class CSection : public CDialogEx
{
	DECLARE_DYNAMIC(CSection)

public:
	CSection(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSection();

// �Ի�������
	enum { IDD = IDD_Section };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
};
