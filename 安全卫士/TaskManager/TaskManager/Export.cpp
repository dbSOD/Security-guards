// Export.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Export.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"


// CExport �Ի���

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


// CExport ��Ϣ�������


BOOL CExport::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//�ҵ�������
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CDia5::g_lpBase);
	PIMAGE_DATA_DIRECTORY pExport = &pNt->OptionalHeader.DataDirectory[0];

	DWORD dwExportFOA = CAddressChange::RVAtoFOA(pExport->VirtualAddress);

	//�ҵ����������ļ��е�λ��
	PIMAGE_EXPORT_DIRECTORY pExportTable = (PIMAGE_EXPORT_DIRECTORY)(dwExportFOA + CDia5::g_lpBase);

	//��ʾ�����ƫ��
	CString cFOA;
	cFOA.Format(L"%08X", dwExportFOA);
	m_Offset = cFOA;
	
	//��ʾ����ֵ
	CString sCharact;
	sCharact.Format(L"%08X", pExportTable->Characteristics);
	m_Charact = sCharact;

	//��ʾ��ַ
	CString cBase;
	cBase.Format(L"%08X", pExportTable->Base);
	m_Base = cBase;

	//��ʾ����
	CString sName;
	sName.Format(L"%08X", pExportTable->Name);
	m_Name = sName;

	//��ʾ�����ִ�
	m_NameSrtring = (CAddressChange::RVAtoFOA(pExportTable->Name)) + CDia5::g_lpBase;

	//��ʾ��������
	CString sNumberOfFun;
	sNumberOfFun.Format(L"%08X", pExportTable->NumberOfFunctions);
	m_NumberOfFun = sNumberOfFun;

	//��ʾ������������
	CString sNumberOfName;
	sNumberOfName.Format(L"%08X", pExportTable->NumberOfNames);
	m_NumberOfName = sNumberOfName;

	//��ʾ������ַ��RVA
	CString sAddressOfFun;
	sAddressOfFun.Format(L"%08X", pExportTable->AddressOfFunctions);
	m_AddressOfFun = sAddressOfFun;

	//��ʾ�������Ʊ�RVA
	CString sAddressOfName;
	sAddressOfName.Format(L"%08X", pExportTable->AddressOfNames);
	m_AddressOfName = sAddressOfName;

	//��ʾ������ű�RVA
	CString sAddressOfOrdinal;
	sAddressOfOrdinal.Format(L"%08X", pExportTable->AddressOfNameOrdinals);
	m_AddressOfOrdinal = sAddressOfOrdinal;

	//��ʾListCtrl
	//�����
	m_ListCtrl.InsertColumn(0, L"���", 0, 100);
	m_ListCtrl.InsertColumn(1, L"RVA", 1, 100);
	m_ListCtrl.InsertColumn(2, L"ƫ��", 2, 100);
	m_ListCtrl.InsertColumn(3, L"������", 3, 100);

	DWORD dwFunCount = pExportTable->NumberOfFunctions;		//����������
	DWORD dwOrdinalCount = pExportTable->NumberOfNames;		//�������ֺ���ŵ�����

	//�ҵ���ַ��
	DWORD* pFunAddr = (DWORD*)(CAddressChange::RVAtoFOA(pExportTable->AddressOfFunctions) + CDia5::g_lpBase);
	//�ҵ����Ʊ�
	DWORD* pNameAddr = (DWORD*)(CAddressChange::RVAtoFOA(pExportTable->AddressOfNames) + CDia5::g_lpBase);
	//�ҵ���ű�
	WORD* pOrdinalAddr = (WORD*)(CAddressChange::RVAtoFOA(pExportTable->AddressOfNameOrdinals)+CDia5::g_lpBase);

	//��ʼ����������
	DWORD dwListIndex = 0;
	for (DWORD i = 0; i < dwFunCount; i++)
	{
		//�����0˵����ַ��Ч ֱ������
		if (pFunAddr[i]==0)
		{
			continue;
		}
		//������ű����Ƿ��д���� �����˵���д˺���������
		BOOL bFlag = FALSE;

		//��ʾ���ؼ���
		CString sNameRVA;	//RVA
		sNameRVA.Format(L"%08X", pFunAddr[i]);
		CString index;		//���
		index.Format(L"%08X", i + pExportTable->Base);
		CString sFOA;	//FOA
		sFOA.Format(L"%08X", CAddressChange::RVAtoFOA(pFunAddr[i]));
		m_ListCtrl.InsertItem(dwListIndex, index);	//���
		m_ListCtrl.SetItemText(dwListIndex, 1, sNameRVA);	//RVA
		m_ListCtrl.SetItemText(dwListIndex, 2, sFOA);	//FOA

		for (DWORD j = 0; j < dwOrdinalCount; j++)
		{
			//���˵������������
			if (i == pOrdinalAddr[j])
			{
				bFlag = TRUE;
				DWORD dwNameRVA = pNameAddr[j];
				CString sName;	//������
				sName.Format(L"%s", CString(CAddressChange::RVAtoFOA(dwNameRVA) + CDia5::g_lpBase));
				m_ListCtrl.SetItemText(dwListIndex++, 3, sName);	//������	
			}
		}
		//�����ű���û�� ˵���˺���ֻ�����û������
		if (!bFlag)
		{
			m_ListCtrl.SetItemText(dwListIndex++, 3, L"NULL");	//����
		}
	}

	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
