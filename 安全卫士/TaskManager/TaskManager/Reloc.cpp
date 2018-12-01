// Reloc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Reloc.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"


// CReloc �Ի���

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


// CReloc ��Ϣ�������


BOOL CReloc::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ʼ��ListCtrl
	WCHAR *wcArr1[] = { L"����", L"����", L"RVA", L"��Ŀ",L" " };
	WCHAR *wcArr2[] = { L"����", L"RVA", L"ƫ��", L"����",L"FAR��ַ"};
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
	//�ҵ��ض�λ��
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew+CDia5::g_lpBase);
	//�ҵ����α�ͷ��ַ
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[5];

	DWORD  dwRelocFOA = CAddressChange::RVAtoFOA(pDir->VirtualAddress);
	
	PIMAGE_BASE_RELOCATION  pReloc = (PIMAGE_BASE_RELOCATION)(dwRelocFOA + CDia5::g_lpBase);

	DWORD dwSectionCount = pNt->FileHeader.NumberOfSections;  //��ȡ���θ���
	//����
	DWORD index = 0;	//�к�
	CString Contect;	//����
	while (pReloc->SizeOfBlock)
	{
		//��ʾ���ݵ��ؼ�
		//����
		Contect.Format(L"%d", index+1);
		m_ListCtrlUp.InsertItem(index, Contect);
		//������
		// �ж����ĸ�����
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
		//�ض�λ�����
		DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
		CString hCount;		//ʮ������
		CString dCount;		//ʮ����
		hCount.Format(L"%X", dwCount);
		dCount.Format(L"%d", dwCount);
		Contect.Format(L"%sh/%sd", hCount, dCount);
		m_ListCtrlUp.SetItemText(index++, 3, Contect);
		
		//��һ���ض�λ��
		pReloc = (PIMAGE_BASE_RELOCATION)(pReloc->SizeOfBlock+(DWORD)pReloc);
		//��һ������
		
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//��ӦList�ĵ����Ϣ
void CReloc::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_ListCtrlDown.DeleteAllItems();
	typedef struct _Offset
	{
		WORD Offset : 12;
		WORD Type : 4;
	}Offset;
	//�ҵ��ض�λ��
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CDia5::g_lpBase);
	//�ҵ����α�ͷ��ַ

	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[5];

	DWORD  dwRelocFOA = CAddressChange::RVAtoFOA(pDir->VirtualAddress);

	PIMAGE_BASE_RELOCATION  pReloc = (PIMAGE_BASE_RELOCATION)(dwRelocFOA + CDia5::g_lpBase);
	//����
	DWORD index = 0;	//�к�
	CString Contect;	//����
	//�ض�λ�����
	DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
	while (pReloc->SizeOfBlock)
	{
		
		//�ض�λ����ʼλ��
		Offset* pOffset = (Offset*)(pReloc + 1);

		//������Ҫ�ض�λ��ÿ������
		for (DWORD i = 0; i < dwCount; i++)
		{
			if (pOffset->Type==3)
			{
				//�����Ҫ�ض�λ�����ݵ�FOA
				DWORD dwDataFOA = CAddressChange::RVAtoFOA(pOffset->Offset + pReloc->VirtualAddress);
				//��Ҫ�ض�λ������
				DWORD* pData = (DWORD*)(dwDataFOA+CDia5::g_lpBase);
				//��ʾ��Ҫ�ض�λ������*pData
				//��ʾ���
				Contect.Format(L"%d", index + 1);
				m_ListCtrlDown.InsertItem(index, Contect);
				//��ʾRVA
				Contect.Format(L"%08X",pOffset->Offset + pReloc->VirtualAddress);
				m_ListCtrlDown.SetItemText(index, 1, Contect);
				//��ʾOFA
				Contect.Format(L"%08X", dwDataFOA);
				m_ListCtrlDown.SetItemText(index, 2, Contect);
				//��ʾType
				Contect.Format(L"%s(%d)", L"HIGHLOW",pOffset->Type);
				m_ListCtrlDown.SetItemText(index, 3, Contect);
				//��ʾVA
				Contect.Format(L"%08X", *pData);
				m_ListCtrlDown.SetItemText(index++, 4, Contect);
			}
			//��һ���ض�λ��
			pOffset++;
		}
		//��һ���ض�λ��
		pReloc = (PIMAGE_BASE_RELOCATION)(pReloc->SizeOfBlock + (DWORD)pReloc);
	}
	*pResult = 0;
}
