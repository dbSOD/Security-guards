#pragma once
#include "afxcmn.h"


// CExport �Ի���

class CExport : public CDialogEx
{
	DECLARE_DYNAMIC(CExport)

public:
	CExport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExport();

// �Ի�������
	enum { IDD = IDD_Export };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_Name;		//���� ���Ǹ�RVA
	CString m_Offset;	//�����ƫ��
	CString m_Charact;	//����ֵ
	CString m_Base;		//��ַ
	CString m_NameSrtring;	//�����ִ� 
	CString m_NumberOfFun;	//��������
	CString m_NumberOfName; //������������
	CString m_AddressOfFun;
	CString m_AddressOfName;
	CString m_AddressOfOrdinal;
	CListCtrl m_ListCtrl;
};
