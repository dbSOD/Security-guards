#pragma once


// CDisk 对话框

class CDisk : public CDialogEx
{
	DECLARE_DYNAMIC(CDisk)

public:
	CDisk(LPCTSTR name, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDisk();
	CString m_path;

// 对话框数据
	enum { IDD = IDD_Disk };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
