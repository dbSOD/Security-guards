#pragma once


// CDisk �Ի���

class CDisk : public CDialogEx
{
	DECLARE_DYNAMIC(CDisk)

public:
	CDisk(LPCTSTR name, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDisk();
	CString m_path;

// �Ի�������
	enum { IDD = IDD_Disk };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Disk;
	CString m_DiskName;
	CString m_System;
	CString m_FreeSpace;
	CString m_TotalSpace;
	CString m_Info;
	virtual BOOL OnInitDialog();
};
