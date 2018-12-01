// Dia5.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia5.h"
#include "afxdialogex.h"
#include <windows.h>
#include "AddressChange.h"
#include "Directory.h"
#include "Section.h"
//�������������ID��ȫ�ֱ���
DWORD g_TaskMangerPid = 0;
//��ʼ����̬����

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
	//�ͷ��ڴ�
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


// CDia5 ��Ϣ�������

//��Ӧ��ק�ļ���Ϣ
void CDia5::OnDropFiles(HDROP hDropInfo)
{
	//����һ���洢�ļ�����·��������
	WCHAR wcFilePath[MAX_PATH] = {0};

	//��ȡ����ק���ļ�������·���� ���ڴ��ļ�
	//1 �ļ���� 2�ļ�������� ��0��ʼ 3 �洢�ļ����Ļ������׵�ַ 4��������С 
	DragQueryFile(hDropInfo, 0, wcFilePath, MAX_PATH);

	//�ж��Ƿ�����Ч�ĺ�׺��
	if (-1 != ExtendString.Find(PathFindExtension(wcFilePath)))
	{
		//���ļ���þ��
		HANDLE hFile = CreateFile(wcFilePath, GENERIC_ALL, NULL, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile==INVALID_HANDLE_VALUE)
		{
			MessageBox(L"�ļ���ʧ��");
			return;
		}

		//��ȡ�ļ���С
		DWORD dwSize = GetFileSize(hFile, NULL);

		//�����ļ���С���ٿռ�
		g_lpBase = new char[dwSize]{0};

		//���ļ�
		DWORD dwCount = 1;
		BOOL bIsSucess = ReadFile(hFile, g_lpBase, dwSize, &dwCount, NULL);
		//�ж��ļ��Ƿ��ȡ�ɹ�
		if (bIsSucess)
		{
			//�ж��Ƿ��PE�ļ�
			PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)g_lpBase;
			PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + g_lpBase);
			if (pDos->e_magic == IMAGE_DOS_SIGNATURE && pNt->Signature == IMAGE_NT_SIGNATURE)
			{
				//�������Ч��PE�ļ��Ͷ�����н���
				//�Բ���CString���ֶν��и�ʽת��
				CString sm_NumberOfSection;	//������
				sm_NumberOfSection.Format(L"%d", pNt->FileHeader.NumberOfSections);
				CString sm_TimeDateStamp;	//�ļ�����ʱ��
				sm_TimeDateStamp.Format(L"%08X", pNt->FileHeader.TimeDateStamp);
				CString sm_SizeOfOptionalHeader;	//��ѡͷ��С
				sm_SizeOfOptionalHeader.Format(L"%08X", pNt->FileHeader.SizeOfOptionalHeader);
				CString sm_characteristic;	//�ļ�����
				sm_characteristic.Format(L"%08X",pNt->FileHeader.Characteristics);
				CString sm_AddressOfEntryPoint;	//��ڵ�
				sm_AddressOfEntryPoint.Format(L"%08X", pNt->OptionalHeader.AddressOfEntryPoint);
				CString sm_SizeOfHeaders;	//����ͷ���Ĵ�С
				sm_SizeOfHeaders.Format(L"%08X", pNt->OptionalHeader.SizeOfHeaders);
				CString sm_FileAlignment;	//�ļ�����
				sm_FileAlignment.Format(L"%08X",pNt->OptionalHeader.FileAlignment);
				CString sm_SizeOfImage;	//�ļ�װ���ڴ����ܴ�С
				sm_SizeOfImage.Format(L"%08X", pNt->OptionalHeader.SizeOfImage);
				CString sm_ImageBase;	//Ĭ�ϼ��ػ�ַ
				sm_ImageBase.Format(L"%08X", pNt->OptionalHeader.ImageBase);
				CString sm_SectionAlignment;	//�ڴ����
				sm_SectionAlignment.Format(L"%08X", pNt->OptionalHeader.SectionAlignment);

				//���ֶ���ʾ��GUI��
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
				MessageBox(L"��������Ч��PE�ļ�");
			}
		}
		else
		{
			MessageBox(L"�ļ���ȡʧ��");
		}	
	}
	else
	{
		MessageBox(L"��������Ч��PE�ļ�");
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//��Ӧ��ַת����ť����
void CDia5::OnBnClickedButton7()
{
	CAddressChange *CDia=new CAddressChange;
	CDia->Create(IDD_AddressChange, this);
	CDia->ShowWindow(SW_SHOW);
}

//��ӦĿ¼��ť
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
		MessageBox(L"������PE�ļ�");
	}
	
}

//����������δ���
void CDia5::OnBnClickedButton4()
{
	CSection *Section = new CSection;
	Section->Create(IDD_Section, this);
	Section->ShowWindow(SW_SHOW);
}


//�̻߳ص�����
DWORD WINAPI Thread(LPVOID lpVoid)
{
	DWORD Pid = 0;
	while (Pid == 0)
	{
		// 2. ��ȡ��Ҫ����ע��Ľ��̵�PID
		HWND hWnd = FindWindowA(NULL, "���������");
		GetWindowThreadProcessId(hWnd, &Pid);
	}

	g_TaskMangerPid = Pid;
	return 0;
}


BOOL CDia5::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//����Աģʽ��ȡ��������ק��Ϣ
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	//�����߳�һֱ�������������ID
	CreateThread(NULL, 0, Thread, this, 0, NULL);

	return TRUE; 
}



//����HOOK
void CDia5::OnBnClickedButton2()
{
	//Hookע��
	//DWORD Pid = 0;
	SIZE_T RealWrite = 0;

	// 1. �����ܱ������̵�PID
	SavePid();
	
	// 2. ��ȡ��Ҫ����ע��Ľ��̵�PID
	HWND hWnd = FindWindowA(NULL, "���������");
	GetWindowThreadProcessId(hWnd, &g_TaskMangerPid);

	// 3. �Ը�Ȩ�޴򿪽��̵ľ��
	HANDLE Process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_TaskMangerPid);

	// 4. �ڽ�������������ռ�,ProcessAddr�����������У��������ǿ�����
	LPVOID ProcessAddr = VirtualAllocEx(Process, NULL,
		MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// 5. д������: DLL��·��
	WriteProcessMemory(Process, ProcessAddr, "C:\\Module.dll",
		strlen("C:\\Module.dll") + 1, &RealWrite);

	// 6. Զ���߳�ע��(�ܾ�����[�ǹ���Ա][64/86])
	HANDLE Thread = CreateRemoteThread(Process, NULL, NULL,
		(LPTHREAD_START_ROUTINE)LoadLibraryA, ProcessAddr, NULL, NULL);

	// 7. �ܹ�ʹ�� LoadLibraryA ����ע���ԭ��
	//		7.1. ��ֻ��һ�����������Զ�ӦCreateRemoteThread����
	//		7.2. ��ͬһ̨������У�����ϵͳdll�ļ��ػ�ַ����һ��
	
	// 8. �ȴ��߳��˳�
	WaitForSingleObject(Thread, INFINITE);

	MessageBox(L"HOOK�����ɹ�");
}

//����PID
void CDia5::SavePid()
{
	// ʹ���ź�������PID
	DWORD Pid = GetCurrentProcessId();

	// ʹ��Pid��Ϊ��ʼ������������һ���ź���
	CreateSemaphoreA(NULL, Pid, Pid + 1, "MPid");
}