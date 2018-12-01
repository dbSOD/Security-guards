#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

//启动项信息结构体
typedef struct _BOOTINFO
{
	TCHAR szBootName[50];			//启动项
	TCHAR szBootOrder[255];			//命令
	TCHAR szHKEY[50];				//主键信息
	TCHAR szBootPos[255];			//删除时要用到的
	TCHAR szBootPos_All[255];		//List中要显示的
}BOOTINFO, *PBOOTINFO;

// CStartingItems 对话框

class CStartingItems : public CDialogEx
{
	DECLARE_DYNAMIC(CStartingItems)

public:
	CStartingItems(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartingItems();

// 对话框数据
	enum { IDD = IDD_StartItem };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();

	//获取启动项信息
	void GetBootInfo();
	//保存启动项信息的容器
	vector<BOOTINFO> m_vecBootInfo;
	afx_msg void OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddStart();
	afx_msg void OnRemoveStart();
	afx_msg void OnRefresh();

	//所选择的启动项
	int m_Select;
	CString strTemp;
};
