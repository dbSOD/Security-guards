#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

#define Win32PATH _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
#define Win64PATH _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
//软件信息结构体
typedef struct _SOFTINFO
{
	TCHAR szSoftName[50];				//软件名称
	TCHAR szSoftVer[50];				//软件版本号
	TCHAR strSoftVenRel[50];			//软件发布厂商
	TCHAR szSoftData[20];				//软件安装日期
	TCHAR strSoftInsPath[MAX_PATH];		//软件安装路径
	TCHAR strSoftUniPath[MAX_PATH];		//软件卸载路径
}SOFTINFO, *PSOFTINFO;
// CSoftManage 对话框

class CSoftManage : public CDialogEx
{
	DECLARE_DYNAMIC(CSoftManage)

public:
	CSoftManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoftManage();

// 对话框数据
	enum { IDD = IDD_Soft };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrl;
	//遍历软件信息
	void GetSoftwareInfo();
	//保存软件相关信息
	vector<SOFTINFO> m_vecSoftInfo;
	//判断系统是否是64位
	BOOL Is64();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	//所选择的软件
	int m_Select;
	CString strTemp;
	afx_msg void OnUninstall();
	afx_msg void OnFlush();
	afx_msg void OnOpenSoft();
	BOOL OpenFolderAndSelectFile(TCHAR* szPath);
};
