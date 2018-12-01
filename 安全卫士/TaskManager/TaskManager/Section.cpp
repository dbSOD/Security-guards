// Section.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Section.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"


// CSection �Ի���

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


// CSection ��Ϣ�������


BOOL CSection::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ʼ��List�ؼ�
	WCHAR* wcArr[] = {L"����",L"VOffset",L"VSize",L"ROffset",L"RSize",L"��־"};
	DWORD len = sizeof(wcArr) / sizeof(wcArr[0]);
	for (DWORD i = 0; i < len; i++)
	{
		m_ListCtrl.InsertColumn(i, wcArr[i], 0, 85);
	}
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//�ҵ�����ͷ��
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew+CDia5::g_lpBase);
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	//ѭ��������α����Ϣ
	DWORD index = 0;	//�к�
	CString Contect;	//����
	WORD wCount = pNt->FileHeader.NumberOfSections;	//ѭ������ ���ļ�ͷ����������
	for (WORD i = 0; i < wCount; i++)
	{
		//������
		m_ListCtrl.InsertItem(index, (CString)pSection->Name);
		//��ʼ����������ַ
		Contect.Format(L"%08X", pSection->VirtualAddress);
		m_ListCtrl.SetItemText(index, 1, Contect);
		//���δ�С(���غ�)
		Contect.Format(L"%08X", pSection->Misc.VirtualSize);
		m_ListCtrl.SetItemText(index, 2, Contect);
		//�ļ�ƫ��
		Contect.Format(L"%08X", pSection->PointerToRawData);
		m_ListCtrl.SetItemText(index, 3, Contect);
		//���δ�С(�ļ���)
		Contect.Format(L"%08X", pSection->SizeOfRawData);
		m_ListCtrl.SetItemText(index, 4, Contect);
		//���ε�����
		Contect.Format(L"%08X", pSection->Characteristics);
		m_ListCtrl.SetItemText(index++, 5, Contect);
		//��һ������
		pSection++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
