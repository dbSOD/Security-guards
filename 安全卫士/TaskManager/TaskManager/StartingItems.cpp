// StartingItems.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "StartingItems.h"
#include "afxdialogex.h"


// CStartingItems �Ի���

IMPLEMENT_DYNAMIC(CStartingItems, CDialogEx)

CStartingItems::CStartingItems(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartingItems::IDD, pParent)
{
	m_Select = -1;
}

CStartingItems::~CStartingItems()
{
}

void CStartingItems::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CStartingItems, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CStartingItems::OnRclickList2)
	ON_COMMAND(ID_32809, &CStartingItems::OnAddStart)
	ON_COMMAND(ID_32810, &CStartingItems::OnRemoveStart)
	ON_COMMAND(ID_32811, &CStartingItems::OnRefresh)
END_MESSAGE_MAP()


// CStartingItems ��Ϣ�������


BOOL CStartingItems::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ʼ��List
	m_ListCtrl.InsertColumn(0, L"������Ŀ", 0, 100);
	m_ListCtrl.InsertColumn(1, L"����", 0, 300);
	m_ListCtrl.InsertColumn(2, L"λ��", 0, 400);
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//��ȡ��������Ϣ
	GetBootInfo();
	return TRUE;  
}

//��ȡ��������Ϣ
void CStartingItems::GetBootInfo()
{
	m_ListCtrl.DeleteAllItems();
	m_vecBootInfo.clear();
	BOOTINFO stcBootInfo = { 0 };

	HKEY RootKey = NULL;						// ����
	HKEY hkResult = NULL;						// ��Ҫ�򿪼��ľ�� 
	LONG lReturn;						// ��¼��ȡע����Ƿ�ɹ�
	DWORD index = 0;
	TCHAR szKeyName[255] = { 0 };       // ע���������
	DWORD dwKeyLen = 255;
	DWORD dwNameLen = 255;
	DWORD dwType = 0;
	CString buffer;
	LPCTSTR lpSubKey[2];				// �Ӽ�����
	DWORD ListIndex = 0;

	lpSubKey[0] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	lpSubKey[1] = _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run");
	
	for (DWORD i = 0; i < 2; i++)
	{
		if (i == 0)
			RootKey = HKEY_CURRENT_USER;
		if (i == 1)
			RootKey = HKEY_LOCAL_MACHINE;

		//��lpSubkey
		lReturn = RegOpenKeyEx(RootKey, lpSubKey[i], 0,
			KEY_ALL_ACCESS, &hkResult);
		if (lReturn == ERROR_SUCCESS)
		{
			//����򿪳ɹ�����ʼö����ֵ
			while (ERROR_NO_MORE_ITEMS !=
				RegEnumValue(hkResult, index, stcBootInfo.szBootName, &dwKeyLen,
				0, NULL, NULL, NULL))
			{
				//��ȡÿ��ֵ������ szKeyNameΪֵ������ bufferΪֵ������
				RegQueryValueEx(hkResult, stcBootInfo.szBootName,
					0, &dwType, (LPBYTE)stcBootInfo.szBootOrder, &dwNameLen);
				//������Ϣ
				//������	stcBootInfo.szBootName
				//����	stcBootInfo.szBootOrder

				//����λ����Ϣ
				//������Ϣ
				stcBootInfo.szHKEY;
				if (i == 0)
					_tcscpy_s(stcBootInfo.szHKEY, _T("HKEY_CURRENT_USER"));
				if (i == 1)
					_tcscpy_s(stcBootInfo.szHKEY, _T("HKEY_LOCAL_MACHINE"));
				//ɾ��ʱҪ�õ���
				_tcscpy_s(stcBootInfo.szBootPos, lpSubKey[i]);
				//List��Ҫ��ʾ��
				buffer.Empty();
				buffer = buffer + stcBootInfo.szHKEY + _T("\\") + stcBootInfo.szBootPos;
				_tcscpy_s(stcBootInfo.szBootPos_All, buffer);


				//��ӵ�vector
				m_vecBootInfo.push_back(stcBootInfo);
				//����List
				m_ListCtrl.InsertItem(ListIndex, stcBootInfo.szBootName);
				m_ListCtrl.SetItemText(ListIndex, 1, stcBootInfo.szBootOrder);
				m_ListCtrl.SetItemText(ListIndex, 2, stcBootInfo.szBootPos_All);
				index++;
				dwNameLen = 255;
				dwKeyLen = 255;
				memset(&stcBootInfo, 0, sizeof(BOOTINFO));
			}
			index = 0;
			RegCloseKey(hkResult);
		}
	}
}


//��Ӧ�Ҽ���Ϣ �����˵�
void CStartingItems::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//��ֹ�ڿհ�����������˵�  
	int n = pNMItemActivate->iItem;//ѡ���˵ڼ���
	if (n != -1)
	{
		//������ѡ�����������

		m_Select = n;
		CMenu m_Menu, *tMenu;
		m_Menu.LoadMenu(IDR_MENU2);

		//�õ���0�������˵�(�˵������кܶ��� ����������õ��ڼ���)
		tMenu = m_Menu.GetSubMenu(7);

		//��ȡ���λ��
		CPoint pt;
		GetCursorPos(&pt);

		//�����˵�
		TrackPopupMenu(tMenu->m_hMenu,
			TPM_LEFTALIGN,
			pt.x, pt.y,	//����
			0, m_hWnd   //���
			, 0);
	}
	

	*pResult = 0;
}


//���������
void CStartingItems::OnAddStart()
{
	//���ļ�����
	TCHAR FilePathName[MAX_PATH] = { 0 };
	CString FileName;
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		//�ļ�����������FilePathName��
		_tcscpy_s(FilePathName, dlg.GetPathName());
	}
	else
	{
		return;
	}

	//��������Key
	HKEY hKey;
	LPCTSTR lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		//��ȡ·������
		DWORD dwRet = _tcslen(FilePathName);

		//�����������Ϣ
		FileName = dlg.GetFileTitle();
		int n = sizeof(TCHAR);
		lRet = RegSetValueEx(hKey, FileName, 0, REG_SZ,
			(BYTE *)FilePathName, dwRet*sizeof(TCHAR));

		//�ر�ע���   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(_T("���������ʧ��!"));
		}
		else
		{
			AfxMessageBox(_T("���������ɹ�!"));
			GetBootInfo();
		}
	}
}

//ɾ��������
void CStartingItems::OnRemoveStart()
{
	if (m_Select == -1)
	{
		return;
	}
	HKEY RootKey = NULL;		//����
	HKEY hKey;
	//ȷ��Ҫɾ���ĸ���
	strTemp = m_vecBootInfo[m_Select].szHKEY;
	if (strTemp == _T("HKEY_CURRENT_USER"))
	{
		RootKey = HKEY_CURRENT_USER;
	}
	if (strTemp == _T("HKEY_LOCAL_MACHINE"))
	{
		RootKey = HKEY_LOCAL_MACHINE;
	}
	if (RootKey == NULL)
	{
		return;
	}
	//ȷ��Ҫɾ����Run�Ӽ�·��
	LPCTSTR lpRun = m_vecBootInfo[m_Select].szBootPos;
	//��������Key   
	long lRet = RegOpenKeyEx(RootKey, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		//��ȡҪɾ�������������
		strTemp = m_vecBootInfo[m_Select].szBootName;
		//ɾ���������� 
		lRet = RegDeleteValue(hKey, strTemp);
		//�ر�ע���   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(_T("ɾ��������ɹ���"));
		}
		else{
			AfxMessageBox(_T("ɾ��������ɹ���"));
			GetBootInfo();
		}
	}
}

//ˢ��������
void CStartingItems::OnRefresh()
{
	GetBootInfo();
}
