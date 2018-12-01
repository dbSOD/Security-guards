// Dia5.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia5.h"
#include "afxdialogex.h"
#include <windows.h>
#include "AddressChange.h"
#include "Directory.h"
#include "Section.h"
//保存任务管理器ID的全局变量
DWORD g_TaskMangerPid = 0;
//初始化静态变量

char* CDia5::g_lpBase = nullptr;

IMPLEMENT_DYNAMIC(CDia5, CDialogEx)

CDia5::CDia5(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia5::IDD, pParent)
	, ExtendString(_T(".sys\\.dll\\.exe\\.EXE\\"))
	, m_NumberOfSection(_T(""))
	, m_TimeDateStamp(_T(""))
	, m_SizeOfOptionalHeader(_T(""))
	, m_characteristic(_T(""))
	, m_AddressOfEntryPoint(_T(""))
	, m_SizeOfHeaders(_T(""))
	, m_FileAlignment(_T(""))
	, m_SizeOfImage(_T(""))
	, m_ImageBase(_T(""))
	, m_SectionAlignment(_T(""))
{

}

CDia5::~CDia5()
{
	//释放内存
	delete []g_lpBase;
}

void CDia5::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_NumberOfSection);
	DDX_Text(pDX, IDC_EDIT3, m_TimeDateStamp);
	DDX_Text(pDX, IDC_EDIT2, m_SizeOfOptionalHeader);
	DDX_Text(pDX, IDC_EDIT4, m_characteristic);
	DDX_Text(pDX, IDC_EDIT5, m_AddressOfEntryPoint);
	DDX_Text(pDX, IDC_EDIT6, m_SizeOfHeaders);
	DDX_Text(pDX, IDC_EDIT7, m_FileAlignment);
	DDX_Text(pDX, IDC_EDIT9, m_SizeOfImage);
	DDX_Text(pDX, IDC_EDIT8, m_ImageBase);
	DDX_Text(pDX, IDC_EDIT10, m_SectionAlignment);
}


BEGIN_MESSAGE_MAP(CDia5, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON7, &CDia5::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CDia5::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CDia5::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CDia5::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDia5 消息处理程序

//响应拖拽文件消息
void CDia5::OnDropFiles(HDROP hDropInfo)
{
	//定义一个存储文件完整路径的数组
	WCHAR wcFilePath[MAX_PATH] = {0};

	//获取被拖拽的文件完整的路径名 用于打开文件
	//1 文件句柄 2文件索引标号 从0开始 3 存储文件名的缓冲区首地址 4缓冲区大小 
	DragQueryFile(hDropInfo, 0, wcFilePath, MAX_PATH);

	//判断是否是有效的后缀名
	if (-1 != ExtendString.Find(PathFindExtension(wcFilePath)))
	{
		//打开文件获得句柄
		HANDLE hFile = CreateFile(wcFilePath, GENERIC_ALL, NULL, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile==INVALID_HANDLE_VALUE)
		{
			MessageBox(L"文件打开失败");
			return;
		}

		//获取文件大小
		DWORD dwSize = GetFileSize(hFile, NULL);

		//根据文件大小开辟空间
		g_lpBase = new char[dwSize]{0};

		//读文件
		DWORD dwCount = 1;
		BOOL bIsSucess = ReadFile(hFile, g_lpBase, dwSize, &dwCount, NULL);
		//判断文件是否读取成功
		if (bIsSucess)
		{
			//判断是否的PE文件
			PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)g_lpBase;
			PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + g_lpBase);
			if (pDos->e_magic == IMAGE_DOS_SIGNATURE && pNt->Signature == IMAGE_NT_SIGNATURE)
			{
				//如果是有效的PE文件就对其进行解析
				//对不是CString的字段进行格式转换
				CString sm_NumberOfSection;	//区段数
				sm_NumberOfSection.Format(L"%d", pNt->FileHeader.NumberOfSections);
				CString sm_TimeDateStamp;	//文件创建时间
				sm_TimeDateStamp.Format(L"%08X", pNt->FileHeader.TimeDateStamp);
				CString sm_SizeOfOptionalHeader;	//可选头大小
				sm_SizeOfOptionalHeader.Format(L"%08X", pNt->FileHeader.SizeOfOptionalHeader);
				CString sm_characteristic;	//文件属性
				sm_characteristic.Format(L"%08X",pNt->FileHeader.Characteristics);
				CString sm_AddressOfEntryPoint;	//入口点
				sm_AddressOfEntryPoint.Format(L"%08X", pNt->OptionalHeader.AddressOfEntryPoint);
				CString sm_SizeOfHeaders;	//所有头部的大小
				sm_SizeOfHeaders.Format(L"%08X", pNt->OptionalHeader.SizeOfHeaders);
				CString sm_FileAlignment;	//文件对齐
				sm_FileAlignment.Format(L"%08X",pNt->OptionalHeader.FileAlignment);
				CString sm_SizeOfImage;	//文件装入内存后的总大小
				sm_SizeOfImage.Format(L"%08X", pNt->OptionalHeader.SizeOfImage);
				CString sm_ImageBase;	//默认加载基址
				sm_ImageBase.Format(L"%08X", pNt->OptionalHeader.ImageBase);
				CString sm_SectionAlignment;	//内存对齐
				sm_SectionAlignment.Format(L"%08X", pNt->OptionalHeader.SectionAlignment);

				//把字段显示到GUI上
				m_NumberOfSection = sm_NumberOfSection;
				m_TimeDateStamp = sm_TimeDateStamp;
				m_SizeOfOptionalHeader = sm_SizeOfOptionalHeader;
				m_characteristic = sm_characteristic;
				m_AddressOfEntryPoint = sm_AddressOfEntryPoint;
				m_SizeOfHeaders = sm_SizeOfHeaders;
				m_FileAlignment = sm_FileAlignment;
				m_SizeOfImage = sm_SizeOfImage;
				m_ImageBase = sm_ImageBase;
				m_SectionAlignment = sm_SectionAlignment;

				UpdateData(FALSE);
			}
			else
			{
				MessageBox(L"请拖入有效的PE文件");
			}
		}
		else
		{
			MessageBox(L"文件读取失败");
		}	
	}
	else
	{
		MessageBox(L"请拖入有效的PE文件");
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//响应地址转换按钮功能
void CDia5::OnBnClickedButton7()
{
	CAddressChange *CDia=new CAddressChange;
	CDia->Create(IDD_AddressChange, this);
	CDia->ShowWindow(SW_SHOW);
}

//相应目录按钮
void CDia5::OnBnClickedButton6()
{
	if (g_lpBase)
	{
		CDirectory *Director = new CDirectory;
		Director->Create(IDD_Directory, this);
		Director->ShowWindow(SW_SHOW);
	}
	else
	{
		MessageBox(L"请拖入PE文件");
	}
	
}

//点击弹出区段窗口
void CDia5::OnBnClickedButton4()
{
	CSection *Section = new CSection;
	Section->Create(IDD_Section, this);
	Section->ShowWindow(SW_SHOW);
}


//线程回调函数
DWORD WINAPI Thread(LPVOID lpVoid)
{
	DWORD Pid = 0;
	while (Pid == 0)
	{
		// 2. 获取需要进行注入的进程的PID
		HWND hWnd = FindWindowA(NULL, "任务管理器");
		GetWindowThreadProcessId(hWnd, &Pid);
	}

	g_TaskMangerPid = Pid;
	return 0;
}


BOOL CDia5::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	//创建线程一直找任务管理器的ID
	CreateThread(NULL, 0, Thread, this, 0, NULL);

	return TRUE; 
}



//开启HOOK
void CDia5::OnBnClickedButton2()
{
	//Hook注入
	//DWORD Pid = 0;
	SIZE_T RealWrite = 0;

	// 1. 传递受保护进程的PID
	SavePid();
	
	// 2. 获取需要进行注入的进程的PID
	HWND hWnd = FindWindowA(NULL, "任务管理器");
	GetWindowThreadProcessId(hWnd, &g_TaskMangerPid);

	// 3. 以高权限打开进程的句柄
	HANDLE Process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_TaskMangerPid);

	// 4. 在进程中申请虚拟空间,ProcessAddr在其它进程中，本进程是看不到
	LPVOID ProcessAddr = VirtualAllocEx(Process, NULL,
		MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// 5. 写入数据: DLL的路径
	WriteProcessMemory(Process, ProcessAddr, "C:\\Module.dll",
		strlen("C:\\Module.dll") + 1, &RealWrite);

	// 6. 远程线程注入(拒绝访问[非管理员][64/86])
	HANDLE Thread = CreateRemoteThread(Process, NULL, NULL,
		(LPTHREAD_START_ROUTINE)LoadLibraryA, ProcessAddr, NULL, NULL);

	// 7. 能够使用 LoadLibraryA 进行注入的原因
	//		7.1. 他只有一个参数，可以对应CreateRemoteThread函数
	//		7.2. 在同一台计算机中，所有系统dll的加载基址都是一样
	
	// 8. 等待线程退出
	WaitForSingleObject(Thread, INFINITE);

	MessageBox(L"HOOK开启成功");
}

//保存PID
void CDia5::SavePid()
{
	// 使用信号量保存PID
	DWORD Pid = GetCurrentProcessId();

	// 使用Pid作为初始化的数量创建一个信号量
	CreateSemaphoreA(NULL, Pid, Pid + 1, "MPid");
}