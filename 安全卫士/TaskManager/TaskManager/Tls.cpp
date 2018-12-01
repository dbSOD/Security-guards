// Tls.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Tls.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"


// CTls �Ի���

IMPLEMENT_DYNAMIC(CTls, CDialogEx)

CTls::CTls(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTls::IDD, pParent)
	, m_StartAddress(_T(""))
	, m_EndAddress(_T(""))
	, m_AddressOfCallBacks(_T(""))
	, m_SizeOfZeroFill(_T(""))
	, m_Characteristics(_T(""))
	, m_AddressOfIndex(_T(""))
{

}

CTls::~CTls()
{
}

void CTls::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_StartAddress);
	DDX_Text(pDX, IDC_EDIT3, m_EndAddress);
	//  DDX_Text(pDX, IDC_EDIT4, AddressOfIndex);
	DDX_Text(pDX, IDC_EDIT5, m_AddressOfCallBacks);
	DDX_Text(pDX, IDC_EDIT6, m_SizeOfZeroFill);
	DDX_Text(pDX, IDC_EDIT7, m_Characteristics);
	DDX_Text(pDX, IDC_EDIT4, m_AddressOfIndex);
}


BEGIN_MESSAGE_MAP(CTls, CDialogEx)
END_MESSAGE_MAP()


// CTls ��Ϣ�������


BOOL CTls::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString Contect;
	//�ҵ�TLS��
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CDia5::g_lpBase);
	PIMAGE_DATA_DIRECTORY pDir = (PIMAGE_DATA_DIRECTORY)&pNt->OptionalHeader.DataDirectory[9];
	
	DWORD dwTLSFOA = CAddressChange::RVAtoFOA(pDir->VirtualAddress);

	PIMAGE_TLS_DIRECTORY pTLS = (PIMAGE_TLS_DIRECTORY)(dwTLSFOA + (DWORD)CDia5::g_lpBase);
	//���ݿ鿪ʼVA
	Contect.Format(L"%08X", pTLS->StartAddressOfRawData);
	m_StartAddress = Contect;
	//���ݿ����VA
	Contect.Format(L"%08X", pTLS->EndAddressOfRawData);
	m_EndAddress = Contect;
	//��������VA
	Contect.Format(L"%08X", pTLS->AddressOfIndex);
	m_AddressOfIndex = Contect;
	//�ص���VA
	Contect.Format(L"%08X", pTLS->AddressOfCallBacks);
	m_AddressOfCallBacks = Contect;
	//�����С
	Contect.Format(L"%08X", pTLS->SizeOfZeroFill);
	m_SizeOfZeroFill = Contect;
	//����ֵ
	Contect.Format(L"%08X", pTLS->Characteristics);
	m_Characteristics = Contect;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
