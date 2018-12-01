// SoftManage.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "SoftManage.h"
#include "afxdialogex.h"


// CSoftManage 对话框

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


// CSoftManage 消息处理程序


BOOL CSoftManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//初始化List
	m_ListCtrl.InsertColumn(0, L"名称", 0, 350);
	m_ListCtrl.InsertColumn(1, L"发布者", 0, 200);
	m_ListCtrl.InsertColumn(2, L"安装时间", 0, 100);
	m_ListCtrl.InsertColumn(3, L"版本", 0, 150);
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//遍历软件信息
	GetSoftwareInfo();
	return TRUE; 
}

//遍历软件信息
void CSoftManage::GetSoftwareInfo()
{
	m_ListCtrl.DeleteAllItems();
	m_vecSoftInfo.clear();
	SOFTINFO stcSoftInfo = {0};

	HKEY RootKey;	//主键
	LPCTSTR lpSubKey;	//子键名称
	HKEY hkResult;		//将要打开键的句柄
	HKEY hkRKey;
	LONG lReturn;		//记录读取注册表是否成功
	CString strBuffer;
	CString strMidReg;
	DWORD index = 0;
	TCHAR szKeyName[255] = { 0 };	//注册表项名称
	DWORD dwKeyLen = 255;
	DWORD dwNameLen = 255;
	DWORD dwType = 0;

	RootKey = HKEY_LOCAL_MACHINE;
	lpSubKey = Is64() ? Win64PATH : Win32PATH;
	//打开一个已存在的注册表键
	lReturn = RegOpenKeyEx(
		RootKey,	    //根键句柄
		lpSubKey,	  //子健路径
		0,			  //保留 必须为0
		KEY_ALL_ACCESS, //打开键句柄的权限
		&hkResult		//子键句柄
		);
	if (lReturn==ERROR_SUCCESS)
	{
		DWORD index = 0;
		DWORD ListIndex = 0;
		//循环遍历Uninstall目录下的子键
		while (ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hkResult, index, szKeyName, &dwKeyLen, 0, NULL, NULL, NULL))
		{
			//通过得到的子键名称重新组合成新的子键路径
			strBuffer.Format(L"%s", szKeyName);
			if (!strBuffer.IsEmpty())
			{
				strMidReg = ((CString)lpSubKey + L"\\" + strBuffer);
			}
			//打开新的子键 获取句柄
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,strMidReg,0,KEY_ALL_ACCESS,&hkRKey)==ERROR_SUCCESS)
			{
				//获取软件名称
				RegQueryValueEx(hkRKey,L"DisplayName",0,&dwType,(LPBYTE)stcSoftInfo.szSoftName,&dwNameLen);
				dwNameLen = 255;	//如果没有重新赋值 下一次将获取不到信息
				//获取软件版本
				RegQueryValueEx(hkRKey,L"DisplayVersion",0,&dwType,(LPBYTE)stcSoftInfo.szSoftVer,&dwNameLen);
				dwNameLen = 255;
				//获取软件安装路径
				RegQueryValueEx(hkRKey,L"InstallLocation",0,&dwType,(LPBYTE)stcSoftInfo.strSoftInsPath,&dwNameLen);
				dwNameLen = 255;
				//获取软件发布者
				RegQueryValueEx(hkRKey, _T("Publisher"),0, &dwType, (LPBYTE)stcSoftInfo.strSoftVenRel, &dwNameLen);
				dwNameLen = 255;
				//获取软件卸载路径
				RegQueryValueEx(hkRKey, _T("UninstallString"),	0, &dwType, (LPBYTE)stcSoftInfo.strSoftUniPath, &dwNameLen);
				dwNameLen = 255;
				//获取软件安装时间
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

				//保存数据
				if (stcSoftInfo.szSoftName[0]!='\0')
				{
					m_vecSoftInfo.push_back(stcSoftInfo);
					//显示到List
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
	//关闭注册表键
	RegCloseKey(hkResult);

}

//判断系统是否是64位
BOOL CSoftManage::Is64()
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_IA64)
	{
		//64位
		return TRUE;
	}
	else
	{
		//32位操作系统
		return FALSE;
	}
	return FALSE;
}

//响应鼠标右键 弹出菜单
void CSoftManage::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int n = pNMItemActivate->iItem;//选择了第几行---------------------------------------

	if (n != -1)
	{
		//保存所选软件的卸载命令
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

	//拿到第0个下拉菜单(菜单可能有很多列 这个函数是拿到第几列)
	tMenu = m_Menu.GetSubMenu(6);

	//获取鼠标位置
	CPoint pt;
	GetCursorPos(&pt);

	//弹出菜单
	TrackPopupMenu(tMenu->m_hMenu,
		TPM_LEFTALIGN,
		pt.x, pt.y,	//坐标
		0, m_hWnd   //句柄
		, 0);

	*pResult = 0;
}

//卸载软件
void CSoftManage::OnUninstall()
{
	if (m_Select == -1)
	{
		return;
	}
	ShellExecute(NULL, _T("open"),
		m_vecSoftInfo[m_Select].strSoftUniPath, NULL, NULL, SW_SHOWNORMAL);
}

//刷新
void CSoftManage::OnFlush()
{
	GetSoftwareInfo();
}

//打开软件位置
void CSoftManage::OnOpenSoft()
{
	if (!m_vecSoftInfo[m_Select].strSoftInsPath[0])
	{
		return;
	}
	//打开文件
	OpenFolderAndSelectFile(m_vecSoftInfo[m_Select].strSoftInsPath);
}

//打开文件位置
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