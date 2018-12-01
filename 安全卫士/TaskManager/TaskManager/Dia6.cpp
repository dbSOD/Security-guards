// Dia6.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia6.h"
#include "afxdialogex.h"
#include <windows.h>
#include <winsvc.h>

//初始化保存服务ID的字符串
CString CDia6::m_ServiceId(" ");

// CDia6 对话框

IMPLEMENT_DYNAMIC(CDia6, CDialogEx)

CDia6::CDia6(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia6::IDD, pParent)
{

}

CDia6::~CDia6()
{
}

void CDia6::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDia6, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDia6::OnRclickList1)
	ON_COMMAND(ID_32796, &CDia6::OnStartService)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDia6::OnClickList1)
	ON_COMMAND(ID_32797, &CDia6::OnFlushService)
END_MESSAGE_MAP()


// CDia6 消息处理程序

void CDia6::ShowServices()
{
	//准备需要用到的内容
	CString Contect;	//内容
	//服务状态
	WCHAR* wcStatus[] = { L"已停止", L"正在启动", L"正在停止", L"正在运行", L"正在继续", L"正在暂停", L"已暂停" };
	//服务类型
	WCHAR* wcType[] = { L"系统加载", L"初始化加载", L"自动", L"手动", L"禁用" };
	
	//打开远程计算机服务控制管理器
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(L"打开服务管理器失败！");
		return;
	}
	//第一次调用 获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);

	//申请需要的内存 第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerivice = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];

	//第二次枚举
	BOOL bStatus = FALSE;
	bStatus = EnumServicesStatusEx(
		hSCM,					//SCM数据库句柄
		SC_ENUM_PROCESS_INFO,	//要返回的属性
		SERVICE_WIN32,			//服务类型
		SERVICE_STATE_ALL,		//服务状态
		(PBYTE)pEnumSerivice,	//接收信息缓冲区
		dwSize,					//缓冲区大小
		&dwSize,				//需要的大小
		&dwServiceNum,			//缓冲区中的服务个数
		NULL,
		NULL
		);
	//遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//获取基础信息 
		//服务名
		m_ListCtrl.InsertItem(i, pEnumSerivice[i].lpServiceName, 0);
		//服务状态 有已停止 正在运行 正在暂停
		m_ListCtrl.SetItemText(i, 3, wcStatus[pEnumSerivice[i].ServiceStatusProcess.dwCurrentState - 1]);
		//PID
		Contect.Format(L"%d", pEnumSerivice[i].ServiceStatusProcess.dwProcessId);
		m_ListCtrl.SetItemText(i, 1, Contect);
		//描述
		m_ListCtrl.SetItemText(i, 2, pEnumSerivice[i].lpDisplayName);
		//获取服务详细信息
		//打开服务
		SC_HANDLE hService = OpenService(hSCM, pEnumSerivice[i].lpServiceName, SERVICE_QUERY_CONFIG);
		//第一次调用获取需要的缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		//第二次调用 获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//获取启动类型
		m_ListCtrl.SetItemText(i, 4, wcType[pServiceConfig->dwStartType]);
	
	}
}



BOOL CDia6::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//初始化ListCtrl
	WCHAR* wcArr[] = { L"名称", L"PID", L"描述", L"状态", L"类型" };
	DWORD len = sizeof(wcArr) / sizeof(wcArr[0]);
	for (DWORD i = 0; i < len; i++)
	{
		m_ListCtrl.InsertColumn(i, wcArr[i], 0, 120);
	}
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	//添加图标
	//创建一个成员为32*32大小使用32位色的icon数组 初始空间为15 满了之后递增1
	ImageList.Create(32, 32, ILC_COLOR32, 1, 1);

	//拿到图标句柄
	HICON Ico = AfxGetApp()->LoadIcon(IDI_ICON17);
	//添加图标
	HICON IconArr[] = { Ico };
	for (int i = 0; i < 1; i++)
	{
		ImageList.Add(IconArr[i]);
	}
	//将CImageList关联到List控件上
	m_ListCtrl.SetImageList(&ImageList, LVSIL_SMALL);

	//遍历服务
	ShowServices();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//响应右键点击消息 弹出服务菜单
void CDia6::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//拿到点击那一行的服务名存起来
	m_ServiceId = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);

	CMenu Menu, *pMenu;
	//加载整个菜单
	Menu.LoadMenu(IDR_MENU2);
	//拿到菜单的第三列
	pMenu = Menu.GetSubMenu(3);
	//获取鼠标位置
	CPoint pt;
	GetCursorPos(&pt);
	//弹出菜单
	TrackPopupMenu(pMenu->m_hMenu,TPM_LEFTALIGN,pt.x,pt.y,0,m_hWnd,0);

	*pResult = 0;
}

//启动或关闭服务
void CDia6::OnStartService()
{
	//打开服务管理对象
	SC_HANDLE hSC = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSC==NULL)
	{
		MessageBox(L"打开服务管理器失败！");
		return;
	}
	//打开选中那一行的服务
	SC_HANDLE hSerivice = OpenService(hSC, m_ServiceId,SERVICE_START|SERVICE_QUERY_STATUS|SERVICE_STOP);
	if (hSerivice == NULL)
	{
		MessageBox(L"服务打开失败");
		CloseServiceHandle(hSerivice);
		return;
	}
	//获得服务状态
	SERVICE_STATUS status;
	if (QueryServiceStatus(hSerivice, &status)==FALSE)
	{
		MessageBox(L"获取服务状态失败！");
		CloseServiceHandle(hSerivice);
		CloseServiceHandle(hSC);
		return;
	}
	//如果服务正在运行关闭服务
	if (status.dwCurrentState==SERVICE_RUNNING)
	{
		//发送控制码
		if (ControlService(hSerivice,SERVICE_CONTROL_STOP,&status)==FALSE)
		{
			MessageBox(L"停止服务失败！");
			CloseServiceHandle(hSerivice);
			CloseServiceHandle(hSC);
			return;
		}
		//等待服务停止
		while (QueryServiceStatus(hSerivice,&status)==TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState==SERVICE_STOPPED)
			{
				MessageBox(L"停止服务成功！");
				CloseServiceHandle(hSerivice);
				CloseServiceHandle(hSC);
				ShowServices();	//刷新
				return;
			}
		}
	}
	//如果服务已停止就开启服务
	else if (status.dwCurrentState==SERVICE_STOPPED)
	{
		//启动服务
		if (StartService(hSerivice,NULL,NULL)==FALSE)
		{
			MessageBox(L"启动服务失败！");
			CloseServiceHandle(hSerivice);
			CloseServiceHandle(hSC);
			return;
		}
		//等待服务启动
		while (QueryServiceStatus(hSerivice,&status)==TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState==SERVICE_RUNNING)
			{
				MessageBox(L"启动服务成功！");
				CloseServiceHandle(hSerivice);
				CloseServiceHandle(hSC);
				ShowServices();	//刷新
				return;
			}
		}
		//循环结束完说明服务启动失败
		MessageBox(L"启动服务失败！");
		CloseServiceHandle(hSerivice); 
		CloseServiceHandle(hSC);
		return;
	}

}

//响应左键的点击消息
void CDia6::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//拿到服务名存起来
	m_ServiceId = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	*pResult = 0;
}

//刷新服务
void CDia6::OnFlushService()
{
	//遍历服务
	ShowServices();
}
