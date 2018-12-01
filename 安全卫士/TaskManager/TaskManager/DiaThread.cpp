// DiaThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "DiaThread.h"
#include "afxdialogex.h"
#include "Dia1.h"

CString CDiaThread::m_ThreadId(" ");
// CDiaThread �Ի���

IMPLEMENT_DYNAMIC(CDiaThread, CDialogEx)

CDiaThread::CDiaThread(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaThread::IDD, pParent)
{

}

CDiaThread::~CDiaThread()
{
}

void CDiaThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDiaThread, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDiaThread::OnRclickList1)
	ON_COMMAND(ID_32792, &CDiaThread::OnStopThread)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDiaThread::OnClickList1)
	ON_COMMAND(ID_32791, &CDiaThread::OnReFreshThread)
	ON_COMMAND(ID_32793, &CDiaThread::OnReStartThread)
	ON_COMMAND(ID_32794, &CDiaThread::OnKillThread)
END_MESSAGE_MAP()


// CDiaThread ��Ϣ�������

void CDiaThread::ThreadsList(DWORD dwPid)	//�����߳�
{

	//��ʼ���߳���Ϣ�Ľṹ��
	THREADENTRY32 te32;
	//������ǰ�����̵߳Ŀ���
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	//�����߳�������� �ṹ���С ��������ʼ�� 
	te32.dwSize = sizeof(THREADENTRY32);
	//��ʼ��ȡ��һ���߳���Ϣ
	Thread32First(hThreadSnap, &te32);
	//ѭ���������е��߳���Ϣ
	do 
	{
		//��������߳̽��й���
		if (te32.th32OwnerProcessID == dwPid)
		{
			//�ٽ��̵߳���Ϣ��ʾ������

			CString ThreadId;	//�߳�ID
			CString Priority;	//�߳����ȼ�
			CString FatherId;	//������ID

			//��ʽת��
			ThreadId.Format(L"%d", te32.th32ThreadID);
			Priority.Format(L"%d", te32.tpBasePri);
			FatherId.Format(L"%d", te32.th32OwnerProcessID);

			int i = 0;
			//��ӽ�ListView
			m_ListCtrl.InsertItem(i, ThreadId);
			m_ListCtrl.SetItemText(i, 1, Priority);
			m_ListCtrl.SetItemText(i, 2, FatherId);
			i++;

		}
	} while (Thread32Next(hThreadSnap,&te32));
}


BOOL CDiaThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//������
	m_ListCtrl.InsertColumn(0, L"�߳�ID", 0, 80);
	m_ListCtrl.InsertColumn(1, L"���ȼ�", 0, 80);
	m_ListCtrl.InsertColumn(2, L"������ID", 0, 80);

	//�õ���ѡ���еĽ���ID Ȼ���ٵ����̴߳���
	DWORD ProcessId = _ttoi(CDia1::m_ProcessId);
	ThreadsList(ProcessId);

	//������չ����
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//����Ҽ����ʱ���̴߳��ڵ����˵�
void CDiaThread::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//���浱ǰѡ���е�ID 
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_ThreadId = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	//ʵ�����˵�����
	CMenu m_Menu, *tMenu;
	//���ز˵�
	m_Menu.LoadMenu(IDR_MENU2);
	//�õ�Ҫ�õĵ�3���˵� �±��0��ʼ
	tMenu = m_Menu.GetSubMenu(2);
	//��ȡ���λ�� �õ����Ĳ˵��������
	CPoint pt;
	GetCursorPos(&pt);
	//�����˵� ����һ�˵���� ��������Ļ�������� �������� ��Ļ���� ���������ھ�� 
	TrackPopupMenu(tMenu->m_hMenu,TPM_LEFTALIGN,pt.x,pt.y,0,m_hWnd,0);

	*pResult = 0;
}

//�����߳�
void CDiaThread::OnStopThread()
{
	//�õ���ǰ�̵߳ľ��
	HANDLE hThread =  OpenThread(PROCESS_ALL_ACCESS,FALSE,_ttoi(m_ThreadId));

	//��ͣ�߳� ��Ҫ���
	SuspendThread(hThread);
}

//��Ӧ���������
void CDiaThread::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//�����������������Ҽ�������õ��߳�ID
	m_ThreadId = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

//ˢ���߳�
void CDiaThread::OnReFreshThread()
{
	//��ɾ�������� �����±���
	m_ListCtrl.DeleteAllItems();
	ThreadsList(_ttoi(CDia1::m_ProcessId));
}

//�ָ��߳�
void CDiaThread::OnReStartThread()
{
	//�õ���ǰ�̵߳ľ��
	HANDLE hThread = OpenThread(PROCESS_ALL_ACCESS, FALSE, _ttoi(m_ThreadId));
	//�ָ��߳� ��Ҫ���
	ResumeThread(hThread);
}

//�����߳�
void CDiaThread::OnKillThread()
{
	//�õ����
	HANDLE hThread = OpenThread(PROCESS_ALL_ACCESS, FALSE, _ttoi(m_ThreadId));
	//�����߳�
	TerminateThread(hThread,-1);
}
