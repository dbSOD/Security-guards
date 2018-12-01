// DiaPile.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "DiaPile.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "Dia1.h"


// CDiaPile 对话框

IMPLEMENT_DYNAMIC(CDiaPile, CDialogEx)

CDiaPile::CDiaPile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaPile::IDD, pParent)
{

}

CDiaPile::~CDiaPile()
{
}

void CDiaPile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDiaPile, CDialogEx)
END_MESSAGE_MAP()


// CDiaPile 消息处理程序

//遍历堆
BOOL CDiaPile::PileList(DWORD dwPid)
{
	//创建线程堆快照
	HANDLE hPileSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwPid);

	if (hPileSnap==INVALID_HANDLE_VALUE)
	{
		MessageBox(L"权限不足 无法查看堆！！！");
		return FALSE;
	}
	//初始化堆信息结构体
	HEAPLIST32 hl32 = { sizeof(HEAPLIST32) };
	int i = 0;	//行号
	if (Heap32ListFirst(hPileSnap, &hl32))
	{
		do 
		{
			HEAPENTRY32 he32 = { sizeof(HEAPENTRY32) };
			if (Heap32First(&he32, dwPid,hl32.th32HeapID))
			{
				do 
				{
					//在这里插入数据
					CString PileId;		 //堆ID
					CString PileSize;	 //堆大小
					CString PileAddress; //起始地址
					CString PileStatus;	 //状态
					//格式化
					PileId.Format(L"%d", he32.th32HeapID);
					PileSize.Format(L"%d", he32.dwBlockSize);
					PileAddress.Format(L"%d", he32.dwAddress);
					PileStatus.Format(L"%d", he32.dwFlags);
					//插入数据
					m_ListCtrl.InsertItem(i, PileId);
					m_ListCtrl.SetItemText(i, 1, PileSize);
					m_ListCtrl.SetItemText(i, 2, PileAddress);
					m_ListCtrl.SetItemText(i, 3, PileStatus);
				} while (Heap32Next(&he32));
			}
		} while (Heap32ListNext(hPileSnap,&hl32));
	}
	CloseHandle(hPileSnap);
	return TRUE;
}


BOOL CDiaPile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListCtrl.InsertColumn(0, L"ID", 0, 100);
	m_ListCtrl.InsertColumn(1, L"大小", 0, 100);
	m_ListCtrl.InsertColumn(2, L"起始地址", 0, 100);
	m_ListCtrl.InsertColumn(3, L"状态", 0, 100);

	//遍历堆并显示到列表
	PileList(_ttoi(CDia1::m_ProcessId));

	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
