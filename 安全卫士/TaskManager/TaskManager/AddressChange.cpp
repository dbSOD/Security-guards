// AddressChange.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "AddressChange.h"
#include "afxdialogex.h"
#include <windows.h>
#include "Dia5.h"


// CAddressChange 对话框

IMPLEMENT_DYNAMIC(CAddressChange, CDialogEx)

CAddressChange::CAddressChange(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddressChange::IDD, pParent)
	, m_VA(_T(""))
	, m_RVA(_T(""))
	, m_Offect(_T(""))
	, m_ExInfo(_T(""))
	, m_Section(_T(""))
	, m_Bytes(_T(""))
{

}

CAddressChange::~CAddressChange()
{
}

void CAddressChange::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_VA);
	DDX_Text(pDX, IDC_EDIT11, m_RVA);
	DDX_Text(pDX, IDC_EDIT2, m_Offect);
	DDX_Text(pDX, IDC_EDIT4, m_ExInfo);
	DDX_Text(pDX, IDC_EDIT3, m_Section);
	DDX_Text(pDX, IDC_EDIT5, m_Bytes);
}


BEGIN_MESSAGE_MAP(CAddressChange, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CAddressChange::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT3, &CAddressChange::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CAddressChange 消息处理程序

//地址转换函数
DWORD CAddressChange::RVAtoFOA(DWORD dwRVA)
{
	//落在哪个区段内
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + CDia5::g_lpBase);

	//找到区段表头地址
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	//获取区段个数
	DWORD dwCount = pNt->FileHeader.NumberOfSections;
	for (DWORD i = 0; i < dwCount; i++)
	{
		if (dwRVA >= pSection->VirtualAddress &&
			dwRVA < pSection->VirtualAddress + pSection->SizeOfRawData)
		{
			return dwRVA -
				pSection->VirtualAddress +
				pSection->PointerToRawData;
		}
		pSection++;
	}
	return 0;
}

//响应计算按钮进行地址转换 根据偏移量和VA算RVA
void CAddressChange::OnBnClickedButton2()
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + CDia5::g_lpBase);
	//找到区段表头地址
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);

	UpdateData(TRUE);
	//把CString转成十六进制
	DWORD nRVA = wcstol(m_RVA, NULL, 16);
	
	//转换地址
	DWORD dwFOA = RVAtoFOA(nRVA);
	DWORD dwVA = nRVA + pNt->OptionalHeader.ImageBase;

	//转换字符串格式
	CString sFOA;
	CString sVA;

	sFOA.Format(L"%08X", dwFOA);
	sVA.Format(L"%08X", dwVA);

	//显示到控件
	m_Offect = sFOA;
	m_VA = sVA;
	m_Section = pSection->Name;


	//更新数据
	UpdateData(FALSE);


}


void CAddressChange::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
