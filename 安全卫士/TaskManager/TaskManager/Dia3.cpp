// Dia3.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia3.h"
#include "afxdialogex.h"

#define 系统垃圾 0
#define 浏览器垃圾 1
#define VS垃圾 2

// CDia3 对话框

IMPLEMENT_DYNAMIC(CDia3, CDialogEx)

CDia3::CDia3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia3::IDD, pParent)
	, ExtendString(_T(".obj\\.tlog\\.log\\.pch\\.ilk\\.pdb"))
	, m_Radio(0)
{

}

CDia3::~CDia3()
{
}

void CDia3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, PathList);
	DDX_Control(pDX, IDC_LIST3, WaitDelete);
	DDX_Text(pDX, IDC_EDIT1, ExtendString);
	DDX_Control(pDX, IDC_LIST1, DeleteList);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
}


BEGIN_MESSAGE_MAP(CDia3, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CDia3::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDia3::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDia3::OnBnClickedButton3)
	ON_COMMAND_RANGE(IDC_RADIO1, IDC_RADIO3, &CDia3::OnClickedRadio1)

	ON_BN_CLICKED(IDC_BUTTON4, &CDia3::OnBnClickedButton4)
END_MESSAGE_MAP()


void CDia3::FindFile(CString  Path)
{
	// 1. 尝试查找第一个文件
	WIN32_FIND_DATA FileInfo = { 0 };
	HANDLE File = FindFirstFile(Path + "\\*", &FileInfo);

	// 2. 遍历文件夹的所有文件
	if (File != INVALID_HANDLE_VALUE)
	{
		do {
			// 3.1 如果是文件夹就递归
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 3.1.1 排除两个文件夹
				if (CString(".") != FileInfo.cFileName && CString("..") != FileInfo.cFileName)
					FindFile(Path + CString("\\") + FileInfo.cFileName);
			}
			// 3.2 如果是文件
			else
			{
				// 3.2.0 累加文件大小和个数
				++FileCount;
				CountSize.QuadPart += FileInfo.nFileSizeLow;

				// 3.2.1 判断后缀名
				if (-1 != ExtendString.Find(PathFindExtension(FileInfo.cFileName)))
				{
					++DestCount;
					DestSize.QuadPart += FileInfo.nFileSizeLow;
					WaitDelete.InsertItem(0, Path + CString("\\") + FileInfo.cFileName);
					DelStrings.push_back(Path + CString("\\") + FileInfo.cFileName);
				}
			}
		} while (FindNextFile(File, &FileInfo));
	}
}


// CDia3 消息处理程序

//响应文件拖拽消息
void CDia3::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//1 取得被拖拽的文件数目
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	//循环获取每一个文件
	for (int i = 0; i < DropCount;i++)
	{
		//存储文件完整路径的数组
		WCHAR wcStr[MAX_PATH];
		//获取每个拖拽文件的文件完整路径放到wcStr里面
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);

		//如果这是一个文件夹就加到列表里
		if (GetFileAttributes(wcStr)&FILE_ATTRIBUTE_DIRECTORY)
		{
			//如果该路径不存在于列表才添加 
			if (find(PathStrings.begin(), PathStrings.end(), wcStr) == PathStrings.end())
			{
				//添加到容器和列表里
				PathStrings.push_back(wcStr);
				PathList.InsertItem(0, wcStr);
			}
		}
	}
	//拖放结束后 释放内存
	DragFinish(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}

//获取待删除的文件信息
void CDia3::OnBnClickedButton2()
{
	//清空所有信息
	WaitDelete.DeleteAllItems();
	DelStrings.clear();
	FileCount = DestCount = 0;
	CountSize.QuadPart = DestSize.QuadPart = 0;

	//删除路径中的所有文件
	for (auto Path : PathStrings)
		FindFile(Path);

	// 2. 获取遍历到的大小信息
	CString Msg;
	Msg.Format(L"所有文件个数: %-3d 大小: %.2lfMB\n可删文件个数: %-3d 大小: %.2lfMB",
		FileCount, CountSize.QuadPart / 1024.0 / 1024.0,
		DestCount, DestSize.QuadPart / 1024.0 / 1024.0);

	// 3. 输出信息
	MessageBox(Msg);
}

//更新文件名
void CDia3::OnBnClickedButton1()
{
	UpdateData();
	MessageBox(L"文件名更新成功!");
}

//清理回收站垃圾
void CDia3::CleatRecycleBin()
{
	//初始化SHQUERYRBINFO
	SHQUERYRBINFO RecycleBinInfo = {};
	RecycleBinInfo.cbSize = sizeof(RecycleBinInfo);
	//查询回收站信息
	SHQueryRecycleBin(NULL, &RecycleBinInfo);
	//清空回收站
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

//删除文件
void CDia3::OnBnClickedButton3()
{
	DeleteCount = 0;
	// 1. 遍历所有的要删除的文件路径
	for (auto File : DelStrings)
	{
		// 2. 判断是否删除成功
		if (DeleteFile(File))
		{
			++DeleteCount;
			DeleteList.InsertItem(0, File);
		}
	}

	// 3. 提示删除信息
	CString Msg;
	Msg.Format(L"需删除文件 %d 个\n已删除文件 %d 个",
		DestCount, DeleteCount);
	MessageBox(Msg);

	if (m_Radio==VS垃圾)
	{
		
	}
	else if (m_Radio==系统垃圾)
	{
		//清理回收站垃圾
		CleatRecycleBin();
	}
	else if (m_Radio == 浏览器垃圾)
	{

	}
}

//响应单选框的点击按钮
void CDia3::OnClickedRadio1(UINT id)
{
	UpdateData(TRUE);
	PathList.DeleteAllItems();
	ExtendString = "  ";
	PathStrings.clear();
	WaitDelete.DeleteAllItems();
	switch (m_Radio)
	{
	case 系统垃圾:
	{
		//设置默认删除的后缀
		ExtendString = (_T(".obj\\.tlog\\.log\\.pch\\.ilk\\.pdb\\.exe"));
		//设置默认清理的路径
		WCHAR wcPath1[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Temp");
		WCHAR wcPath2[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Microsoft\\Windows\\Explorer");
		WCHAR wcPath3[MAX_PATH] = _T("C:\\Users\\GuiShou\\Documents\\Tencent Files\\873217126\\Image\\Group");
		WCHAR wcPath4[MAX_PATH] = _T("C:\\Users\\GuiShou\\Documents\\WeChat Files\\crt873217126\\Data");
		WCHAR wcPath5[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Netease\\CloudMusic\\webapp");
	
	
		//添加到保存路径的容器
		PathStrings.push_back(wcPath1);
		PathStrings.push_back(wcPath2);
		PathStrings.push_back(wcPath3);
		PathStrings.push_back(wcPath4);
		PathStrings.push_back(wcPath5);
		//显示到List
		PathList.InsertItem(0,L"回收站");
		PathList.InsertItem(1, wcPath1);
		PathList.InsertItem(2, wcPath2);
		PathList.InsertItem(3, wcPath3);
		PathList.InsertItem(4, wcPath4);
		PathList.InsertItem(5, wcPath5);
	}
		break;
	case 浏览器垃圾:
	{
		//设置默认清理的路径
		WCHAR wcPath1[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Roaming\\SogouExplorer\\Webkit\\Default\\Cache");
		WCHAR wcPath2[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Microsoft\\Windows\\WebCache");
		WCHAR wcPath3[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Packages\\Microsoft.MicrosoftEdge_8wekyb3d8bbwe\\AC\\Temp");

		
		//添加到保存路径的容器
		PathStrings.push_back(wcPath1);
		PathStrings.push_back(wcPath2);
		PathStrings.push_back(wcPath3);
		//显示到List
		PathList.InsertItem(0, wcPath1);
		PathList.InsertItem(1, wcPath2);
		PathList.InsertItem(2, wcPath3);
	}
		break;
	case VS垃圾:
	{
		ExtendString=(_T(".obj\\.tlog\\.lastbuildstate\\.idb\\.pdb\\.pch\\.res\\.ilk\\.exe\\.sdf\\.ipch\\.log\\.db\\.suo\\"));
	}
		break;
	}
	UpdateData(FALSE);
}


BOOL CDia3::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//设置默认删除的后缀
	ExtendString = (_T(".obj\\.tlog\\.log\\.pch\\.ilk\\.pdb\\.exe"));
	//设置默认清理的路径
	WCHAR wcPath1[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Temp");
	WCHAR wcPath2[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Microsoft\\Windows\\Explorer");
	WCHAR wcPath3[MAX_PATH] = _T("C:\\Users\\GuiShou\\Documents\\Tencent Files\\873217126\\Image\\Group");
	WCHAR wcPath4[MAX_PATH] = _T("C:\\Users\\GuiShou\\Documents\\WeChat Files\\crt873217126\\Data");
	WCHAR wcPath5[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Netease\\CloudMusic\\webapp");


	//添加到保存路径的容器
	PathStrings.push_back(wcPath1);
	PathStrings.push_back(wcPath2);
	PathStrings.push_back(wcPath3);
	PathStrings.push_back(wcPath4);
	PathStrings.push_back(wcPath5);
	//显示到List
	PathList.InsertItem(0, L"回收站");
	PathList.InsertItem(1, wcPath1);
	PathList.InsertItem(2, wcPath2);
	PathList.InsertItem(3, wcPath3);
	PathList.InsertItem(4, wcPath4);
	PathList.InsertItem(5, wcPath5);

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	UpdateData(FALSE);

	return TRUE;  
}

//响应选择文件按钮
void CDia3::OnBnClickedButton4()
{
	wchar_t szPath[MAX_PATH];
	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bInfo;
	bInfo.hwndOwner = m_hWnd;
	bInfo.pidlRoot = NULL;
	bInfo.pszDisplayName = szPath;
	bInfo.lpszTitle = L"请选择需要清理的文件夹：";
	bInfo.ulFlags = 0;
	bInfo.lpfn = NULL;
	bInfo.lParam = 0;
	bInfo.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bInfo);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		// 获取到正确的文件夹
		PathStrings.push_back(szPath);
		// 获取当前路径个数
		DWORD dwCount = PathList.GetItemCount();
		PathList.InsertItem(dwCount, szPath);
	}
	else
		AfxMessageBox(L"无效的文件夹，请重新选择");
}
