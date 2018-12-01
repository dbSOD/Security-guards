#pragma once
#include <vector>
#include "afxcmn.h"
#include "Md5.h"
#include "data.h"
#include "resource.h"

#define WM_MYUPDATEDATA				WM_USER+100 
using std::vector;

//病毒信息结构体
typedef struct _VIRINFO
{
	TCHAR szVirName[50];		//病毒名称
	TCHAR szVirPath[MAX_PATH];	//病毒路径
	TCHAR szVirSize[50];		//病毒大小
}VIRINFO, *PVIRINFO;

class CDia7 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia7)

public:
	CDia7(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia7();

// 对话框数据
	enum { IDD = IDD_DIALOG7 }; 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitSelect();		//初始化查杀选项
	void StartScan();		//开始扫描
	CString BrowseFolder(HWND hWnd, LPCTSTR lpTitle);	//弹出文件夹选择框
	void Scan_Single(LPCTSTR szPath);	//单个文件查杀
	void Scam_All(LPCTSTR szPath);		//全路径查杀
	BOOL MD5Scan(LPCTSTR szPath);		// MD5查杀
	CString GetShowSize(DOUBLE dwSize);	//获取病毒大小
	BOOL WhiteScan(LPCTSTR szPath);		//白名单查杀
	void InitSocket();				//初始化且连接scoket

	CString m_szMD5;		//保存文件的MD5值
	CListCtrl m_ListCtrl;
	BOOL IsSelect[6];	//杀毒选项是否被选中
	BOOL IsSingleScan;	//是否为单个文件查杀	否则为全路径
	BOOL IsMD5;			//是否为MD5查杀		否则为白名单
	BOOL IsLocal;		//是否为本地查杀		否则为云端

	//查杀路径保存
	CString m_SinglePath;	//保存单个文件查杀路径的字符串
	CString m_AllPath;		//保存全路径查杀的路径字符串
	afx_msg void OnBnClickedButton9();
	
	vector<VIRINFO> m_vecVirInfo;	//保存病毒信息Vector
	//病毒库加载信息
	vector<CString> m_LocalMD5;		//本地MD5库
	vector<CString> m_LocalWhite;	//本地白名单库
	vector<CString> m_ServerMD5;	//云端MD5库
	vector<CString> m_ServerWhite;	//云端白名单库
	CString m_szStatus;		//显示提示信息的static
	vector<CString> m_ProcessId;	//保存进程ID的结构体 用来结束进程用
	afx_msg void OnBnClickedButton8();

	
	CSocket m_Socket;		//连接云端服务器类对象
	void ProcessList();	//遍历进程并刷新到列表控件
protected:

	afx_msg LRESULT OnMyupdatedata(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton2();
};
