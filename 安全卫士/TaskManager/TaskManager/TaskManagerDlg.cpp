
// TaskManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManagerDlg.h"
#include "afxdialogex.h"
#include <Psapi.h>
#include <powrprof.h>	
#pragma comment(lib, "PowrProf.lib")



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnDropFiles(HDROP hDropInfo);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTaskManagerDlg 对话框



CTaskManagerDlg::CTaskManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskManagerDlg::IDD, pParent)
{


}

void CTaskManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MyTable);
}

BEGIN_MESSAGE_MAP(CTaskManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTaskManagerDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON5, &CTaskManagerDlg::OnBnClickedButton5)

	ON_COMMAND(ID_32800, &CTaskManagerDlg::OnLockScreen)
	ON_COMMAND(ID_32771, &CTaskManagerDlg::OnShutDown)
	ON_COMMAND(ID_32772, &CTaskManagerDlg::OnRestart)
	ON_COMMAND(ID_32801, &CTaskManagerDlg::OnCancle)
	ON_COMMAND(ID_32799, &CTaskManagerDlg::OnDormancy)
END_MESSAGE_MAP()



//计算CPU占用时间
double CTaskManagerDlg::FILETIME2Double(const _FILETIME& filetime)
{
	return double(filetime.dwHighDateTime*4.294967296e9) + double(filetime.dwLowDateTime);
}



// CTaskManagerDlg 消息处理程序

BOOL CTaskManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	::SetCursor(::LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1)));
	SendMessage(WM_SETCURSOR, (WPARAM)m_Cursor, NULL);
	//加载图标
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	UpLevel();		//提权

	//获取内存占用率
	//初始化内存信息的结构体对象
	MEMORYSTATUS memStatuas;
	//获取系统内存信息
	GlobalMemoryStatus(&memStatuas);
	DWORD Memory = memStatuas.dwMemoryLoad;


	//载入和初始化状态栏
	UINT buf[2] = {1,2};
	m_status.Create(this);

	//将状态栏分割 给每个窗格设置ID
	m_status.SetIndicators(buf,2);

	//设置指定的指示符窗格到新ID 样式 宽度
	m_status.SetPaneInfo(0, 1, 0, 320);	//给第0个窗格设置一个新ID为1 样式为0 宽度为100
	m_status.SetPaneInfo(1, 2, 0, 300);

	//格式化内存占用率和CPU占用率
	CString MemoryFootprint;	//内存占用率
	
	MemoryFootprint.Format(L"%s:%d", L"内存占用率", Memory);

	//设置内存占用率
	m_status.SetPaneText(0, MemoryFootprint);

	//重新定位和调整状态栏大小 同时显示状态栏 不调整默认为0
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	//设置定时器 定时刷新状态栏 1非零计时器标识符 2 时间间隔 3 回调函数地址
	SetTimer(1, 1000, NULL);
	SetTimer(2, 500, NULL);

	//初始化选项卡相关信息
	TCHAR Name[7][7] = { L"进程", L"窗口", L"垃圾清理", L"文件查看", L"PE工具", L"服务", L"病毒查杀" };
	for (int i = 0; i < 7;i++)
	{
		m_MyTable.InsertItem(i, Name[i]);
	}

	//给子窗口指针赋值

	m_MyTable.m_Dia[0] = new CDia1();
	m_MyTable.m_Dia[1] = new CDia2();
	m_MyTable.m_Dia[2] = new CDia3();
	m_MyTable.m_Dia[3] = new CDia4();
	m_MyTable.m_Dia[4] = new CDia5();
	m_MyTable.m_Dia[5] = new CDia6();
	m_MyTable.m_Dia[6] = new CDia7();

	//创建子窗口
	UINT DiaIds[7] = { IDD_DIALOG1, IDD_DIALOG2, IDD_DIALOG3, IDD_DIALOG4, IDD_DIALOG5, IDD_DIALOG6, IDD_DIALOG7 };
	for (int i = 0; i < 7; i++)
	{
		m_MyTable.m_Dia[i]->Create(DiaIds[i], &m_MyTable);
	}


	//控制两个窗口的大小
	CRect rc;
	m_MyTable.GetClientRect(rc);
	rc.DeflateRect(2, 23, 2, 2);
	for (int i = 0; i < 7; i++)
	{
		m_MyTable.m_Dia[i]->MoveWindow(rc);
	}
	
	//显示第一个子窗口
	m_MyTable.m_Dia[0]->ShowWindow(SW_SHOW);
	for (int i = 1; i < 7; i++)
	{
		m_MyTable.m_Dia[i]->ShowWindow(SW_HIDE);
	}

	//注册全局热键
	:: RegisterHotKey(this->GetSafeHwnd(),0xa001,MOD_CONTROL,'K');

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTaskManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTaskManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTaskManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//响应退出程序按钮的消息
void CTaskManagerDlg::OnBnClickedButton1()
{
	if (IDYES == MessageBox(L"是否要退出？", L"退出", MB_YESNO))
		CTaskManagerDlg::OnOK();	//一个默认的退出按钮
}


void CTaskManagerDlg::OnClose()
{
	//if (IDYES == MessageBox(L"是否要退出？", L"退出", MB_YESNO))
	CDialogEx::OnClose();
}


//定时器响应消息 定时刷新状态栏
void CTaskManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		//刷新内存占用率
		MEMORYSTATUS memStatuas;
		//获取系统内存信息
		GlobalMemoryStatus(&memStatuas);

		DWORD Memory = memStatuas.dwMemoryLoad;

		CString MemoryFootprint;	//内存占用率
		MemoryFootprint.Format(L"%s:%d", L"内存占用率", Memory);
		m_status.SetPaneText(0, MemoryFootprint);
	}
	else if (nIDEvent == 2)
	{
		//计算CPU占用率 在定时器里做 就不需要等待 也不需要另外开一个线程
		//1 空闲时间 2 内核时间 3 用户时间
		static _FILETIME idleTime = { 0 }, kernelTime = { 0 }, userTime = { 0 };

		//获取新的时间
		_FILETIME newIdleTime, newKernelTime, newUserTime;
		GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

		//将各个时间转换
		double dOldIdleTime = FILETIME2Double(idleTime);
		double dNewIdleTime = FILETIME2Double(newIdleTime);
		double dOldKernelTime = FILETIME2Double(kernelTime);
		double dNewKernelTime = FILETIME2Double(newKernelTime);
		double dOldUserTime = FILETIME2Double(userTime);
		double dNewUserTime = FILETIME2Double(newUserTime);

		//计算出使用率
		int time = (int)(100.0 - (dNewIdleTime - dOldIdleTime) /
			(dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);

		idleTime = newIdleTime;
		kernelTime = newKernelTime;
		userTime = newUserTime;

		CString CpuFootprint;
		CpuFootprint.Format(L"%s:%d%%", L"CPU占用率", time);

		//设置CPU占用率
		m_status.SetPaneText(1, CpuFootprint);
	}
}


//内存清理
void CTaskManagerDlg::OnBnClickedButton5()
{
	UpdateData(FALSE);
	//获取清理前的内存信息
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//开始清理内存
	ClearMemory();
	//获取清理后的内存信息
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//输出清理信息
	CString Contect;
	Contect.Format(_T("内存清理完毕！本次清理 : %.2f MB"), preUsedMem - afterUsedMem);
	MessageBox(Contect);
}

//清理内存
void CTaskManagerDlg::ClearMemory()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);
	int count = 0;

	//获取系统中第一个进程的信息  
	BOOL Status = Process32First(hProcessSnap, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			//设置进程工作区大小
			SetProcessWorkingSetSize(hProcess, -1, -1);
			//尽可能多的将指定进程的页面从工作区移除
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		//获取下一个进程的信息  
		Status = Process32Next(hProcessSnap, &ProcessInfo);
	}
}

BOOL CTaskManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	//判断是全局热键同时也是当前注册的键
	if (pMsg->message==WM_HOTKEY&&pMsg->wParam==0xa001)
	{
		//隐藏窗口代码
		if (m_IsWindowHide==TRUE)
		{
			ShowWindow(SW_HIDE);
			m_IsWindowHide = FALSE;
		}
		//显示窗口代码
		else
		{
			ShowWindow(SW_SHOW);
			m_IsWindowHide = TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//锁屏
void CTaskManagerDlg::OnLockScreen()
{
	LockWorkStation();
}

//关机
void CTaskManagerDlg::OnShutDown()
{
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE,NULL);
}

//重启
void CTaskManagerDlg::OnRestart()
{
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, NULL);
}

//注销
void CTaskManagerDlg::OnCancle()
{
	ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, NULL);
}

//休眠
void CTaskManagerDlg::OnDormancy()
{
	SetSuspendState(TRUE, FALSE, FALSE);
}

//提权
void CTaskManagerDlg::UpLevel()
{
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}


