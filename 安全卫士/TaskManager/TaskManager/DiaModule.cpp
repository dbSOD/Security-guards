// DiaModule.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "DiaModule.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "Dia1.h"


// CDiaModule �Ի���

IMPLEMENT_DYNAMIC(CDiaModule, CDialogEx)

CDiaModule::CDiaModule(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaModule::IDD, pParent)
{
	
}

CDiaModule::~CDiaModule()
{
}

void CDiaModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDiaModule, CDialogEx)
END_MESSAGE_MAP()


// CDiaModule ��Ϣ�������

//����ģ��
BOOL CDiaModule::ModuleList(DWORD dwPid)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	//��ʼ��ģ����Ϣ�ṹ��
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	//����ģ����� 1 �������� 2 ����ID
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	//��������Ч�ͷ���false
	if (hModuleSnap==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	//ͨ��ģ����վ����ȡ��һ��ģ�����Ϣ
	if (!Module32First(hModuleSnap,&me32))
	{
		//��ȡʧ����رվ��
		CloseHandle(hModuleSnap);
		return FALSE;
	}
	do 
	{	
		int i = 0;	//�к�
		CString ModuleId;	  //ģ��ID
		CString ModuleSize;	  //ģ���С
		CString ModuleCount;  //ģ�����
		//����ת�ַ���
		ModuleId.Format(L"%d", me32.th32ModuleID);
		ModuleSize.Format(L"%d", me32.modBaseSize);
		ModuleCount.Format(L"%d", me32.ProccntUsage);

		//�������ģ����Ϣ��ʾ���б�
		m_ListCtrl.InsertItem(i, ModuleId);				//����ģ��ID
		m_ListCtrl.SetItemText(i, 1, me32.szModule);	//����ģ����
		m_ListCtrl.SetItemText(i, 2, ModuleSize);		//����ģ���С
		m_ListCtrl.SetItemText(i, 3, ModuleCount);		//����ģ�����
		m_ListCtrl.SetItemText(i, 4, me32.szExePath);	//����ģ��·��

	} while (Module32Next(hModuleSnap,&me32));
	return TRUE;
}

//��ʼ������ ����ģ����Ϣ
BOOL CDiaModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//����� 1�к� 2���� 3���뷽ʽ 4�п�
	m_ListCtrl.InsertColumn(0, L"ģ��ID", 0, 80);
	m_ListCtrl.InsertColumn(1, L"ģ����", 0, 110);
	m_ListCtrl.InsertColumn(2, L"ģ���С", 0, 80);
	m_ListCtrl.InsertColumn(3, L"ʹ�ü���", 0, 80);
	m_ListCtrl.InsertColumn(4, L"ģ��·��", 0, 235);

	//����ģ��
	ModuleList(_ttoi(CDia1::m_ProcessId));
	//������չ����
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
