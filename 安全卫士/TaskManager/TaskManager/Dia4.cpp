// Dia4.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia4.h"
#include "afxdialogex.h"
#include "Disk.h"
#include "FileAttr.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

HWND hComWnd = NULL;
HWND hListCtrl = NULL;
TCHAR CurrentDir[MAX_PATH] = { 0 };
TCHAR CurrentFile[MAX_PATH] = { 0 };

//LPCTSTR TypeName[7] = {
//	TEXT("δ֪�豸"),
//	TEXT("��Ч��·��"),
//	TEXT("���ƶ��豸"),
//	TEXT("�����ƶ��豸"),
//	TEXT("����������"),
//	TEXT("CD-ROM ������"),
//	TEXT("����洢��"),
//};

LPCTSTR ListColumn[4] = {
	TEXT("����"),
	TEXT("����ʱ��"),
	TEXT("����"),
	TEXT("��С"),
};

UINT ColumnWide[4] = {
	155, 120, 90, 50
};





// CDia4 �Ի���

IMPLEMENT_DYNAMIC(CDia4, CDialogEx)

CDia4::CDia4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia4::IDD, pParent)
{

}

CDia4::~CDia4()
{
}

void CDia4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComBox);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDia4, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDia4::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDia4::OnSelchangeCombo1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDia4::OnRclickList1)
	ON_COMMAND(ID_32802, &CDia4::OnPopFileAttr)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST1, &CDia4::OnItemactivateList1)
END_MESSAGE_MAP()


// CDia4 ��Ϣ�������


BOOL CDia4::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ʼ�������б������Ϣ ��ȡ�̷�
	TCHAR NameBuffer[MAX_PATH] = { 0 };
	//��ȡ�����߼������� �ŵ�NameBuffer��
	GetLogicalDriveStrings(MAX_PATH, NameBuffer);
	//ѭ�����е��ַ���  ÿ���ַ�����\0��β ѭ��һ��ֻ���õ�һ���ַ��� ѭ������֮����Ҫ�����ַ�������
	for (int i = 0; NameBuffer[i]!=0; ++i)
	{
		m_ComBox.AddString(&NameBuffer[i]);
		i += _tcslen(&NameBuffer[i]);
	}
	//����Ĭ��ѡ��
	m_ComBox.SetCurSel(0);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.InsertColumn(0, L"����", 0, 100);
	m_ListCtrl.InsertColumn(1, L"����ʱ��", 0, 200);
	m_ListCtrl.InsertColumn(2, L"����", 0, 100);
	m_ListCtrl.InsertColumn(3, L"��С", 0, 100);
	ShowFileList(L"C:\\");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//�鿴��ť
void CDia4::OnBnClickedButton1()
{
	TCHAR temp[20] = { 0 };
	m_ComBox.GetWindowText(temp, 20);
	CDisk *pCD = new CDisk(temp);
	pCD->DoModal();
	delete pCD;
}



void CDia4::ShowFileList(LPCTSTR FullPath)
{
	_tcscpy_s(CurrentDir, MAX_PATH, FullPath);
	m_ListCtrl.DeleteAllItems();
	TCHAR FindPath[MAX_PATH] = { 0 };
	_tcscat_s(FindPath, MAX_PATH, FullPath);
	_tcscat_s(FindPath, MAX_PATH, _T("\\*"));
	WIN32_FIND_DATA FindData = { 0 };
	HANDLE Handle = FindFirstFile(FindPath, &FindData);
	if (Handle != INVALID_HANDLE_VALUE)
	{
		int index = 0;
		LVITEM LvItem = { 0 };
		do {
			if (_tcscmp(FindData.cFileName, TEXT(".")) != 0)
			{
				LvItem.iItem = index;
				m_ListCtrl.InsertItem(&LvItem);
				m_ListCtrl.SetItemText(index, 0, FindData.cFileName);
				SYSTEMTIME SystemTime = { 0 };
				FileTimeToSystemTime(&FindData.ftCreationTime, &SystemTime);
				TCHAR CreateTime[MAX_PATH] = { 0 };
				_stprintf_s(CreateTime, MAX_PATH, TEXT("%d/%d/%d %d:%d:%d"),
					SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
					SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
				m_ListCtrl.SetItemText(index, 1, CreateTime);

				CreateTime[0] = 0;
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					_tcscat_s(CreateTime, MAX_PATH, TEXT("Ŀ¼ "));
				else
					_tcscat_s(CreateTime, MAX_PATH, TEXT("�ļ� "));
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
					_tcscat_s(CreateTime, MAX_PATH, TEXT("���� "));
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					_tcscat_s(CreateTime, MAX_PATH, TEXT("ֻ��"));
				m_ListCtrl.SetItemText(index, 2, CreateTime);

				CreateTime[0] = 0;
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					_tcscat_s(CreateTime, MAX_PATH, TEXT("-"));
				else
				{
					if (FindData.nFileSizeLow > 1073741824)
						_stprintf_s(CreateTime, MAX_PATH, TEXT("%.2lfGB"), FindData.nFileSizeLow / 1024.0 / 1024.0 / 1024.0);
					else if (FindData.nFileSizeLow > 1048576)
						_stprintf_s(CreateTime, MAX_PATH, TEXT("%.2lfMB"), FindData.nFileSizeLow / 1024.0 / 1024.0);
					else
						_stprintf_s(CreateTime, MAX_PATH, TEXT("%.2lfKB"), FindData.nFileSizeLow / 1024.0 / 1024.0);

				}
				m_ListCtrl.SetItemText(index, 3, CreateTime);

				++index;
			}

		} while (FindNextFile(Handle, &FindData));
	}
}


void CDia4::UpdateDir(LPCTSTR FullPath)
{
	TCHAR Path[MAX_PATH] = { 0 };
	_stprintf_s(Path, MAX_PATH, TEXT("%s\\%s"), CurrentDir, FullPath);

	if (GetFileAttributes(Path) & FILE_ATTRIBUTE_DIRECTORY)
	{
		ShowFileList(Path);
	}
}

//��Ӧ�л���������Ϣ
void CDia4::OnSelchangeCombo1()
{
	TCHAR temp[MAX_PATH] = { 0 };
	m_ComBox.GetWindowText(temp, MAX_PATH);
	ShowFileList(temp);
}

//�Ҽ����List�ؼ��������Բ˵�
void CDia4::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CMenu menu, *pmenu;
	menu.LoadMenu(IDR_MENU2);
	pmenu = menu.GetSubMenu(4);
	//����һ������ȷ�����λ�õ�λ�� 
	CPoint point1;
	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����
	GetCursorPos(&point1);
	//��ָ��λ����ʾ�����˵�
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
	*pResult = 0;
}

//�������ԶԻ���
void CDia4::OnPopFileAttr()
{
	int i = m_ListCtrl.GetSelectionMark();
	CString FileName = m_ListCtrl.GetItemText(i, 0);
	_stprintf_s(CurrentFile, MAX_PATH, L"%s\\%s", CurrentDir, FileName);

	CFileAttr *pFaD = new CFileAttr(CurrentFile);
	pFaD->DoModal();
	delete pFaD;
}

//˫����Ϣ
void CDia4::OnItemactivateList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int i = m_ListCtrl.GetSelectionMark();
	CString FileName = m_ListCtrl.GetItemText(i, 0);
	UpdateDir(FileName);
	*pResult = 0;
}
