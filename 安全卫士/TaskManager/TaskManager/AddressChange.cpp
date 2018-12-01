// AddressChange.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "AddressChange.h"
#include "afxdialogex.h"
#include <windows.h>
#include "Dia5.h"


// CAddressChange �Ի���

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


// CAddressChange ��Ϣ�������

//��ַת������
DWORD CAddressChange::RVAtoFOA(DWORD dwRVA)
{
	//�����ĸ�������
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + CDia5::g_lpBase);

	//�ҵ����α�ͷ��ַ
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	//��ȡ���θ���
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

//��Ӧ���㰴ť���е�ַת�� ����ƫ������VA��RVA
void CAddressChange::OnBnClickedButton2()
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + CDia5::g_lpBase);
	//�ҵ����α�ͷ��ַ
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);

	UpdateData(TRUE);
	//��CStringת��ʮ������
	DWORD nRVA = wcstol(m_RVA, NULL, 16);
	
	//ת����ַ
	DWORD dwFOA = RVAtoFOA(nRVA);
	DWORD dwVA = nRVA + pNt->OptionalHeader.ImageBase;

	//ת���ַ�����ʽ
	CString sFOA;
	CString sVA;

	sFOA.Format(L"%08X", dwFOA);
	sVA.Format(L"%08X", dwVA);

	//��ʾ���ؼ�
	m_Offect = sFOA;
	m_VA = sVA;
	m_Section = pSection->Name;


	//��������
	UpdateData(FALSE);


}


void CAddressChange::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
