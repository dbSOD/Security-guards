#pragma once


// CFileAttr 对话框

class CFileAttr : public CDialogEx
{
	DECLARE_DYNAMIC(CFileAttr)

public:
	CFileAttr(LPCTSTR file,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileAttr();
	CString m_CurrentFile;

// 对话框数据
	enum { IDD = IDD_FileAttr };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
