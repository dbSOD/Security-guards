#pragma once


// CDelay 对话框

class CDelay : public CDialogEx
{
	DECLARE_DYNAMIC(CDelay)

public:
	CDelay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDelay();

// 对话框数据
	enum { IDD = IDD_Delay };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_AllAttributes;		//属性
	CString m_DllAllName;			//延迟加载的DLL名字
	CString m_ModuleHandleRVA;		//模块句柄
	CString m_ImportAddressRVA;		//IAT
	CString m_ImportNameRVA;		//INT
	CString m_BoundImportAddress;	//绑定IAT
	CString m_UnloadInfo;			//卸载信息
	CString m_TimeData;				//时间戳
};
