// Dia1.cpp : 实现文件
//

#include "stdafx.h"

#include "Dia1.h"
#include "DiaPile.h"
#include "afxdialogex.h"
#include "DiaThread.h"
#include "DiaModule.h"

CString CDia1::m_ProcessId(" ");
// CDia1 对话框

IMPLEMENT_DYNAMIC(CDia1, CDialogEx)

CDia1::CDia1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia1::IDD, pParent)
{

}

CDia1::~CDia1()
{
}

void CDia1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDia1, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDia1::OnRclickList1)
	ON_COMMAND(ID_32783, &CDia1::OnRefresh)
	ON_COMMAND(ID_32781, &CDia1::OnEixt)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDia1::OnClickList1)
	ON_COMMAND(ID_32784, &CDia1::OnThreadList)
	ON_COMMAND(ID_32786, &CDia1::OnLookModule)
	ON_COMMAND(ID_32795, &CDia1::OnLookPile)
END_MESSAGE_MAP()



// CDia1 消息处理程序


BOOL CDia1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HCURSOR m_Cursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1));
	SendMessage(WM_SETCURSOR, (WPARAM)m_Cursor, NULL);
	//添加图标
	//创建一个成员为32*32大小使用32位色的icon数组 初始空间为15 满了之后递增1
	ImageList.Create(32, 32, ILC_COLOR32, 15, 1);
	//拿到图标句柄
	HICON Ico2 = AfxGetApp()->LoadIcon(IDI_ICON2);
	HICON Ico3 = AfxGetApp()->LoadIcon(IDI_ICON3);
	HICON Ico4 = AfxGetApp()->LoadIcon(IDI_ICON4);
	HICON Ico5 = AfxGetApp()->LoadIcon(IDI_ICON5);
	HICON Ico6 = AfxGetApp()->LoadIcon(IDI_ICON6);
	HICON Ico7 = AfxGetApp()->LoadIcon(IDI_ICON7);
	HICON Ico8 = AfxGetApp()->LoadIcon(IDI_ICON8);
	HICON Ico9 = AfxGetApp()->LoadIcon(IDI_ICON9);
	HICON Ico10 = AfxGetApp()->LoadIcon(IDI_ICON10);
	HICON Ico11 = AfxGetApp()->LoadIcon(IDI_ICON11);
	HICON Ico12 = AfxGetApp()->LoadIcon(IDI_ICON12);
	HICON Ico13 = AfxGetApp()->LoadIcon(IDI_ICON13);
	HICON Ico14 = AfxGetApp()->LoadIcon(IDI_ICON14);
	HICON Ico15 = AfxGetApp()->LoadIcon(IDI_ICON15);
	HICON Ico16 = AfxGetApp()->LoadIcon(IDI_ICON16);
	//添加图标
	HICON IconArr[15] = { Ico2, Ico3, Ico4, Ico5, Ico6, Ico7, Ico8, Ico9, Ico10, Ico11, Ico12
		, Ico13, Ico14, Ico15, Ico16 };
	for (int i = 0; i < 15; i++)
	{
		ImageList.Add(IconArr[i]);
	}

	//将CImageList关联到List控件上
	m_ListCtrl.SetImageList(&ImageList, LVSIL_SMALL);                            

	//1 给列表添加列
	//第三个参数是对齐方式 可以设置LVCFMT_RIGHT LVCFMT_LEFT LVCFMT_CENTER
	m_ListCtrl.InsertColumn(0, L"进程名", 0, 200);    
	m_ListCtrl.InsertColumn(1, L"ID", 0, 100);
	m_ListCtrl.InsertColumn(2, L"优先级", 0, 100);
	m_ListCtrl.InsertColumn(3, L"父进程ID", 0, 100);
	m_ListCtrl.InsertColumn(4, L"持有线程数", 0, 100);

	//遍历进程
	ProcessList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}



//遍历进程并刷新到列表
void CDia1::ProcessList()
{
	//包含进程信息的结构体 这个结构体长度在使用前必须设置
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	//创建一个快照 参数一 要获取的快照类型  参数2 当获取模块 堆信息时 需要指定一个进程ID 所以此处无效
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//找到第一个进程的信息 参数1 快照句柄 参数2 进程信息
	if (Process32First(hSnap, &pe) == TRUE)
	{
		//循环遍历其他进程
		do
		{
			int i = 0;
			CString ProcessID;	//进程ID
			CString Priority;	//进程优先级
			CString FatherId;	//父进程ID
			CString ProcessNum; //持有线程数
			//把ID和优先级转成字符串
			ProcessID.Format(L"%d", pe.th32ProcessID);
			Priority.Format(L"%d", pe.pcPriClassBase);
			FatherId.Format(L"%d", pe.th32ParentProcessID);
			ProcessNum.Format(L"%d", pe.cntThreads);

			//把进程名添加到行
			m_ListCtrl.InsertItem(i, pe.szExeFile, rand() % 15);
			//把ID添加到行 最后一个参数是图标索引
			m_ListCtrl.SetItemText(i, 1, ProcessID);
			//把优先级添加进表格内容
			m_ListCtrl.SetItemText(i, 2, Priority);
			//把父进程ID添加进去
			m_ListCtrl.SetItemText(i, 3, FatherId);
			//持有线程数
			m_ListCtrl.SetItemText(i, 4, ProcessNum);


			i++;
		} while (Process32Next(hSnap, &pe));
	}
	//设置扩展属性
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

//响应右键点击消息
void CDia1::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//拿到当前行的进程ID存到m_ProcessId里面
	m_ProcessId = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 1);

	// TODO:  在此添加控件通知处理程序代码
	CMenu m_Menu,*tMenu;
	m_Menu.LoadMenu(IDR_MENU2);

	//拿到第0个下拉菜单(菜单可能有很多列 这个函数是拿到第几列)
	tMenu = m_Menu.GetSubMenu(0);

	//获取鼠标位置
	CPoint pt;
	GetCursorPos(&pt);

	//弹出菜单
	TrackPopupMenu(tMenu->m_hMenu,
		TPM_LEFTALIGN,
		pt.x, pt.y,	//坐标
		0,m_hWnd   //句柄
		,0);
	*pResult = 0;
}


//刷新进程
void CDia1::OnRefresh()
{
	// TODO:  在此添加命令处理程序代码
	//删除全部菜单
	m_ListCtrl.DeleteAllItems();
	//再把进程刷新出来
	ProcessList();
}

//退出进程
void CDia1::OnEixt()
{
	//打开进程拿到进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _ttoi(m_ProcessId));
	//结束进程
	TerminateProcess(hProcess, 0);
}

//左键点击List控件时响应的消息
void CDia1::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//拿到进程ID存起来
	m_ProcessId = m_ListCtrl.GetItemText(pNMItemActivate->iItem,1);

	*pResult = 0;
}


//弹出查看的线程窗口
void CDia1::OnThreadList()
{
	//弹出线程窗口
	CDiaThread m_DiaThread;
	m_DiaThread.DoModal();
	
}

//查看模块
void CDia1::OnLookModule()
{
	//这里不能用局部对象 否则窗口会一闪而过 因为出了这个函数 类对象就被释放了
	CDiaModule *DiaModule=new CDiaModule;
	//1窗口ID 2 this指针
	DiaModule->Create(IDD_DIALOG9, this);
	DiaModule->ShowWindow(SW_SHOW);
}

//查看堆
void CDia1::OnLookPile()
{
	CDiaPile *DiaPile = new CDiaPile;
	DiaPile->Create(IDD_DIALOG10, this);
	DiaPile->ShowWindow(SW_SHOW);

}
