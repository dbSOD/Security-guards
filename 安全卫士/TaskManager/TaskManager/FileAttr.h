#pragma once


// CFileAttr �Ի���

class CFileAttr : public CDialogEx
{
	DECLARE_DYNAMIC(CFileAttr)

public:
	CFileAttr(LPCTSTR file,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileAttr();
	CString m_CurrentFile;

// �Ի�������
	enum { IDD = IDD_FileAttr };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
	CString m_Type;
	CString m_Path;
	CString m_CreatTime;
	CString m_LVisitTime;
	CString m_LChangeTime;
	BOOL m_Read;
	BOOL m_Hide;
	virtual BOOL OnInitDialog();
	CString m_MD5;

	BOOL md5fileValue();
};
