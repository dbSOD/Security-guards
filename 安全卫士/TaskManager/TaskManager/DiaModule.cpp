// DiaModule.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "DiaModule.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "Dia1.h"


// CDiaModule 对话框

IMPLEMENT_DYNAMIC(CDiaModule, CDialogEx)

CDiaModule::CDiaModule(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaModule::IDD, pParent)
{
	
}

CDiaModule::~CDiaModule()
{
}

void CDiaModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDiaModule, CDialogEx)
END_MESSAGE_MAP()


// CDiaModule 消息处理程序

//遍历模块
BOOL CDiaModule::ModuleList(DWORD dwPid)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	//初始化模块信息结构体
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	//创建模块快照 1 快照类型 2 进程ID
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	//如果句柄无效就返回false
	if (hModuleSnap==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	//通过模块快照句柄获取第一个模块的信息
	if (!Module32First(hModuleSnap,&me32))
	{
		//获取失败则关闭句柄
		CloseHandle(hModuleSnap);
		return FALSE;
	}
	do 
	{	
		int i = 0;	//行号
		CString ModuleId;	  //模块ID
		CString ModuleSize;	  //模块大小
		CString ModuleCount;  //模块计数
		//数字转字符串
		ModuleId.Format(L"%d", me32.th32ModuleID);
		ModuleSize.Format(L"%d", me32.modBaseSize);
		ModuleCount.Format(L"%d", me32.ProccntUsage);

		//再这里把模块信息显示到列表
		m_ListCtrl.InsertItem(i, ModuleId);				//插入模块ID
		m_ListCtrl.SetItemText(i, 1, me32.szModule);	//插入模块名
		m_ListCtrl.SetItemText(i, 2, ModuleSize);		//插入模块大小
		m_ListCtrl.SetItemText(i, 3, ModuleCount);		//插入模块计数
		m_ListCtrl.SetItemText(i, 4, me32.szExePath);	//插入模块路径

	} while (Module32Next(hModuleSnap,&me32));
	return TRUE;
}

//初始化函数 插入模块信息
BOOL CDiaModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//添加列 1列号 2内容 3对齐方式 4列宽
	m_ListCtrl.InsertColumn(0, L"模块ID", 0, 80);
	m_ListCtrl.InsertColumn(1, L"模块名", 0, 110);
	m_ListCtrl.InsertColumn(2, L"模块大小", 0, 80);
	m_ListCtrl.InsertColumn(3, L"使用计数", 0, 80);
	m_ListCtrl.InsertColumn(4, L"模块路径", 0, 235);

	//遍历模块
	ModuleList(_ttoi(CDia1::m_ProcessId));
	//设置扩展属性
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
