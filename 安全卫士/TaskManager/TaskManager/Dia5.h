#pragma once


// CDia5 对话框

class CDia5 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia5)

public:
	CDia5(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia5();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString ExtendString;				//保存扩展名的字符串
	CString m_NumberOfSection;			//区段数
	CString m_TimeDateStamp;		    //创建时间
	CString m_SizeOfOptionalHeader;		//可选头大小
	CString m_characteristic;			//特征值 也叫文件属性
	CString m_AddressOfEntryPoint;		//程序入口点
	CString m_SizeOfHeaders;			//所有的头部的大小
	CString m_FileAlignment;			//文件对齐
	CString m_SizeOfImage;				//映像大小
	CString m_ImageBase;				//默认加载基址
	CString m_SectionAlignment;			//内存对齐
	afx_msg void OnBnClickedButton7();
	
	static char* g_lpBase;	//文件在内存中的首地址
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton4();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();

	void SavePid();		//保存PID
};
