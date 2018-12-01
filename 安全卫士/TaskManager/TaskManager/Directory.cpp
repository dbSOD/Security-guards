// Directory.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Directory.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"
#include "Export.h"
#include "Import.h"
#include "Resoure.h"
#include "Reloc.h"
#include "Tls.h"
#include "Delay.h"

// CDirectory 对话框

IMPLEMENT_DYNAMIC(CDirectory, CDialogEx)

CDirectory::CDirectory(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDirectory::IDD, pParent)
	, m_ExportRVA(_T(""))
	, m_ExportSize(_T(""))
	, m_ImportRVA(_T(""))
	, m_ImportSize(_T(""))
	, m_ResourceRVA(_T(""))
	, m_ResourceSize(_T(""))
	, m_RelocRVA(_T(""))
	, m_RelocSize(_T(""))
	, m_TLSRVA(_T(""))
	, m_TLSSize(_T(""))
	, m_DelayRVA(_T(""))
	, m_DelaySize(_T(""))
{

}

CDirectory::~CDirectory()
{
}

void CDirectory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_ExportRVA);
	DDX_Text(pDX, IDC_EDIT3, m_ExportSize);
	DDX_Text(pDX, IDC_EDIT4, m_ImportRVA);
	DDX_Text(pDX, IDC_EDIT9, m_ImportSize);
	DDX_Text(pDX, IDC_EDIT6, m_ResourceRVA);
	DDX_Text(pDX, IDC_EDIT12, m_ResourceSize);
	DDX_Text(pDX, IDC_EDIT5, m_RelocRVA);
	DDX_Text(pDX, IDC_EDIT10, m_RelocSize);
	DDX_Text(pDX, IDC_EDIT8, m_TLSRVA);
	DDX_Text(pDX, IDC_EDIT13, m_TLSSize);
	DDX_Text(pDX, IDC_EDIT7, m_DelayRVA);
	DDX_Text(pDX, IDC_EDIT14, m_DelaySize);
}


BEGIN_MESSAGE_MAP(CDirectory, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDirectory::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDirectory::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON8, &CDirectory::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CDirectory::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON5, &CDirectory::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON10, &CDirectory::OnBnClickedButton10)
END_MESSAGE_MAP()


// CDirectory 消息处理程序


BOOL CDirectory::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CDia5::g_lpBase);
	//找到导出表
	PIMAGE_DATA_DIRECTORY pExport = &pNt->OptionalHeader.DataDirectory[0];

	//显示导出表的信息到GUI
	CString sExportRVA;
	CString sExportSize;
	sExportRVA.Format(L"%08X", pExport->VirtualAddress);
	sExportSize.Format(L"%08X", pExport->Size);
	m_ExportRVA = sExportRVA;
	m_ExportSize = sExportSize;

	//找到导入表
	PIMAGE_DATA_DIRECTORY pImport = &pNt->OptionalHeader.DataDirectory[1];
	//显示导入表的信息
	CString sImportRVA;
	CString sImportSize;
	sImportRVA.Format(L"%08X", pImport->VirtualAddress);
	sImportSize.Format(L"%08X", pImport->Size);

	m_ImportRVA = sImportRVA;
	m_ImportSize = sImportSize;

	//找到资源表
	PIMAGE_DATA_DIRECTORY pResource = &pNt->OptionalHeader.DataDirectory[2];
	//显示资源表信息
	CString sResourceRVA;
	CString sResourceSize;
	sResourceRVA.Format(L"%08X", pResource->VirtualAddress);
	sResourceSize.Format(L"%08X", pResource->Size);
	m_ResourceRVA = sResourceRVA;
	m_ResourceSize = sResourceSize;

	//找到重定位表
	PIMAGE_DATA_DIRECTORY pReloc = &pNt->OptionalHeader.DataDirectory[5];
	//显示重定位表信息
	CString sRelocRVA;
	CString sRelocSize;
	sRelocRVA.Format(L"%08X", pReloc->VirtualAddress);
	sRelocSize.Format(L"%08X", pReloc->Size);
	m_RelocRVA = sRelocRVA;
	m_RelocSize = sRelocSize;

	//找到TLS表
	PIMAGE_DATA_DIRECTORY pTLS = &pNt->OptionalHeader.DataDirectory[9];
	//显示TLS表信息
	CString sTLSRVA;
	CString sTLSSize;
	sTLSRVA.Format(L"%08X", pTLS->VirtualAddress);
	sTLSSize.Format(L"%08X", pTLS->Size);
	m_TLSRVA = sTLSRVA;
	m_TLSSize = sTLSSize;

	//找到延迟载入表
	PIMAGE_DATA_DIRECTORY pDelay = &pNt->OptionalHeader.DataDirectory[13];
	//显示延迟载入表信息
	CString sDelayRVA;
	CString sDelaySize;
	sDelayRVA.Format(L"%08X", pDelay->VirtualAddress);
	sDelaySize.Format(L"%08X", pDelay->Size);
	m_DelayRVA = sDelayRVA;
	m_DelaySize = sDelaySize;
	UpdateData(FALSE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//点击按钮弹出导出表信息信息
void CDirectory::OnBnClickedButton1()
{
	if (m_ExportSize!="00000000")
	{
		CExport *Export = new CExport();
		Export->Create(IDD_Export, this);
		Export->ShowWindow(SW_SHOW);
	}
	else
	{
		MessageBox(L"PE文件没有导出表");
	}
	
}

//点击按钮弹出导入表
void CDirectory::OnBnClickedButton2()
{
	if (m_ImportSize!="00000000")
	{
		CImport *Import = new CImport();
		Import->Create(IDD_Import, this);
		Import->ShowWindow(SW_SHOW);
	}
	else
	{
		MessageBox(L"PE文件没有导入表");
	}
	
}

//点击弹出资源按钮
void CDirectory::OnBnClickedButton8()
{
	if (m_ResourceSize!="00000000")
	{
		CResoure *Resoure = new CResoure;
		Resoure->Create(IDD_Resoure, this);
		Resoure->ShowWindow(SW_SHOW);
	}
	else
	{
		MessageBox(L"PE文件没有资源表");
	}
	
}

//点击按钮弹出重定位表
void CDirectory::OnBnClickedButton9()
{
	if (m_ResourceSize!="00000000")
	{
		CReloc *Reloc = new CReloc;
		Reloc->Create(IDD_Reloc, this);
		Reloc->ShowWindow(SW_SHOW);
	}
	else
	{
		MessageBox(L"PE文件没有重定位表");
	}
	
}

//点击弹出TLS表
void CDirectory::OnBnClickedButton5()
{
	if (m_TLSSize!="00000000")
	{
		CTls *Tls = new CTls;
		Tls->Create(IDD_TLS, this);
		Tls->ShowWindow(SW_SHOW);
	}
	else
	{
		MessageBox(L"PE文件没有TLS表");
	}
	
}

//点击弹出延迟载入表
void CDirectory::OnBnClickedButton10()
{
	if (m_DelaySize!="00000000")
	{
		CDelay *Delay = new CDelay;
		Delay->Create(IDD_Delay, this);
		Delay->ShowWindow(SW_SHOW);
	}
	else
	{
		MessageBox(L"PE文件没有延时载入表");
	}
}
