// Section.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Section.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"


// CSection 对话框

IMPLEMENT_DYNAMIC(CSection, CDialogEx)

CSection::CSection(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSection::IDD, pParent)
{

}

CSection::~CSection()
{
}

void CSection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CSection, CDialogEx)
END_MESSAGE_MAP()


// CSection 消息处理程序


BOOL CSection::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//初始化List控件
	WCHAR* wcArr[] = {L"名称",L"VOffset",L"VSize",L"ROffset",L"RSize",L"标志"};
	DWORD len = sizeof(wcArr) / sizeof(wcArr[0]);
	for (DWORD i = 0; i < len; i++)
	{
		m_ListCtrl.InsertColumn(i, wcArr[i], 0, 85);
	}
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//找到区段头表
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew+CDia5::g_lpBase);
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	//循环输出区段表的信息
	DWORD index = 0;	//行号
	CString Contect;	//内容
	WORD wCount = pNt->FileHeader.NumberOfSections;	//循环次数 用文件头的区块数量
	for (WORD i = 0; i < wCount; i++)
	{
		//区段名
		m_ListCtrl.InsertItem(index, (CString)pSection->Name);
		//起始的相对虚拟地址
		Contect.Format(L"%08X", pSection->VirtualAddress);
		m_ListCtrl.SetItemText(index, 1, Contect);
		//区段大小(加载后)
		Contect.Format(L"%08X", pSection->Misc.VirtualSize);
		m_ListCtrl.SetItemText(index, 2, Contect);
		//文件偏移
		Contect.Format(L"%08X", pSection->PointerToRawData);
		m_ListCtrl.SetItemText(index, 3, Contect);
		//区段大小(文件中)
		Contect.Format(L"%08X", pSection->SizeOfRawData);
		m_ListCtrl.SetItemText(index, 4, Contect);
		//区段的属性
		Contect.Format(L"%08X", pSection->Characteristics);
		m_ListCtrl.SetItemText(index++, 5, Contect);
		//下一个区块
		pSection++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
