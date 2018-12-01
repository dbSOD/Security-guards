
// TaskManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManagerDlg.h"
#include "afxdialogex.h"
#include <Psapi.h>
#include <powrprof.h>	
#pragma comment(lib, "PowrProf.lib")



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnDropFiles(HDROP hDropInfo);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTaskManagerDlg �Ի���



CTaskManagerDlg::CTaskManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskManagerDlg::IDD, pParent)
{


}

void CTaskManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MyTable);
}

BEGIN_MESSAGE_MAP(CTaskManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTaskManagerDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON5, &CTaskManagerDlg::OnBnClickedButton5)

	ON_COMMAND(ID_32800, &CTaskManagerDlg::OnLockScreen)
	ON_COMMAND(ID_32771, &CTaskManagerDlg::OnShutDown)
	ON_COMMAND(ID_32772, &CTaskManagerDlg::OnRestart)
	ON_COMMAND(ID_32801, &CTaskManagerDlg::OnCancle)
	ON_COMMAND(ID_32799, &CTaskManagerDlg::OnDormancy)
END_MESSAGE_MAP()



//����CPUռ��ʱ��
double CTaskManagerDlg::FILETIME2Double(const _FILETIME& filetime)
{
	return double(filetime.dwHighDateTime*4.294967296e9) + double(filetime.dwLowDateTime);
}



// CTaskManagerDlg ��Ϣ�������

BOOL CTaskManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	::SetCursor(::LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1)));
	SendMessage(WM_SETCURSOR, (WPARAM)m_Cursor, NULL);
	//����ͼ��
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	UpLevel();		//��Ȩ

	//��ȡ�ڴ�ռ����
	//��ʼ���ڴ���Ϣ�Ľṹ�����
	MEMORYSTATUS memStatuas;
	//��ȡϵͳ�ڴ���Ϣ
	GlobalMemoryStatus(&memStatuas);
	DWORD Memory = memStatuas.dwMemoryLoad;


	//����ͳ�ʼ��״̬��
	UINT buf[2] = {1,2};
	m_status.Create(this);

	//��״̬���ָ� ��ÿ����������ID
	m_status.SetIndicators(buf,2);

	//����ָ����ָʾ��������ID ��ʽ ���
	m_status.SetPaneInfo(0, 1, 0, 320);	//����0����������һ����IDΪ1 ��ʽΪ0 ���Ϊ100
	m_status.SetPaneInfo(1, 2, 0, 300);

	//��ʽ���ڴ�ռ���ʺ�CPUռ����
	CString MemoryFootprint;	//�ڴ�ռ����
	
	MemoryFootprint.Format(L"%s:%d", L"�ڴ�ռ����", Memory);

	//�����ڴ�ռ����
	m_status.SetPaneText(0, MemoryFootprint);

	//���¶�λ�͵���״̬����С ͬʱ��ʾ״̬�� ������Ĭ��Ϊ0
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	//���ö�ʱ�� ��ʱˢ��״̬�� 1�����ʱ����ʶ�� 2 ʱ���� 3 �ص�������ַ
	SetTimer(1, 1000, NULL);
	SetTimer(2, 500, NULL);

	//��ʼ��ѡ������Ϣ
	TCHAR Name[7][7] = { L"����", L"����", L"��������", L"�ļ��鿴", L"PE����", L"����", L"������ɱ" };
	for (int i = 0; i < 7;i++)
	{
		m_MyTable.InsertItem(i, Name[i]);
	}

	//���Ӵ���ָ�븳ֵ

	m_MyTable.m_Dia[0] = new CDia1();
	m_MyTable.m_Dia[1] = new CDia2();
	m_MyTable.m_Dia[2] = new CDia3();
	m_MyTable.m_Dia[3] = new CDia4();
	m_MyTable.m_Dia[4] = new CDia5();
	m_MyTable.m_Dia[5] = new CDia6();
	m_MyTable.m_Dia[6] = new CDia7();

	//�����Ӵ���
	UINT DiaIds[7] = { IDD_DIALOG1, IDD_DIALOG2, IDD_DIALOG3, IDD_DIALOG4, IDD_DIALOG5, IDD_DIALOG6, IDD_DIALOG7 };
	for (int i = 0; i < 7; i++)
	{
		m_MyTable.m_Dia[i]->Create(DiaIds[i], &m_MyTable);
	}


	//�����������ڵĴ�С
	CRect rc;
	m_MyTable.GetClientRect(rc);
	rc.DeflateRect(2, 23, 2, 2);
	for (int i = 0; i < 7; i++)
	{
		m_MyTable.m_Dia[i]->MoveWindow(rc);
	}
	
	//��ʾ��һ���Ӵ���
	m_MyTable.m_Dia[0]->ShowWindow(SW_SHOW);
	for (int i = 1; i < 7; i++)
	{
		m_MyTable.m_Dia[i]->ShowWindow(SW_HIDE);
	}

	//ע��ȫ���ȼ�
	:: RegisterHotKey(this->GetSafeHwnd(),0xa001,MOD_CONTROL,'K');

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTaskManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTaskManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTaskManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��Ӧ�˳�����ť����Ϣ
void CTaskManagerDlg::OnBnClickedButton1()
{
	if (IDYES == MessageBox(L"�Ƿ�Ҫ�˳���", L"�˳�", MB_YESNO))
		CTaskManagerDlg::OnOK();	//һ��Ĭ�ϵ��˳���ť
}


void CTaskManagerDlg::OnClose()
{
	//if (IDYES == MessageBox(L"�Ƿ�Ҫ�˳���", L"�˳�", MB_YESNO))
	CDialogEx::OnClose();
}


//��ʱ����Ӧ��Ϣ ��ʱˢ��״̬��
void CTaskManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		//ˢ���ڴ�ռ����
		MEMORYSTATUS memStatuas;
		//��ȡϵͳ�ڴ���Ϣ
		GlobalMemoryStatus(&memStatuas);

		DWORD Memory = memStatuas.dwMemoryLoad;

		CString MemoryFootprint;	//�ڴ�ռ����
		MemoryFootprint.Format(L"%s:%d", L"�ڴ�ռ����", Memory);
		m_status.SetPaneText(0, MemoryFootprint);
	}
	else if (nIDEvent == 2)
	{
		//����CPUռ���� �ڶ�ʱ������ �Ͳ���Ҫ�ȴ� Ҳ����Ҫ���⿪һ���߳�
		//1 ����ʱ�� 2 �ں�ʱ�� 3 �û�ʱ��
		static _FILETIME idleTime = { 0 }, kernelTime = { 0 }, userTime = { 0 };

		//��ȡ�µ�ʱ��
		_FILETIME newIdleTime, newKernelTime, newUserTime;
		GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

		//������ʱ��ת��
		double dOldIdleTime = FILETIME2Double(idleTime);
		double dNewIdleTime = FILETIME2Double(newIdleTime);
		double dOldKernelTime = FILETIME2Double(kernelTime);
		double dNewKernelTime = FILETIME2Double(newKernelTime);
		double dOldUserTime = FILETIME2Double(userTime);
		double dNewUserTime = FILETIME2Double(newUserTime);

		//�����ʹ����
		int time = (int)(100.0 - (dNewIdleTime - dOldIdleTime) /
			(dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);

		idleTime = newIdleTime;
		kernelTime = newKernelTime;
		userTime = newUserTime;

		CString CpuFootprint;
		CpuFootprint.Format(L"%s:%d%%", L"CPUռ����", time);

		//����CPUռ����
		m_status.SetPaneText(1, CpuFootprint);
	}
}


//�ڴ�����
void CTaskManagerDlg::OnBnClickedButton5()
{
	UpdateData(FALSE);
	//��ȡ����ǰ���ڴ���Ϣ
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//��ʼ�����ڴ�
	ClearMemory();
	//��ȡ�������ڴ���Ϣ
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//���������Ϣ
	CString Contect;
	Contect.Format(_T("�ڴ�������ϣ��������� : %.2f MB"), preUsedMem - afterUsedMem);
	MessageBox(Contect);
}

//�����ڴ�
void CTaskManagerDlg::ClearMemory()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);
	int count = 0;

	//��ȡϵͳ�е�һ�����̵���Ϣ  
	BOOL Status = Process32First(hProcessSnap, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			//���ý��̹�������С
			SetProcessWorkingSetSize(hProcess, -1, -1);
			//�����ܶ�Ľ�ָ�����̵�ҳ��ӹ������Ƴ�
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		//��ȡ��һ�����̵���Ϣ  
		Status = Process32Next(hProcessSnap, &ProcessInfo);
	}
}

BOOL CTaskManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	//�ж���ȫ���ȼ�ͬʱҲ�ǵ�ǰע��ļ�
	if (pMsg->message==WM_HOTKEY&&pMsg->wParam==0xa001)
	{
		//���ش��ڴ���
		if (m_IsWindowHide==TRUE)
		{
			ShowWindow(SW_HIDE);
			m_IsWindowHide = FALSE;
		}
		//��ʾ���ڴ���
		else
		{
			ShowWindow(SW_SHOW);
			m_IsWindowHide = TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//����
void CTaskManagerDlg::OnLockScreen()
{
	LockWorkStation();
}

//�ػ�
void CTaskManagerDlg::OnShutDown()
{
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE,NULL);
}

//����
void CTaskManagerDlg::OnRestart()
{
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, NULL);
}

//ע��
void CTaskManagerDlg::OnCancle()
{
	ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, NULL);
}

//����
void CTaskManagerDlg::OnDormancy()
{
	SetSuspendState(TRUE, FALSE, FALSE);
}

//��Ȩ
void CTaskManagerDlg::UpLevel()
{
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}


