// Export.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Export.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"


// CExport 对话框

IMPLEMENT_DYNAMIC(CExport, CDialogEx)

CExport::CExport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExport::IDD, pParent)
	, m_Name(_T(""))
	, m_Offset(_T(""))
	, m_Charact(_T(""))
	, m_Base(_T(""))
	, m_NameSrtring(_T(""))
	, m_NumberOfFun(_T(""))
	, m_NumberOfName(_T(""))
	, m_AddressOfFun(_T(""))
	, m_AddressOfName(_T(""))
	, m_AddressOfOrdinal(_T(""))
{

}

CExport::~CExport()
{
}

void CExport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT17, m_Name);
	DDX_Text(pDX, IDC_EDIT1, m_Offset);
	DDX_Text(pDX, IDC_EDIT15, m_Charact);
	DDX_Text(pDX, IDC_EDIT16, m_Base);
	DDX_Text(pDX, IDC_EDIT11, m_NameSrtring);
	DDX_Text(pDX, IDC_EDIT2, m_NumberOfFun);
	DDX_Text(pDX, IDC_EDIT18, m_NumberOfName);
	DDX_Text(pDX, IDC_EDIT19, m_AddressOfFun);
	DDX_Text(pDX, IDC_EDIT20, m_AddressOfName);
	DDX_Text(pDX, IDC_EDIT21, m_AddressOfOrdinal);
	DDX_Control(pDX, IDC_LIST4, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CExport, CDialogEx)
END_MESSAGE_MAP()


// CExport 消息处理程序


BOOL CExport::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//找到导出表
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CDia5::g_lpBase);
	PIMAGE_DATA_DIRECTORY pExport = &pNt->OptionalHeader.DataDirectory[0];

	DWORD dwExportFOA = CAddressChange::RVAtoFOA(pExport->VirtualAddress);

	//找到导出表在文件中的位置
	PIMAGE_EXPORT_DIRECTORY pExportTable = (PIMAGE_EXPORT_DIRECTORY)(dwExportFOA + CDia5::g_lpBase);

	//显示输出表偏移
	CString cFOA;
	cFOA.Format(L"%08X", dwExportFOA);
	m_Offset = cFOA;
	
	//显示特征值
	CString sCharact;
	sCharact.Format(L"%08X", pExportTable->Characteristics);
	m_Charact = sCharact;

	//显示基址
	CString cBase;
	cBase.Format(L"%08X", pExportTable->Base);
	m_Base = cBase;

	//显示名称
	CString sName;
	sName.Format(L"%08X", pExportTable->Name);
	m_Name = sName;

	//显示名称字串
	m_NameSrtring = (CAddressChange::RVAtoFOA(pExportTable->Name)) + CDia5::g_lpBase;

	//显示函数数量
	CString sNumberOfFun;
	sNumberOfFun.Format(L"%08X", pExportTable->NumberOfFunctions);
	m_NumberOfFun = sNumberOfFun;

	//显示函数名称数量
	CString sNumberOfName;
	sNumberOfName.Format(L"%08X", pExportTable->NumberOfNames);
	m_NumberOfName = sNumberOfName;

	//显示函数地址表RVA
	CString sAddressOfFun;
	sAddressOfFun.Format(L"%08X", pExportTable->AddressOfFunctions);
	m_AddressOfFun = sAddressOfFun;

	//显示函数名称表RVA
	CString sAddressOfName;
	sAddressOfName.Format(L"%08X", pExportTable->AddressOfNames);
	m_AddressOfName = sAddressOfName;

	//显示函数序号表RVA
	CString sAddressOfOrdinal;
	sAddressOfOrdinal.Format(L"%08X", pExportTable->AddressOfNameOrdinals);
	m_AddressOfOrdinal = sAddressOfOrdinal;

	//显示ListCtrl
	//添加列
	m_ListCtrl.InsertColumn(0, L"序号", 0, 100);
	m_ListCtrl.InsertColumn(1, L"RVA", 1, 100);
	m_ListCtrl.InsertColumn(2, L"偏移", 2, 100);
	m_ListCtrl.InsertColumn(3, L"函数名", 3, 100);

	DWORD dwFunCount = pExportTable->NumberOfFunctions;		//函数的数量
	DWORD dwOrdinalCount = pExportTable->NumberOfNames;		//函数名字和序号的数量

	//找到地址表
	DWORD* pFunAddr = (DWORD*)(CAddressChange::RVAtoFOA(pExportTable->AddressOfFunctions) + CDia5::g_lpBase);
	//找到名称表
	DWORD* pNameAddr = (DWORD*)(CAddressChange::RVAtoFOA(pExportTable->AddressOfNames) + CDia5::g_lpBase);
	//找到序号表
	WORD* pOrdinalAddr = (WORD*)(CAddressChange::RVAtoFOA(pExportTable->AddressOfNameOrdinals)+CDia5::g_lpBase);

	//开始遍历导出表
	DWORD dwListIndex = 0;
	for (DWORD i = 0; i < dwFunCount; i++)
	{
		//如果是0说明地址无效 直接跳过
		if (pFunAddr[i]==0)
		{
			continue;
		}
		//遍历序号表中是否有此序号 如果有说明有此函数有名字
		BOOL bFlag = FALSE;

		//显示到控件上
		CString sNameRVA;	//RVA
		sNameRVA.Format(L"%08X", pFunAddr[i]);
		CString index;		//序号
		index.Format(L"%08X", i + pExportTable->Base);
		CString sFOA;	//FOA
		sFOA.Format(L"%08X", CAddressChange::RVAtoFOA(pFunAddr[i]));
		m_ListCtrl.InsertItem(dwListIndex, index);	//序号
		m_ListCtrl.SetItemText(dwListIndex, 1, sNameRVA);	//RVA
		m_ListCtrl.SetItemText(dwListIndex, 2, sFOA);	//FOA

		for (DWORD j = 0; j < dwOrdinalCount; j++)
		{
			//相等说明函数有名字
			if (i == pOrdinalAddr[j])
			{
				bFlag = TRUE;
				DWORD dwNameRVA = pNameAddr[j];
				CString sName;	//函数名
				sName.Format(L"%s", CString(CAddressChange::RVAtoFOA(dwNameRVA) + CDia5::g_lpBase));
				m_ListCtrl.SetItemText(dwListIndex++, 3, sName);	//函数名	
			}
		}
		//如果序号表中没有 说明此函数只有序号没有名字
		if (!bFlag)
		{
			m_ListCtrl.SetItemText(dwListIndex++, 3, L"NULL");	//名字
		}
	}

	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
