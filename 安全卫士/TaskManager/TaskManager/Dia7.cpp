// Dia7.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia7.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <atlstr.h>


// CDia7 对话框

IMPLEMENT_DYNAMIC(CDia7, CDialogEx)

CDia7::CDia7(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia7::IDD, pParent)

	, m_szStatus(_T(""))
{
	
}

CDia7::~CDia7()
{
}

void CDia7::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Text(pDX, IDC_STATIC1, m_szStatus);
}


BEGIN_MESSAGE_MAP(CDia7, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON9, &CDia7::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON8, &CDia7::OnBnClickedButton8)
	ON_MESSAGE(WM_MYUPDATEDATA, &CDia7::OnMyupdatedata)
	ON_BN_CLICKED(IDC_BUTTON2, &CDia7::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDia7 消息处理程序



BOOL CDia7::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	//初始化List
	m_ListCtrl.InsertColumn(0, _T("进程名/文件名"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("进程ID/路径"), 0, 190);
	m_ListCtrl.InsertColumn(2, _T("优先级/大小"), 0, 100);
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//初始化连接socket
	InitSocket();
	return TRUE;  
}

//初始化扫描选项
void CDia7::InitSelect()
{
	if (IsSelect[0])
	{
		//单文件查杀
		IsSingleScan = TRUE;
	}
	else
	{
		//全路径查杀
		IsSingleScan = FALSE;
	}
	if (IsSelect[2])
	{
		//MD5查杀
		IsMD5 = TRUE;
	}
	else
	{
		//白名单查杀
		IsMD5 = FALSE;
	}
	if (IsSelect[4])
	{
		//本地查杀
		IsLocal = TRUE;
	}
	else
	{
		//云端查杀
		IsLocal = FALSE;
	}
}


//线程回调函数
DWORD WINAPI ScanThread(LPVOID lpVoid)
{
	CDia7* pScanCtrl = (CDia7*)lpVoid;
	//查杀路径判断
	if (pScanCtrl->IsSingleScan)
	{
		//单文件查杀
		pScanCtrl->Scan_Single(pScanCtrl->m_SinglePath);
	}
	else
	{
		//全路径查杀
		//调用路径扫描 传入路径参数
		pScanCtrl->Scam_All(pScanCtrl->m_AllPath);
	}

	pScanCtrl->m_szStatus.Format(_T("扫描完毕！本次发现病毒:%d"), pScanCtrl->m_vecVirInfo.size());
	pScanCtrl->SendMessage(WM_MYUPDATEDATA, FALSE);
	return 0;

}

//开始扫描
void CDia7::StartScan()
{
	//1 先判断扫描路径
	if (IsSingleScan&&IsMD5)
	{
		m_SinglePath.Empty();
		//如果为单个文件查杀 弹出文件选择框 路径保存在m_SinglePath
		CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),NULL);
		if (dlg.DoModal()==IDOK)
		{
			//打开成功 保存文件路径
			m_SinglePath = dlg.GetPathName();
		}
		else
		{
			MessageBox(L"打开文件失败！");
			return;
		}
		if (m_SinglePath.IsEmpty())
		{
			MessageBox(L"路径不能为空!");
			return;
		}
	}
	else
	{
		if (IsMD5)
		{
			//如果为全路径查杀 弹出文件夹选择框 路径保存在m_AllPath
			m_AllPath.Empty();
			m_AllPath = BrowseFolder(this->m_hWnd, _T("请选择查杀路径"));
			if (m_AllPath.IsEmpty())
			{
				MessageBox(L"查杀路径不能为空！");
				return;
			}
		}
		
	}
	m_ListCtrl.DeleteAllItems();
	//2 判断是本地还是云端
	if (IsLocal)
	{
		//如果是本地 再判断是MD5还是白名单 
		if (IsMD5)
		{
			//本地MD5 加载本地MD5病毒库
			//先获取病毒库中MD5的个数
			CString Temp;
			//读取.ini文件
			GetPrivateProfileString(
				_T("Num"),						 //INI文件中的一个字段名
				_T("MD5"), 						 //lpAppName 下的一个键名，也就是里面具体的变量名
				_T("DefaultName"),				 //指定的条目没有找到时返回的默认值。可设为空（""）
				Temp.GetBuffer(MAX_PATH),		 //接收INI文件中的值的CString对象，即接收缓冲区
				MAX_PATH, 						 //接收缓冲区的大小
				_T(".\\LocalVirBase\\Vir.ini")	 //完整的INI文件路径名
				);
			Temp.ReleaseBuffer();
			int n = _ttoi(Temp);
			//再根据个数遍历MD5信息 加载到Vector
			for (int i = 0; i < n; i++)
			{
				CString tempCount;
				tempCount.Format(_T("%d"), i);
				GetPrivateProfileString(_T("MD5"), tempCount, _T("DefaultName"), Temp.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\LocalVirBase\\Vir.ini"));
				Temp.ReleaseBuffer();
				m_LocalMD5.push_back(Temp);
			}
			if (m_LocalMD5.size() == 0)
			{
				MessageBox(L"读取本地病毒库失败");
				return;
			}
		}
		else
		{
			//白名单查杀 加载本地白名单病毒库
			//先获取病毒库中白名单的个数
			CString Temp;
			GetPrivateProfileString(_T("Num"), _T("White"), _T("DefaultName"), Temp.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\LocalVirBase\\Vir.ini"));
			Temp.ReleaseBuffer();
			int n = _ttoi(Temp);
			//再根据个数遍历MD5信息 加载到Vector里
			for (int i = 0; i < n; i++)
			{
				CString tempCount;
				tempCount.Format(L"%d", i);
				GetPrivateProfileString(_T("White"),
					tempCount, _T("DefaultName"),
					Temp.GetBuffer(MAX_PATH), MAX_PATH,
					_T(".\\LocalVirBase\\Vir.ini"));
				Temp.ReleaseBuffer();
				m_LocalWhite.push_back(Temp);
			}
			if (m_LocalWhite.size()==0)
			{
				MessageBox(L"读取白名单失败!");
				return;
			}
		}
	}
	else
	{
		//如果是云端 再判断是MD5还是白名单，下载云端病毒库信息
		if (IsMD5)
		{
//--------------------------云端MD5查杀 加载云端MD5病毒库---------------------------------------------------
			//发送消息
			Msg msg = { Md5Message };
			m_Socket.Send((char*)&msg, sizeof(msg));

			//先让用户等待一秒
			m_szStatus = L"正 在 连 接 服 务 器!";
			UpdateData(FALSE);
			Sleep(1000);

			//循环接收消息
			char szMD5[36];		//消息内容
			int nNumber=0;		//消息数量
			m_Socket.Receive(&nNumber, sizeof(nNumber));
			for (int i = 0; i < nNumber; i++)
			{
				m_Socket.Receive(szMD5, 36);
				m_ServerMD5.push_back((CString)szMD5);
			}		
		}
		else
		{
			//云端白名单查杀 
		}
	}


	//3 创建扫描线程 开始扫描病毒 防止干扰主线程
	CloseHandle(CreateThread(NULL, 0, ScanThread, this, 0, NULL));

}



//单个文件查杀
void CDia7::Scan_Single(LPCTSTR szPath)
{
	WIN32_FIND_DATA wfd;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	FindFirstFile(sFindFilter,&wfd);

	//判断是MD5查杀还是白名单查杀
	if (IsMD5)
	{
		//是MD5查杀 判断是否是病毒
		if (MD5Scan(szPath))
		{
			//插入到List
			m_ListCtrl.InsertItem(0, wfd.cFileName);	//文件名
			m_ListCtrl.SetItemText(0, 1, szPath);		//文件路径
			m_ListCtrl.SetItemText(0, 2, GetShowSize(wfd.nFileSizeLow));	//文件大小
			//保存到Vector
			VIRINFO VirInfo;
			_tcscpy_s(VirInfo.szVirName,wfd.cFileName);
			_tcscpy_s(VirInfo.szVirPath, szPath);
			_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
			m_vecVirInfo.push_back(VirInfo);
		}
	}
	else
	{
		//白名单查杀
		//如果是病毒 将文件信息保存到病毒Vector
		if (WhiteScan(szPath))
		{

			if (!IsLocal)
			{
				//插入到List
				m_ListCtrl.InsertItem(0, wfd.cFileName);	//文件名
				m_ListCtrl.SetItemText(0, 1, szPath);		//文件路径
				m_ListCtrl.SetItemText(0, 2,				//文件大小
					GetShowSize(wfd.nFileSizeLow));
				//保存到Vector
				VIRINFO VirInfo;
				_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
				_tcscpy_s(VirInfo.szVirPath, szPath);
				_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
				m_vecVirInfo.push_back(VirInfo);
			}
			
		}
	}

}

//MD5查杀
BOOL CDia7::MD5Scan(LPCTSTR szPath)
{
	//LPCTSTR 路径名转CHAR*
	int num = WideCharToMultiByte(CP_OEMCP, NULL, szPath, -1, NULL, 0, NULL, FALSE);
	char* pChar = new char[num];
	WideCharToMultiByte(CP_OEMCP, NULL, szPath, -1, pChar, num, NULL, FALSE);

	//获取文件的MD5值
	char* md5 = md5FileValue(pChar);
	//CHAR* 转LPCTSTR
	num = MultiByteToWideChar(0, 0, md5, -1, NULL, 0);
	wchar_t* wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, md5, -1, wide, num);
	m_szMD5 = wide;
	delete[]pChar;
	delete[]wide;

	//获取到文件的MD5信息 再判断是本地还是云端查杀
	if (IsLocal)
	{
		//本地查杀 与m_LocalMD5作对比
		for (DWORD i = 0; i < m_LocalMD5.size(); i++)
		{
			if (m_LocalMD5[i]==m_szMD5)
			{
				//如果匹配 说明是病毒
				return TRUE;
			}
		}
	}
	else
	{
		//云端MD5查杀		与m_ServerMD5作对比
		for (DWORD i = 0; i < m_ServerMD5.size(); i++)
		{
			if (m_ServerMD5[i] == m_szMD5)
			{
				//是病毒 返回真
				return TRUE;
			}
		}
		
	}
	return FALSE;
}


//点击扫描按钮响应消息
void CDia7::OnBnClickedButton9()
{
	//获取按钮状态
	IsSelect[0] = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();
	IsSelect[1] = ((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck();
	IsSelect[2] = ((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck();
	IsSelect[3] = ((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck();
	IsSelect[4] = ((CButton*)GetDlgItem(IDC_RADIO5))->GetCheck();
	IsSelect[5] = ((CButton*)GetDlgItem(IDC_RADIO6))->GetCheck();

	//判断按钮是否被选中
	if (IsSelect[0] == 0 && IsSelect[1]==0)
	{
		MessageBox(L"请选择查杀路径！");
		return;
	}
	if (IsSelect[2] == 0 && IsSelect[3] == 0)
	{
		MessageBox(L"请选择查杀方式！");
		return;
	}
	if (IsSelect[4] == 0 && IsSelect[5] == 0)
	{
		MessageBox(L"请选择病毒库！");
		return;
	}

	//初始化选项设置
	m_vecVirInfo.clear();
	m_LocalMD5.clear();
	m_LocalWhite.clear();
	m_ServerMD5.clear();
	m_ServerWhite.clear();
	InitSelect();
	//开始扫描
	StartScan();
}
//弹出选择文件夹菜单 返回路径
CString CDia7::BrowseFolder(HWND hWnd, LPCTSTR lpTitle)
{
	TCHAR szPath[MAX_PATH] = {0};
	BROWSEINFO m_bi;

	m_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	m_bi.hwndOwner = m_hWnd;
	m_bi.pidlRoot = NULL;
	m_bi.lpszTitle = lpTitle;
	m_bi.lpfn = NULL;
	m_bi.lParam = NULL;
	m_bi.pszDisplayName = szPath;

	LPITEMIDLIST pidl = ::SHBrowseForFolder(&m_bi);
	if (pidl)
	{
		if (!::SHGetPathFromIDList(pidl, szPath)) szPath[0] = 0;
		IMalloc* pMalloc = NULL;
		if (SUCCEEDED(::SHGetMalloc(&pMalloc)))	 // 取得IMalloc分配器接口
		{
			pMalloc->Free(pidl); // 释放内存
			pMalloc->Release(); // 释放接口
		}

	}
	return szPath;
}

//获取病毒大小
CString CDia7::GetShowSize(DOUBLE dwSize)
{
	CString szSize;
	if (dwSize<1024)
	{
		//B
		szSize.Format(L"%d B", (DWORD)dwSize);
		return szSize;
	}
	else if (dwSize<1024*1024)
	{
		//KB
		szSize.Format(L"%.2f KB", ((DOUBLE)dwSize) / 1024);
		return szSize;
	}
	else if (dwSize<1024*1024*1024)
	{
		//MB
		szSize.Format(L"%.2f MB", (DOUBLE)dwSize / 1024 / 1024);
	}
	if (dwSize>1024 * 1024 * 1024)
	{
		//GB
		szSize.Format(_T("%.2f GB"), (DOUBLE)dwSize / 1024 / 1024 / 1024);
		return szSize;
	}
	return _T("");
}
//遍历进程并刷新到列表控件
void CDia7::ProcessList()
{
	//包含进程信息的结构体 这个结构体长度在使用前必须设置
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	//创建一个快照 参数一 要获取的快照类型  参数2 当获取模块 堆信息时 需要指定一个进程ID 所以此处无效
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//找到第一个进程的信息 参数1 快照句柄 参数2 进程信息
	if (Process32First(hSnap, &pe) == TRUE)
	{
		//循环遍历其他进程
		int index = 0;
		do
		{
			CString ProcessID;	//进程ID
			CString Priority;	//进程优先级
			//把ID和优先级转成字符串
			ProcessID.Format(L"%d", pe.th32ProcessID);
			Priority.Format(L"%d", pe.pcPriClassBase);
			//如果进程名跟黑名单匹配 就添加到列表 并且保存进程ID到Vecvtor
			for (DWORD i = 0; i < m_LocalWhite.size();i++)
			{
				if (m_LocalWhite[i] == pe.szExeFile)
				{
					//比对成功说明在黑名单里 是病毒
					//把进程名添加到行
					m_ListCtrl.InsertItem(index, pe.szExeFile);
					//把ID添加到行 最后一个参数是图标索引
					m_ListCtrl.SetItemText(index, 1, ProcessID);
					//把ID添加到容器
					m_ProcessId.push_back(ProcessID);
					//把优先级添加进表格内容
					m_ListCtrl.SetItemText(index++, 2, Priority);
					
					//保存到Vector
					VIRINFO VirInfo;
					_tcscpy_s(VirInfo.szVirName, pe.szExeFile);
					_tcscpy_s(VirInfo.szVirPath, ProcessID);
					_tcscpy_s(VirInfo.szVirSize,Priority);
					m_vecVirInfo.push_back(VirInfo);
				}
			}	
		} while (Process32Next(hSnap, &pe));
	}
	//设置扩展属性
}
//白名单查杀
BOOL CDia7::WhiteScan(LPCTSTR szPath)
{
	//判断是本地查杀还是云端查杀
	if (IsLocal)
	{
		//遍历进程
		ProcessList();

	}
	else
	{
		//云端查杀 与m_ServerWhite作对比
		for (DWORD i = 0; i < m_ServerWhite.size(); i++)
		{
			if (m_ServerWhite[i] == szPath)
			{
				//比对成功说明在白名单里 不是病毒
				return FALSE; 
			}
		}
	}
	return TRUE;
}

//全路径查杀
void CDia7::Scam_All(LPCTSTR szPath)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes=0;

	sFindFilter = szPath;
	sFindFilter += TEXT("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
		return;

	do 
	{
		if (_tcscmp(wfd.cFileName, TEXT(".")) == 0 ||
			_tcscmp(wfd.cFileName, TEXT("..")) == 0)
		{
			continue;
		}

		//获取完整的路径名
		sFullPath = szPath;
		sFullPath += TEXT("\\");
		sFullPath += wfd.cFileName;

		//如果当前路径是文件夹 则需要递归文件夹中的文件
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			Scam_All(sFullPath);
		}
		//否则 这是一个文件
		else
		{
			//输出正在搜索的问价路径界面 让用户直观的看到正在扫描的文件路径
			m_szStatus = _T("正在扫描 : ") + sFullPath;
			SendMessage(WM_MYUPDATEDATA, FALSE);

			//判断查杀方式
			if (IsMD5)
			{
				//如果是病毒 将文件信息保存到病毒Vector
				if (MD5Scan(sFullPath))
				{
					//插入List中显示
					m_ListCtrl.InsertItem(0, wfd.cFileName);	//文件名
					m_ListCtrl.SetItemText(0, 1, sFullPath);	//文件路径
					m_ListCtrl.SetItemText(0, 2,				//文件大小
						GetShowSize(wfd.nFileSizeLow));
					//保存病毒信息到Vector容器
					VIRINFO VirInfo;
					_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
					_tcscpy_s(VirInfo.szVirPath, sFullPath);
					_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
					m_vecVirInfo.push_back(VirInfo);

				}
			}
			else
			{
				//白名单查杀
				if (WhiteScan(sFullPath))
				{
					//插入List中显示
					m_ListCtrl.InsertItem(0, wfd.cFileName);	//文件名
					m_ListCtrl.SetItemText(0, 1, sFullPath);	//文件路径
					m_ListCtrl.SetItemText(0, 2,				//文件大小
						GetShowSize(wfd.nFileSizeLow));
					//保存病毒信息到Vector容器
					VIRINFO VirInfo;
					_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
					_tcscpy_s(VirInfo.szVirPath, sFullPath);
					_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
					m_vecVirInfo.push_back(VirInfo);
				}
			}
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
}

//响应清除按钮
void CDia7::OnBnClickedButton8()
{
	if (IsLocal&&!IsMD5)
	{
		//黑名单结束进程
		for (DWORD i = 0; i < m_ProcessId.size(); i++)
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _ttoi(m_ProcessId[i]));
			//结束进程
			TerminateProcess(hProcess, 0);
		}
	}
	else
	{
		//其他删除文件
		//先判断病毒Vector是否为空 若为空 则提示没有可以清除的文件
		if (m_vecVirInfo.size() == 0)
		{
			MessageBox(L"没有可以清除的文件！");
			return;
		}
		//删除病毒
		for (DWORD i = 0; i < m_vecVirInfo.size(); i++)
		{
			DeleteFile(m_vecVirInfo[i].szVirPath);
		}
		m_vecVirInfo.clear();
		
	}
	m_ListCtrl.DeleteAllItems();
	MessageBox(L"清 除 病 毒 成 功 !");
	
}




//初始化和连接scoket
void CDia7::InitSocket()
{
	//初始化winsock库
	WSADATA wa = {};
	int nIsSuccess = WSAStartup(
		MAKEWORD(2, 2),		//版本信息
		&wa					//动态库有关的信息
		);
	if (nIsSuccess != 0)
	{
		MessageBox(L"初始化失败！");
	}

	//生成一个socket
	BOOL bRet = m_Socket.Create();
	//判断是否连接成功
	DWORD dwErr = GetLastError();
	if (!bRet&&WSANOTINITIALISED == dwErr)
	{
		CString str = _T("未调用WSAStartup,或者WSAStartup失败");
		MessageBox(str);
	}
	m_Socket.Connect(L"127.0.0.1", 1234);
}

//更新显示文本
afx_msg LRESULT CDia7::OnMyupdatedata(WPARAM wParam, LPARAM lParam)
{
	return UpdateData(wParam);
}

//提交样本到数据库
void CDia7::OnBnClickedButton2()
{
	//把存储单个文件路径的容器清空
	m_SinglePath.Empty();
	//弹出文件选择框 路径保存在m_SinglePath
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)_TEXT("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		//打开成功 保存文件路径
		m_SinglePath = dlg.GetPathName();
	}
	else
	{
		MessageBox(L"打开文件失败！");
		return;
	}
	if (m_SinglePath.IsEmpty())
	{
		MessageBox(L"路径不能为空!");
		return;
	}

	//根据文件路径 获取文件的MD5值
	char* md5 = md5FileValue(CW2A((LPTSTR)(LPCTSTR)m_SinglePath));

	//向服务端发送消息
	Msg msg = { CommitMessage };

	memcpy(msg.contect.MD5, md5, 36);

	
	//发送消息
	m_Socket.Send((char*)&msg, sizeof(msg));

	//先让用户等待一秒
	m_szStatus = L"正 在 连 接 服 务 器!";
	UpdateData(FALSE);
	Sleep(1000);

	//接收消息
	int nSuccess = 0;		
	m_Socket.Receive(&nSuccess, sizeof(nSuccess));
	
	if (nSuccess)
	{
		MessageBox(L"病毒库更新成功！");
		m_szStatus = L" 病毒库更新成功！";
		UpdateData(FALSE);
	}
}
