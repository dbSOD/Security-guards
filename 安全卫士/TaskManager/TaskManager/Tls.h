#pragma once


// CTls 对话框

class CTls : public CDialogEx
{
	DECLARE_DYNAMIC(CTls)

public:
	CTls(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTls();

// 对话框数据
	enum { IDD = IDD_TLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_StartAddress;		  //源数据的起始位置
	CString m_EndAddress;		  //源数据的终止位置
//	CString AddressOfIndex;		  
	CString m_AddressOfCallBacks; //TLS回调函数地址表的位置
	CString m_SizeOfZeroFill;	  //用0填充TLS变量区域的大小
	CString m_Characteristics;	  //保留 恒为0
	CString m_AddressOfIndex;	  //保留TLS索引的位置
};
