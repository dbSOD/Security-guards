// Resoure.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Resoure.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"
#include <map>
std::map<HTREEITEM, PIMAGE_RESOURCE_DIRECTORY_ENTRY>tree_addr;

// CResoure �Ի���

IMPLEMENT_DYNAMIC(CResoure, CDialogEx)

CResoure::CResoure(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResoure::IDD, pParent)
	, m_FirstEdit(_T("0000"))
	, m_SecondEidt(_T("0000"))
	, m_ThirdEidt(_T("0000"))
	, m_FouthEdit(_T("0000"))
	, m_RVAEdit(_T("00000000"))
	, m_FOAEdit(_T("00000000"))
	, m_SizeEdit(_T("00000000"))
{

}

CResoure::~CResoure()
{
}

void CResoure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_FirstEdit);
	DDX_Text(pDX, IDC_EDIT17, m_SecondEidt);
	DDX_Text(pDX, IDC_EDIT11, m_ThirdEidt);
	DDX_Text(pDX, IDC_EDIT22, m_FouthEdit);
	DDX_Text(pDX, IDC_EDIT20, m_RVAEdit);
	DDX_Text(pDX, IDC_EDIT23, m_FOAEdit);
	DDX_Text(pDX, IDC_EDIT24, m_SizeEdit);
}


BEGIN_MESSAGE_MAP(CResoure, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CResoure::OnClickTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CResoure::OnSelchangedTree1)
END_MESSAGE_MAP()


// CResoure ��Ϣ�������


BOOL CResoure::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//�ҵ���Դ��
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CDia5::g_lpBase);
	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[2];

	DWORD dwResFOA = CAddressChange::RVAtoFOA(pDir->VirtualAddress);

	//���� ���ҵ���Դ��ĵ�һ���ṹ��
	PIMAGE_RESOURCE_DIRECTORY pResHead = (PIMAGE_RESOURCE_DIRECTORY)(dwResFOA + CDia5::g_lpBase);
	pRhead = (DWORD)pResHead;

	//��һ�� ����
	WCHAR* arryResType[] = { L"", L"���ָ�루Cursor��", L"λͼ��Bitmap��", L"ͼ�꣨Icon��", L"�˵���Menu��"
		, L"�Ի���Dialog��", L"�ַ����б�String Table��", L"����Ŀ¼��Font Directory��", L"���壨Font��", L"��ݼ���Accelerators��"
		, L"�Ǹ�ʽ����Դ��Unformatted��", L"��Ϣ�б�Message Table��", L"���ָ���飨Croup Cursor��", L"", L"ͼ���飨Group Icon��", L""
		, L"�汾��Ϣ��Version Information��" };
	//��Դ����
	DWORD dwCount_1 = pResHead->NumberOfIdEntries + pResHead->NumberOfNamedEntries;
	CString Content;	//Ҫ��ʾ������
	//��ʾ��һ��Edit������
	Content.Format(L"%04X", pResHead->NumberOfIdEntries);
	m_FirstEdit = Content;
	//��ʾ�ڶ���Edit������
	Content.Format(L"%04X", pResHead->NumberOfNamedEntries);
	m_SecondEidt = Content;
	//�ҵ��������Ľṹ��
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResHeadEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResHead+1);
	//������Դ����
	for (DWORD i_1 = 0; i_1 < dwCount_1; i_1++)
	{
		HTREEITEM hFirst = {0};	//��һ��ĸ��ڵ���
		//�ж��Ƿ����ַ�������
		if (pResHeadEntry->NameIsString)
		{
			//������ַ������� ����RVA�ҵ������ַ������ֺͳ��ȵĽṹ��
			PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)(pResHeadEntry->NameOffset + (DWORD)pResHead);
			//�����ַ�����������ռ�
			WCHAR* wcName = new WCHAR[pName->Length + 1]{};
			//�����ֿ���������Ŀռ���
			memcpy(wcName,pName->NameString,pName->Length * 2);

			//��ʾ���ൽ���ռ�
			hFirst = m_TreeCtrl.InsertItem(wcName);
			tree_addr.insert({ hFirst, pResHeadEntry});
			//�ͷ�����Ŀռ�
			delete []wcName;
		}
		else
		{
			//�������ID����
			if (pResHeadEntry->Id <= 16 && pResHeadEntry->Id > 0)
			{
				hFirst = m_TreeCtrl.InsertItem(arryResType[pResHeadEntry->Id]);
				tree_addr.insert({ hFirst, pResHeadEntry});
			}
			else
			{
				Content.Format(L"%d", pResHeadEntry->Id);
				hFirst = m_TreeCtrl.InsertItem(Content);
				tree_addr.insert({ hFirst, pResHeadEntry});
			}
		}
		//�������һ�� �鿴�Ƿ�����һ��
		if (pResHeadEntry->DataIsDirectory)
		{
			//�ڶ���ĵ�һ���ṹ��
			PIMAGE_RESOURCE_DIRECTORY pRes2 = (PIMAGE_RESOURCE_DIRECTORY)(pResHeadEntry->OffsetToDirectory+(DWORD)pResHead);
			//�ڶ���ĸ���
			DWORD dwCount_2 = pRes2->NumberOfIdEntries + pRes2->NumberOfNamedEntries;
			//�ڶ���ĵڶ����ṹ��
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pRes2Entry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes2 + 1);
			//�����ڶ���
			for (DWORD i_2 = 0; i_2 < dwCount_2; i_2++)
			{
				HTREEITEM hSecond = {0};	//�ڶ���ĸ��ڵ�

				//�ж��Ƿ����ַ�������
				if (pRes2Entry->NameIsString)
				{
					//������ַ�������ͨ��RVA�ҵ�ָ���ַ������ֺͳ��ȵ��Ǹ��ṹ��
					PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)(pRes2Entry->NameOffset + (DWORD)pResHead);
					WCHAR* wcName = new WCHAR[pName->Length + 1]{};
					memcpy(wcName,pName->NameString,pName->Length*2);
					//����һ����ӽڵ����������ʾ�ڶ�����Դ
					hSecond = m_TreeCtrl.InsertItem(wcName, hFirst);
					tree_addr.insert({ hSecond, pRes2Entry});
					delete []wcName;
				}
				else
				{
					//ID����
					Content.Format(L"%d", pRes2Entry->Id);
					hSecond = m_TreeCtrl.InsertItem(Content, hFirst);
					tree_addr.insert({ hSecond, pRes2Entry});
				}
				//�ж��Ƿ�����һ��
				if (pRes2Entry->DataIsDirectory)
				{
					//�ҵ�������ĵ�һ���ṹ��
					PIMAGE_RESOURCE_DIRECTORY pRes3 = (PIMAGE_RESOURCE_DIRECTORY)(pRes2Entry->OffsetToDirectory + (DWORD)pResHead);
					//�ҵ�������ĵڶ����ṹ��
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pRes3Entry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes3+1);
					//�ҵ��ڶ���������ָ�����Դ�ṹ��
					PIMAGE_RESOURCE_DATA_ENTRY pDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(pRes3Entry->OffsetToData+(DWORD)pResHead);
					//ͨ��RVA�ҵ��������Դλ��
					DWORD dwDataFOA = CAddressChange::RVAtoFOA(pDataEntry->OffsetToData);
					DWORD dwDataSize = pDataEntry->Size;
					unsigned char* pData = (unsigned char*)(dwDataFOA + CDia5::g_lpBase);

					//��ʾ��������Դ������ 
					//Content.Format(L"%02X", pData);
					//m_TreeCtrl.InsertItem(Content, hSecond);
				}
				//��һ����Դ
				pRes2Entry++;
			}//�ڶ���
		}
		//��һ����Դ
		pResHeadEntry++;
	}//��һ��
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//��Ӧ���ؼ��ĵ����Ϣ
void CResoure::OnClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//��ȡ���ؼ�ѡ�е�����
	//CString Temp = m_TreeCtrl.GetItemText(m_TreeCtrl.GetSelectedItem());
	*pResult = 0;
}

//��Ӧ���ؼ��ĵ����Ϣ
void CResoure::OnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CString Content;
	//�õ�ѡ�еľ��
	HTREEITEM hTree=m_TreeCtrl.GetSelectedItem();
	//�õ���Ӧ�Ľṹ��
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResEntry = tree_addr[hTree];

	//����������һ�� �����һ�㻹����һ����ʾ��Դ����
	//��һ��ĵ�һ���ṹ��
	PIMAGE_RESOURCE_DIRECTORY pRes2 = (PIMAGE_RESOURCE_DIRECTORY)(pResEntry->OffsetToDirectory + pRhead);
	//��һ��ĵڶ����ṹ��
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pRes2Entry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes2 + 1);
	//��ʾ������Edit������
	Content.Format(L"%04X", pRes2->NumberOfNamedEntries);
	m_ThirdEidt = Content;
	//��ʾ���ĸ�Edit������
	Content.Format(L"%04X", pRes2->NumberOfIdEntries);
	m_FouthEdit = Content;

	//�����һ��û����һ�� ��ʾ��Դ����
	if (!(pRes2Entry->DataIsDirectory))
	{
		//�ҵ��ڶ���������ָ�����Դ�ṹ��
		PIMAGE_RESOURCE_DATA_ENTRY pDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(pRes2Entry->OffsetToData + pRhead);
		//��ʾRVA
		Content.Format(L"%08X", pDataEntry->OffsetToData);
		m_RVAEdit = Content;
		//��ʾFOA
		Content.Format(L"%08X", CAddressChange::RVAtoFOA(pDataEntry->OffsetToData));
		m_FOAEdit = Content;
		//��ʾSize
		Content.Format(L"%08X", pDataEntry->Size);
		m_SizeEdit = Content;
	}
	UpdateData(FALSE);
	*pResult = 0;
}
