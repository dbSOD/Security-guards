// Dia2.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia2.h"
#include "afxdialogex.h"
#include "StartingItems.h"
#include "SoftManage.h"

CListCtrl CDia2::m_ListCtrl = {};
// CDia2 对话框

IMPLEMENT_DYNAMIC(CDia2, CDialogEx)

CDia2::CDia2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia2::IDD, pParent)
{

}

CDia2::~CDia2()
{
}

void CDia2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDia2, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDia2::OnRclickList1)
	ON_COMMAND(ID_32787, &CDia2::OnReFresh)
	ON_BN_CLICKED(IDC_BUTTON1, &CDia2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON8, &CDia2::OnBnClickedButton8)
END_MESSAGE_MAP()


// CDia2 消息处理程序

//枚举窗口的回调函数
BOOL CALLBACK CDia2::EnumWinProc(HWND hWnd, LPARAM lParam)
{
	//根据窗口句柄获取窗口名      
	TCHAR WindowsName[200];                                           
	::GetWindowText(hWnd, WindowsName, 200);
	//判断窗口是否被隐藏
	if (::IsWindowVisible(hWnd)==TRUE&&wcslen(WindowsName)!=0)
	{
		//窗口没有被隐藏且窗口标题长度不为0 则将窗口信息显示到页面
		//添加行
		static int i = 0;
		m_ListCtrl.InsertItem(i, WindowsName);
	}
	return TRUE;
}


BOOL CDia2::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//插入列
	m_ListCtrl.InsertColumn(0, L"窗口名", 0, 200);
	m_ListCtrl.InsertColumn(1, L"窗口句柄", 0, 200);
	//枚举窗口
	EnumWindows(&EnumWinProc, NULL);
	//设置扩展属性
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDia2::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	CMenu m_Menu, *tMenu;
	m_Menu.LoadMenu(IDR_MENU2);

	//拿到第0个下拉菜单(菜单可能有很多列 这个函数是拿到第几列)
	tMenu = m_Menu.GetSubMenu(1);

	//获取鼠标位置
	CPoint pt;
	GetCursorPos(&pt);

	//弹出菜单
	TrackPopupMenu(tMenu->m_hMenu,
		TPM_LEFTALIGN,
		pt.x, pt.y,	//坐标
		0, m_hWnd   //句柄
		, 0);
	*pResult = 0;
}

//刷新窗口
void CDia2::OnReFresh()
{
	//删除全部
	m_ListCtrl.DeleteAllItems();
	//枚举窗口
	EnumWindows(&EnumWinProc, NULL);
}


//点击弹出软件管理
void CDia2::OnBnClickedButton1()
{
	CSoftManage* SoftDlg = new CSoftManage;
	SoftDlg->Create(IDD_Soft, this);
	SoftDlg->ShowWindow(SW_SHOW);
}

//点击弹出启动项窗口
void CDia2::OnBnClickedButton8()
{
	CStartingItems *StatDig = new CStartingItems;
	StatDig->Create(IDD_StartItem, this);
	StatDig->ShowWindow(SW_SHOW);
}
