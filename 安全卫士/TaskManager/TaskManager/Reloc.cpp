// Reloc.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Reloc.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"


// CReloc 对话框

IMPLEMENT_DYNAMIC(CReloc, CDialogEx)

CReloc::CReloc(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReloc::IDD, pParent)
{

}

CReloc::~CReloc()
{
}

void CReloc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrlUp);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrlDown);
}


BEGIN_MESSAGE_MAP(CReloc, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CReloc::OnClickList1)
END_MESSAGE_MAP()


// CReloc 消息处理程序


BOOL CReloc::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//初始化ListCtrl
	WCHAR *wcArr1[] = { L"索引", L"区段", L"RVA", L"项目",L" " };
	WCHAR *wcArr2[] = { L"索引", L"RVA", L"偏移", L"类型",L"FAR地址"};
	for (int i = 0; i < 5;i++)
	{
		m_ListCtrlUp.InsertColumn(i, wcArr1[i], 0, 100);
		m_ListCtrlDown.InsertColumn(i, wcArr2[i], 0, 100);
	}
	m_ListCtrlUp.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListCtrlDown.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	typedef struct _Offset
	{
		WORD Offset : 12;
		WORD Type : 4;
	}Offset;
	//找到重定位表
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew+CDia5::g_lpBase);
	//找到区段表头地址
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[5];

	DWORD  dwRelocFOA = CAddressChange::RVAtoFOA(pDir->VirtualAddress);
	
	PIMAGE_BASE_RELOCATION  pReloc = (PIMAGE_BASE_RELOCATION)(dwRelocFOA + CDia5::g_lpBase);

	DWORD dwSectionCount = pNt->FileHeader.NumberOfSections;  //获取区段个数
	//遍历
	DWORD index = 0;	//行号
	CString Contect;	//内容
	while (pReloc->SizeOfBlock)
	{
		//显示数据到控件
		//索引
		Contect.Format(L"%d", index+1);
		m_ListCtrlUp.InsertItem(index, Contect);
		//区段名
		// 判断在哪个区段
		for (DWORD i = 0; i < dwSectionCount; i++)
		{
			if (pReloc->VirtualAddress >= pSection->VirtualAddress &&
				pReloc->VirtualAddress < pSection->VirtualAddress + pSection->SizeOfRawData)
			{
				m_ListCtrlUp.SetItemText(index, 1, (CString)pSection->Name);
				break;
			}
			pSection++;
		}
		
		//RVA
		Contect.Format(L"%08X", pReloc->VirtualAddress);
		m_ListCtrlUp.SetItemText(index, 2, Contect);
		//重定位项个数
		DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
		CString hCount;		//十六进制
		CString dCount;		//十进制
		hCount.Format(L"%X", dwCount);
		dCount.Format(L"%d", dwCount);
		Contect.Format(L"%sh/%sd", hCount, dCount);
		m_ListCtrlUp.SetItemText(index++, 3, Contect);
		
		//下一个重定位块
		pReloc = (PIMAGE_BASE_RELOCATION)(pReloc->SizeOfBlock+(DWORD)pReloc);
		//下一个区段
		
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//响应List的点击消息
void CReloc::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_ListCtrlDown.DeleteAllItems();
	typedef struct _Offset
	{
		WORD Offset : 12;
		WORD Type : 4;
	}Offset;
	//找到重定位表
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CDia5::g_lpBase);
	//找到区段表头地址

	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[5];

	DWORD  dwRelocFOA = CAddressChange::RVAtoFOA(pDir->VirtualAddress);

	PIMAGE_BASE_RELOCATION  pReloc = (PIMAGE_BASE_RELOCATION)(dwRelocFOA + CDia5::g_lpBase);
	//遍历
	DWORD index = 0;	//行号
	CString Contect;	//内容
	//重定位项个数
	DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
	while (pReloc->SizeOfBlock)
	{
		
		//重定位项起始位置
		Offset* pOffset = (Offset*)(pReloc + 1);

		//遍历需要重定位的每个数据
		for (DWORD i = 0; i < dwCount; i++)
		{
			if (pOffset->Type==3)
			{
				//算出需要重定位项数据的FOA
				DWORD dwDataFOA = CAddressChange::RVAtoFOA(pOffset->Offset + pReloc->VirtualAddress);
				//需要重定位的数据
				DWORD* pData = (DWORD*)(dwDataFOA+CDia5::g_lpBase);
				//显示需要重定位的数据*pData
				//显示序号
				Contect.Format(L"%d", index + 1);
				m_ListCtrlDown.InsertItem(index, Contect);
				//显示RVA
				Contect.Format(L"%08X",pOffset->Offset + pReloc->VirtualAddress);
				m_ListCtrlDown.SetItemText(index, 1, Contect);
				//显示OFA
				Contect.Format(L"%08X", dwDataFOA);
				m_ListCtrlDown.SetItemText(index, 2, Contect);
				//显示Type
				Contect.Format(L"%s(%d)", L"HIGHLOW",pOffset->Type);
				m_ListCtrlDown.SetItemText(index, 3, Contect);
				//显示VA
				Contect.Format(L"%08X", *pData);
				m_ListCtrlDown.SetItemText(index++, 4, Contect);
			}
			//下一个重定位项
			pOffset++;
		}
		//下一个重定位块
		pReloc = (PIMAGE_BASE_RELOCATION)(pReloc->SizeOfBlock + (DWORD)pReloc);
	}
	*pResult = 0;
}
