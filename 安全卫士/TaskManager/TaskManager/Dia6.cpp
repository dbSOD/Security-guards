// Dia6.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia6.h"
#include "afxdialogex.h"
#include <windows.h>
#include <winsvc.h>

//��ʼ���������ID���ַ���
CString CDia6::m_ServiceId(" ");

// CDia6 �Ի���

IMPLEMENT_DYNAMIC(CDia6, CDialogEx)

CDia6::CDia6(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia6::IDD, pParent)
{

}

CDia6::~CDia6()
{
}

void CDia6::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDia6, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDia6::OnRclickList1)
	ON_COMMAND(ID_32796, &CDia6::OnStartService)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDia6::OnClickList1)
	ON_COMMAND(ID_32797, &CDia6::OnFlushService)
END_MESSAGE_MAP()


// CDia6 ��Ϣ�������

void CDia6::ShowServices()
{
	//׼����Ҫ�õ�������
	CString Contect;	//����
	//����״̬
	WCHAR* wcStatus[] = { L"��ֹͣ", L"��������", L"����ֹͣ", L"��������", L"���ڼ���", L"������ͣ", L"����ͣ" };
	//��������
	WCHAR* wcType[] = { L"ϵͳ����", L"��ʼ������", L"�Զ�", L"�ֶ�", L"����" };
	
	//��Զ�̼����������ƹ�����
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(L"�򿪷��������ʧ�ܣ�");
		return;
	}
	//��һ�ε��� ��ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);

	//������Ҫ���ڴ� �ڶ��ε���
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerivice = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];

	//�ڶ���ö��
	BOOL bStatus = FALSE;
	bStatus = EnumServicesStatusEx(
		hSCM,					//SCM���ݿ���
		SC_ENUM_PROCESS_INFO,	//Ҫ���ص�����
		SERVICE_WIN32,			//��������
		SERVICE_STATE_ALL,		//����״̬
		(PBYTE)pEnumSerivice,	//������Ϣ������
		dwSize,					//��������С
		&dwSize,				//��Ҫ�Ĵ�С
		&dwServiceNum,			//�������еķ������
		NULL,
		NULL
		);
	//������Ϣ
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//��ȡ������Ϣ 
		//������
		m_ListCtrl.InsertItem(i, pEnumSerivice[i].lpServiceName, 0);
		//����״̬ ����ֹͣ �������� ������ͣ
		m_ListCtrl.SetItemText(i, 3, wcStatus[pEnumSerivice[i].ServiceStatusProcess.dwCurrentState - 1]);
		//PID
		Contect.Format(L"%d", pEnumSerivice[i].ServiceStatusProcess.dwProcessId);
		m_ListCtrl.SetItemText(i, 1, Contect);
		//����
		m_ListCtrl.SetItemText(i, 2, pEnumSerivice[i].lpDisplayName);
		//��ȡ������ϸ��Ϣ
		//�򿪷���
		SC_HANDLE hService = OpenService(hSCM, pEnumSerivice[i].lpServiceName, SERVICE_QUERY_CONFIG);
		//��һ�ε��û�ȡ��Ҫ�Ļ�������С
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		//�ڶ��ε��� ��ȡ��Ϣ
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//��ȡ��������
		m_ListCtrl.SetItemText(i, 4, wcType[pServiceConfig->dwStartType]);
	
	}
}



BOOL CDia6::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ʼ��ListCtrl
	WCHAR* wcArr[] = { L"����", L"PID", L"����", L"״̬", L"����" };
	DWORD len = sizeof(wcArr) / sizeof(wcArr[0]);
	for (DWORD i = 0; i < len; i++)
	{
		m_ListCtrl.InsertColumn(i, wcArr[i], 0, 120);
	}
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	//���ͼ��
	//����һ����ԱΪ32*32��Сʹ��32λɫ��icon���� ��ʼ�ռ�Ϊ15 ����֮�����1
	ImageList.Create(32, 32, ILC_COLOR32, 1, 1);

	//�õ�ͼ����
	HICON Ico = AfxGetApp()->LoadIcon(IDI_ICON17);
	//���ͼ��
	HICON IconArr[] = { Ico };
	for (int i = 0; i < 1; i++)
	{
		ImageList.Add(IconArr[i]);
	}
	//��CImageList������List�ؼ���
	m_ListCtrl.SetImageList(&ImageList, LVSIL_SMALL);

	//��������
	ShowServices();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//��Ӧ�Ҽ������Ϣ ��������˵�
void CDia6::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//�õ������һ�еķ�����������
	m_ServiceId = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);

	CMenu Menu, *pMenu;
	//���������˵�
	Menu.LoadMenu(IDR_MENU2);
	//�õ��˵��ĵ�����
	pMenu = Menu.GetSubMenu(3);
	//��ȡ���λ��
	CPoint pt;
	GetCursorPos(&pt);
	//�����˵�
	TrackPopupMenu(pMenu->m_hMenu,TPM_LEFTALIGN,pt.x,pt.y,0,m_hWnd,0);

	*pResult = 0;
}

//������رշ���
void CDia6::OnStartService()
{
	//�򿪷���������
	SC_HANDLE hSC = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSC==NULL)
	{
		MessageBox(L"�򿪷��������ʧ�ܣ�");
		return;
	}
	//��ѡ����һ�еķ���
	SC_HANDLE hSerivice = OpenService(hSC, m_ServiceId,SERVICE_START|SERVICE_QUERY_STATUS|SERVICE_STOP);
	if (hSerivice == NULL)
	{
		MessageBox(L"�����ʧ��");
		CloseServiceHandle(hSerivice);
		return;
	}
	//��÷���״̬
	SERVICE_STATUS status;
	if (QueryServiceStatus(hSerivice, &status)==FALSE)
	{
		MessageBox(L"��ȡ����״̬ʧ�ܣ�");
		CloseServiceHandle(hSerivice);
		CloseServiceHandle(hSC);
		return;
	}
	//��������������йرշ���
	if (status.dwCurrentState==SERVICE_RUNNING)
	{
		//���Ϳ�����
		if (ControlService(hSerivice,SERVICE_CONTROL_STOP,&status)==FALSE)
		{
			MessageBox(L"ֹͣ����ʧ�ܣ�");
			CloseServiceHandle(hSerivice);
			CloseServiceHandle(hSC);
			return;
		}
		//�ȴ�����ֹͣ
		while (QueryServiceStatus(hSerivice,&status)==TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState==SERVICE_STOPPED)
			{
				MessageBox(L"ֹͣ����ɹ���");
				CloseServiceHandle(hSerivice);
				CloseServiceHandle(hSC);
				ShowServices();	//ˢ��
				return;
			}
		}
	}
	//���������ֹͣ�Ϳ�������
	else if (status.dwCurrentState==SERVICE_STOPPED)
	{
		//��������
		if (StartService(hSerivice,NULL,NULL)==FALSE)
		{
			MessageBox(L"��������ʧ�ܣ�");
			CloseServiceHandle(hSerivice);
			CloseServiceHandle(hSC);
			return;
		}
		//�ȴ���������
		while (QueryServiceStatus(hSerivice,&status)==TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState==SERVICE_RUNNING)
			{
				MessageBox(L"��������ɹ���");
				CloseServiceHandle(hSerivice);
				CloseServiceHandle(hSC);
				ShowServices();	//ˢ��
				return;
			}
		}
		//ѭ��������˵����������ʧ��
		MessageBox(L"��������ʧ�ܣ�");
		CloseServiceHandle(hSerivice); 
		CloseServiceHandle(hSC);
		return;
	}

}

//��Ӧ����ĵ����Ϣ
void CDia6::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//�õ�������������
	m_ServiceId = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	*pResult = 0;
}

//ˢ�·���
void CDia6::OnFlushService()
{
	//��������
	ShowServices();
}
