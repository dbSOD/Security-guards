// Delay.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Delay.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"


// CDelay �Ի���

IMPLEMENT_DYNAMIC(CDelay, CDialogEx)

CDelay::CDelay(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDelay::IDD, pParent)
	, m_AllAttributes(_T(""))
	, m_DllAllName(_T(""))
	, m_ModuleHandleRVA(_T(""))
	, m_ImportAddressRVA(_T(""))
	, m_ImportNameRVA(_T(""))
	, m_BoundImportAddress(_T(""))
	, m_UnloadInfo(_T(""))
	, m_TimeData(_T(""))
{

}

CDelay::~CDelay()
{
}

void CDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_AllAttributes);
	DDX_Text(pDX, IDC_EDIT15, m_DllAllName);
	DDX_Text(pDX, IDC_EDIT16, m_ModuleHandleRVA);
	DDX_Text(pDX, IDC_EDIT17, m_ImportAddressRVA);
	DDX_Text(pDX, IDC_EDIT11, m_ImportNameRVA);
	DDX_Text(pDX, IDC_EDIT2, m_BoundImportAddress);
	DDX_Text(pDX, IDC_EDIT3, m_UnloadInfo);
	DDX_Text(pDX, IDC_EDIT4, m_TimeData);
}


BEGIN_MESSAGE_MAP(CDelay, CDialogEx)
END_MESSAGE_MAP()


// CDelay ��Ϣ�������


BOOL CDelay::OnInitDialog()
{
	//�ҵ��ӳټ��ر�
	CDialogEx::OnInitDialog();
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew+CDia5::g_lpBase);
	PIMAGE_DATA_DIRECTORY pDir = (PIMAGE_DATA_DIRECTORY)&pNt->OptionalHeader.DataDirectory[13];
	DWORD dwDelayFOA = CAddressChange::RVAtoFOA(pDir->VirtualAddress);
	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = (PIMAGE_DELAYLOAD_DESCRIPTOR)(dwDelayFOA + CDia5::g_lpBase);
	//��ʾ���ؼ�
	//����
	CString Contect;
	Contect.Format(L"%08X", pDelay->Attributes);
	m_AllAttributes = Contect;
	//����
	Contect.Format(L"%08X", pDelay->DllNameRVA);
	m_DllAllName = Contect;
	//ģ����
	Contect.Format(L"%08X", pDelay->ModuleHandleRVA);
	m_ModuleHandleRVA = Contect;
	//INT
	Contect.Format(L"%08X", pDelay->ImportNameTableRVA);
	m_ImportNameRVA = Contect;
	//IAT
	Contect.Format(L"%08X", pDelay->ImportAddressTableRVA);
	m_ImportAddressRVA = Contect;
	//��IAT
	Contect.Format(L"%08X", pDelay->BoundImportAddressTableRVA);
	m_BoundImportAddress = Contect;
	//ж����Ϣ
	Contect.Format(L"%08X", pDelay->UnloadInformationTableRVA);
	m_UnloadInfo = Contect;
	//ʱ���
	Contect.Format(L"%08X", pDelay->TimeDateStamp);
	m_TimeData = Contect;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
