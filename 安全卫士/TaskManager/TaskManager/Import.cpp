// Import.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Import.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"


// CImport �Ի���

IMPLEMENT_DYNAMIC(CImport, CDialogEx)

CImport::CImport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImport::IDD, pParent)
{

}

CImport::~CImport()
{
}

void CImport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrlUp);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrlDown);
}


BEGIN_MESSAGE_MAP(CImport, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CImport::OnClickList1)
END_MESSAGE_MAP()


// CImport ��Ϣ�������


BOOL CImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ʼ������Ϣ
	m_ListCtrlUp.InsertColumn(0, L"DLL����",0,100);
	m_ListCtrlUp.InsertColumn(1, L"OriginalFirstThunk", 0, 100);
	m_ListCtrlUp.InsertColumn(2, L"����ʱ���־", 0, 100);
	m_ListCtrlUp.InsertColumn(3, L"ForwarderChain", 0, 100);
	m_ListCtrlUp.InsertColumn(4, L"����", 0, 100);
	m_ListCtrlUp.InsertColumn(5, L"FirstThunk", 0, 100);
	m_ListCtrlUp.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ListCtrlDown.InsertColumn(0, L"ThunkRVA", 0, 120);
	m_ListCtrlDown.InsertColumn(1, L"Thunkƫ��", 0, 120);
	m_ListCtrlDown.InsertColumn(2, L"Thunkֵ", 0, 120);
	m_ListCtrlDown.InsertColumn(3, L"��ʾ", 0, 120);
	//m_ListCtrlDown.InsertColumn(4, L"API����", 0, 120);

	//�ҵ������
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + (DWORD)CDia5::g_lpBase);
	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[1];

	DWORD dwImportFOA = CAddressChange::RVAtoFOA(pDir->VirtualAddress);

	//��������ļ��е�λ��
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(dwImportFOA + (DWORD)CDia5::g_lpBase);
	int i = 0;
	//���������
	while (pImport->Name)
	{
		DWORD dwNameFOA = CAddressChange::RVAtoFOA(pImport->Name);
		//----------------------------------��ʾ���ؼ�----------------------------
		//��ʾDLL����
		CString sName;
		sName.Format(L"%S", dwNameFOA + CDia5::g_lpBase);
		m_ListCtrlUp.InsertItem(i, sName);
		//��ʾname��RVA
		CString sNameRVA;
		sNameRVA.Format(L"%08X", pImport->Name);
		m_ListCtrlUp.SetItemText(i, 4, sNameRVA);
		//��ʾOriginalFirstThunk��������Ƶ�RVA
		CString sOriginalFirstThunk;
		sOriginalFirstThunk.Format(L"%08X", pImport->OriginalFirstThunk);
		m_ListCtrlUp.SetItemText(i, 1, sOriginalFirstThunk);
		//��ʾ����ʱ���־(û��)
		CString sTimeDate;
		sTimeDate.Format(L"%08X", pImport->TimeDateStamp);
		m_ListCtrlUp.SetItemText(i, 2, sTimeDate);
		//��ʾת������(û��)
		CString sForwarderChain;
		sForwarderChain.Format(L"%08X", pImport->ForwarderChain);
		m_ListCtrlUp.SetItemText(i, 3, sForwarderChain);
		//��ʾFirstThunk�����ַ���RVA
		CString sFirstThunk;
		sFirstThunk.Format(L"%08X", pImport->FirstThunk);
		m_ListCtrlUp.SetItemText(i, 5, sFirstThunk);

		i++;
		//��һ��ģ��
		pImport++;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//��Ӧ�б�ĵ����Ϣ
void CImport::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//ɾ��������
	m_ListCtrlDown.DeleteAllItems();
	//��ȡѡ����к�
	int nRow = m_ListCtrlUp.GetSelectionMark();
	//����һ����������Ż�ȡ����RVA
	WCHAR wFirstThunk[9] = {};
	//��ȡ�������Ʊ��RVA
	m_ListCtrlUp.GetItemText(nRow, 1, wFirstThunk, 9);
	DWORD dwFirstThunk = 0;
	//CStingת����ʮ�����Ƶ�DWORD
	_stscanf_s(wFirstThunk,L"%X",&dwFirstThunk);
	//����������Ʊ��FOA
	DWORD dwThunkFOA = CAddressChange::RVAtoFOA(dwFirstThunk);
	//�ҵ��������Ʊ�
	DWORD* pINTAddr = (DWORD*)(dwThunkFOA + CDia5::g_lpBase);
	int row = 0;	//Ҫ������кź�ѭ������
	CString Content;
	//��ʼ�����������Ʊ�
	while (*pINTAddr)
	{
		//��ʾFirstThunk��RVA
		Content.Format(L"%08X", dwFirstThunk + 4 * row);
		m_ListCtrlDown.InsertItem(row, Content);
		//��ʾFirstThunk���ļ�ƫ��
		Content.Format(L"%08X", dwThunkFOA + 4 * row);
		m_ListCtrlDown.SetItemText(row,1,Content);
		//��ʾFristThunk������ƫ���е�����
		Content.Format(L"%08X", *pINTAddr);
		m_ListCtrlDown.SetItemText(row, 2, Content);

		PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)pINTAddr;
		//�ж��Ƿ������Ƶ���
		if (!IMAGE_SNAP_BY_ORDINAL(pThunk->u1.Ordinal))
		{
			//ͨ����������RVA ���FOA �õ�ָ�����ֵĽṹ��
			DWORD pNameFOA = (DWORD)CAddressChange::RVAtoFOA(pThunk->u1.Ordinal);
			auto pName = (PIMAGE_IMPORT_BY_NAME)(pNameFOA + CDia5::g_lpBase);
			//��ʾ��ź�����
			Content.Format(L"%04X", pName->Hint);
			m_ListCtrlDown.SetItemText(row, 3, Content);
			Content = CString(pName->Name);
			m_ListCtrlDown.SetItemText(row, 4, Content.GetBuffer());
		}
		else
		{
			//�������ŵ���
			Content.Format(L"%04X", pThunk->u1.Ordinal & 0xFFFF);
			m_ListCtrlDown.SetItemText(row, 3, Content);
		}
		row++;
		pINTAddr++;
	}
	*pResult = 0;
}
