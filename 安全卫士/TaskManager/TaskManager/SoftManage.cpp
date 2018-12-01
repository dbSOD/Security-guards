// SoftManage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "SoftManage.h"
#include "afxdialogex.h"


// CSoftManage �Ի���

IMPLEMENT_DYNAMIC(CSoftManage, CDialogEx)

CSoftManage::CSoftManage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSoftManage::IDD, pParent)
{

}

CSoftManage::~CSoftManage()
{
}

void CSoftManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CSoftManage, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSoftManage::OnRclickList1)
	ON_COMMAND(ID_32806, &CSoftManage::OnUninstall)
	ON_COMMAND(ID_32807, &CSoftManage::OnFlush)
	ON_COMMAND(ID_32808, &CSoftManage::OnOpenSoft)
END_MESSAGE_MAP()


// CSoftManage ��Ϣ�������


BOOL CSoftManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ʼ��List
	m_ListCtrl.InsertColumn(0, L"����", 0, 350);
	m_ListCtrl.InsertColumn(1, L"������", 0, 200);
	m_ListCtrl.InsertColumn(2, L"��װʱ��", 0, 100);
	m_ListCtrl.InsertColumn(3, L"�汾", 0, 150);
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//���������Ϣ
	GetSoftwareInfo();
	return TRUE; 
}

//���������Ϣ
void CSoftManage::GetSoftwareInfo()
{
	m_ListCtrl.DeleteAllItems();
	m_vecSoftInfo.clear();
	SOFTINFO stcSoftInfo = {0};

	HKEY RootKey;	//����
	LPCTSTR lpSubKey;	//�Ӽ�����
	HKEY hkResult;		//��Ҫ�򿪼��ľ��
	HKEY hkRKey;
	LONG lReturn;		//��¼��ȡע����Ƿ�ɹ�
	CString strBuffer;
	CString strMidReg;
	DWORD index = 0;
	TCHAR szKeyName[255] = { 0 };	//ע���������
	DWORD dwKeyLen = 255;
	DWORD dwNameLen = 255;
	DWORD dwType = 0;

	RootKey = HKEY_LOCAL_MACHINE;
	lpSubKey = Is64() ? Win64PATH : Win32PATH;
	//��һ���Ѵ��ڵ�ע����
	lReturn = RegOpenKeyEx(
		RootKey,	    //�������
		lpSubKey,	  //�ӽ�·��
		0,			  //���� ����Ϊ0
		KEY_ALL_ACCESS, //�򿪼������Ȩ��
		&hkResult		//�Ӽ����
		);
	if (lReturn==ERROR_SUCCESS)
	{
		DWORD index = 0;
		DWORD ListIndex = 0;
		//ѭ������UninstallĿ¼�µ��Ӽ�
		while (ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hkResult, index, szKeyName, &dwKeyLen, 0, NULL, NULL, NULL))
		{
			//ͨ���õ����Ӽ�����������ϳ��µ��Ӽ�·��
			strBuffer.Format(L"%s", szKeyName);
			if (!strBuffer.IsEmpty())
			{
				strMidReg = ((CString)lpSubKey + L"\\" + strBuffer);
			}
			//���µ��Ӽ� ��ȡ���
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,strMidReg,0,KEY_ALL_ACCESS,&hkRKey)==ERROR_SUCCESS)
			{
				//��ȡ�������
				RegQueryValueEx(hkRKey,L"DisplayName",0,&dwType,(LPBYTE)stcSoftInfo.szSoftName,&dwNameLen);
				dwNameLen = 255;	//���û�����¸�ֵ ��һ�ν���ȡ������Ϣ
				//��ȡ����汾
				RegQueryValueEx(hkRKey,L"DisplayVersion",0,&dwType,(LPBYTE)stcSoftInfo.szSoftVer,&dwNameLen);
				dwNameLen = 255;
				//��ȡ�����װ·��
				RegQueryValueEx(hkRKey,L"InstallLocation",0,&dwType,(LPBYTE)stcSoftInfo.strSoftInsPath,&dwNameLen);
				dwNameLen = 255;
				//��ȡ���������
				RegQueryValueEx(hkRKey, _T("Publisher"),0, &dwType, (LPBYTE)stcSoftInfo.strSoftVenRel, &dwNameLen);
				dwNameLen = 255;
				//��ȡ���ж��·��
				RegQueryValueEx(hkRKey, _T("UninstallString"),	0, &dwType, (LPBYTE)stcSoftInfo.strSoftUniPath, &dwNameLen);
				dwNameLen = 255;
				//��ȡ�����װʱ��
				RegQueryValueEx(hkRKey, _T("InstallDate"),0, &dwType, (LPBYTE)stcSoftInfo.szSoftData, &dwNameLen);
				
				if (stcSoftInfo.szSoftData[0])
				{
					stcSoftInfo.szSoftData[9] = stcSoftInfo.szSoftData[7];
					stcSoftInfo.szSoftData[8] = stcSoftInfo.szSoftData[6];
					stcSoftInfo.szSoftData[7] = '-';
					stcSoftInfo.szSoftData[6] = stcSoftInfo.szSoftData[5];
					stcSoftInfo.szSoftData[5] = stcSoftInfo.szSoftData[4];
					stcSoftInfo.szSoftData[4] = '-';
				}
				dwNameLen = 255;

				//��������
				if (stcSoftInfo.szSoftName[0]!='\0')
				{
					m_vecSoftInfo.push_back(stcSoftInfo);
					//��ʾ��List
					m_ListCtrl.InsertItem(ListIndex, stcSoftInfo.szSoftName);
					m_ListCtrl.SetItemText(ListIndex, 1, stcSoftInfo.strSoftVenRel);
					m_ListCtrl.SetItemText(ListIndex, 2, stcSoftInfo.szSoftData);
					m_ListCtrl.SetItemText(ListIndex, 3, stcSoftInfo.szSoftVer);
				}
				index++;
			}
			dwKeyLen = 255;
			memset(szKeyName, 0, 255);
			memset(&stcSoftInfo, 0, sizeof(SOFTINFO));
		}
	}
	//�ر�ע����
	RegCloseKey(hkResult);

}

//�ж�ϵͳ�Ƿ���64λ
BOOL CSoftManage::Is64()
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_IA64)
	{
		//64λ
		return TRUE;
	}
	else
	{
		//32λ����ϵͳ
		return FALSE;
	}
	return FALSE;
}

//��Ӧ����Ҽ� �����˵�
void CSoftManage::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int n = pNMItemActivate->iItem;//ѡ���˵ڼ���---------------------------------------

	if (n != -1)
	{
		//������ѡ�����ж������
		strTemp = m_ListCtrl.GetItemText(n, 0);
		for (DWORD i = 0; i < m_vecSoftInfo.size(); i++)
		{
			if (strTemp == m_vecSoftInfo[i].szSoftName)
			{
				m_Select = i;
				break;
			}
		}
		strTemp.Empty();
	}
	CMenu m_Menu, *tMenu;
	m_Menu.LoadMenu(IDR_MENU2);

	//�õ���0�������˵�(�˵������кܶ��� ����������õ��ڼ���)
	tMenu = m_Menu.GetSubMenu(6);

	//��ȡ���λ��
	CPoint pt;
	GetCursorPos(&pt);

	//�����˵�
	TrackPopupMenu(tMenu->m_hMenu,
		TPM_LEFTALIGN,
		pt.x, pt.y,	//����
		0, m_hWnd   //���
		, 0);

	*pResult = 0;
}

//ж�����
void CSoftManage::OnUninstall()
{
	if (m_Select == -1)
	{
		return;
	}
	ShellExecute(NULL, _T("open"),
		m_vecSoftInfo[m_Select].strSoftUniPath, NULL, NULL, SW_SHOWNORMAL);
}

//ˢ��
void CSoftManage::OnFlush()
{
	GetSoftwareInfo();
}

//�����λ��
void CSoftManage::OnOpenSoft()
{
	if (!m_vecSoftInfo[m_Select].strSoftInsPath[0])
	{
		return;
	}
	//���ļ�
	OpenFolderAndSelectFile(m_vecSoftInfo[m_Select].strSoftInsPath);
}

//���ļ�λ��
BOOL CSoftManage::OpenFolderAndSelectFile(TCHAR* szPath)
{
	LPSHELLFOLDER pDesktopFolder;
	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		LPITEMIDLIST     pidl;
		ULONG             chEaten;
		ULONG             dwAttributes;
		HRESULT             hr;

		hr = pDesktopFolder->ParseDisplayName(
			NULL, NULL, szPath, &chEaten, &pidl, &dwAttributes);
		if (FAILED(hr))
		{
			pDesktopFolder->Release();
			return FALSE;
		}
		LPCITEMIDLIST pidlFolder = pidl;

		CoInitialize(NULL);
		hr = SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);

		pDesktopFolder->Release();

		if (hr == S_OK)
		{
			return TRUE;
		}
	}
	return FALSE;
}