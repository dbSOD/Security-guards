// DiaThread.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "DiaThread.h"
#include "afxdialogex.h"
#include "Dia1.h"

CString CDiaThread::m_ThreadId(" ");
// CDiaThread 对话框

IMPLEMENT_DYNAMIC(CDiaThread, CDialogEx)

CDiaThread::CDiaThread(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaThread::IDD, pParent)
{

}

CDiaThread::~CDiaThread()
{
}

void CDiaThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDiaThread, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDiaThread::OnRclickList1)
	ON_COMMAND(ID_32792, &CDiaThread::OnStopThread)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDiaThread::OnClickList1)
	ON_COMMAND(ID_32791, &CDiaThread::OnReFreshThread)
	ON_COMMAND(ID_32793, &CDiaThread::OnReStartThread)
	ON_COMMAND(ID_32794, &CDiaThread::OnKillThread)
END_MESSAGE_MAP()


// CDiaThread 消息处理程序

void CDiaThread::ThreadsList(DWORD dwPid)	//遍历线程
{

	//初始化线程信息的结构体
	THREADENTRY32 te32;
	//创建当前所有线程的快照
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	//设置线程输入参数 结构体大小 这里必须初始化 
	te32.dwSize = sizeof(THREADENTRY32);
	//开始获取第一个线程信息
	Thread32First(hThreadSnap, &te32);
	//循环遍历所有的线程信息
	do 
	{
		//在这里对线程进行过滤
		if (te32.th32OwnerProcessID == dwPid)
		{
			//再将线程的信息显示到界面

			CString ThreadId;	//线程ID
			CString Priority;	//线程优先级
			CString FatherId;	//父进程ID

			//格式转换
			ThreadId.Format(L"%d", te32.th32ThreadID);
			Priority.Format(L"%d", te32.tpBasePri);
			FatherId.Format(L"%d", te32.th32OwnerProcessID);

			int i = 0;
			//添加进ListView
			m_ListCtrl.InsertItem(i, ThreadId);
			m_ListCtrl.SetItemText(i, 1, Priority);
			m_ListCtrl.SetItemText(i, 2, FatherId);
			i++;

		}
	} while (Thread32Next(hThreadSnap,&te32));
}


BOOL CDiaThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//插入列
	m_ListCtrl.InsertColumn(0, L"线程ID", 0, 80);
	m_ListCtrl.InsertColumn(1, L"优先级", 0, 80);
	m_ListCtrl.InsertColumn(2, L"父进程ID", 0, 80);

	//拿到被选中行的进程ID 然后再弹出线程窗口
	DWORD ProcessId = _ttoi(CDia1::m_ProcessId);
	ThreadsList(ProcessId);

	//设置扩展属性
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//鼠标右键点击时在线程窗口弹出菜单
void CDiaThread::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//保存当前选中行的ID 
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_ThreadId = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	//实例化菜单对象
	CMenu m_Menu, *tMenu;
	//加载菜单
	m_Menu.LoadMenu(IDR_MENU2);
	//拿到要用的第3个菜单 下标从0开始
	tMenu = m_Menu.GetSubMenu(2);
	//获取鼠标位置 让弹出的菜单跟随鼠标
	CPoint pt;
	GetCursorPos(&pt);
	//弹出菜单 参数一菜单句柄 参数二屏幕坐标属性 参数三四 屏幕坐标 参数六窗口句柄 
	TrackPopupMenu(tMenu->m_hMenu,TPM_LEFTALIGN,pt.x,pt.y,0,m_hWnd,0);

	*pResult = 0;
}

//挂起线程
void CDiaThread::OnStopThread()
{
	//拿到当前线程的句柄
	HANDLE hThread =  OpenThread(PROCESS_ALL_ACCESS,FALSE,_ttoi(m_ThreadId));

	//暂停线程 需要句柄
	SuspendThread(hThread);
}

//响应鼠标左键点击
void CDiaThread::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//不管是左键点击还是右键点击都拿到线程ID
	m_ThreadId = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

//刷新线程
void CDiaThread::OnReFreshThread()
{
	//先删掉所有项 再重新遍历
	m_ListCtrl.DeleteAllItems();
	ThreadsList(_ttoi(CDia1::m_ProcessId));
}

//恢复线程
void CDiaThread::OnReStartThread()
{
	//拿到当前线程的句柄
	HANDLE hThread = OpenThread(PROCESS_ALL_ACCESS, FALSE, _ttoi(m_ThreadId));
	//恢复线程 需要句柄
	ResumeThread(hThread);
}

//结束线程
void CDiaThread::OnKillThread()
{
	//拿到句柄
	HANDLE hThread = OpenThread(PROCESS_ALL_ACCESS, FALSE, _ttoi(m_ThreadId));
	//结束线程
	TerminateThread(hThread,-1);
}
