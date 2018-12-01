// Dia1.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "Dia1.h"
#include "DiaPile.h"
#include "afxdialogex.h"
#include "DiaThread.h"
#include "DiaModule.h"

CString CDia1::m_ProcessId(" ");
// CDia1 �Ի���

IMPLEMENT_DYNAMIC(CDia1, CDialogEx)

CDia1::CDia1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia1::IDD, pParent)
{

}

CDia1::~CDia1()
{
}

void CDia1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDia1, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDia1::OnRclickList1)
	ON_COMMAND(ID_32783, &CDia1::OnRefresh)
	ON_COMMAND(ID_32781, &CDia1::OnEixt)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDia1::OnClickList1)
	ON_COMMAND(ID_32784, &CDia1::OnThreadList)
	ON_COMMAND(ID_32786, &CDia1::OnLookModule)
	ON_COMMAND(ID_32795, &CDia1::OnLookPile)
END_MESSAGE_MAP()



// CDia1 ��Ϣ�������


BOOL CDia1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HCURSOR m_Cursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1));
	SendMessage(WM_SETCURSOR, (WPARAM)m_Cursor, NULL);
	//���ͼ��
	//����һ����ԱΪ32*32��Сʹ��32λɫ��icon���� ��ʼ�ռ�Ϊ15 ����֮�����1
	ImageList.Create(32, 32, ILC_COLOR32, 15, 1);
	//�õ�ͼ����
	HICON Ico2 = AfxGetApp()->LoadIcon(IDI_ICON2);
	HICON Ico3 = AfxGetApp()->LoadIcon(IDI_ICON3);
	HICON Ico4 = AfxGetApp()->LoadIcon(IDI_ICON4);
	HICON Ico5 = AfxGetApp()->LoadIcon(IDI_ICON5);
	HICON Ico6 = AfxGetApp()->LoadIcon(IDI_ICON6);
	HICON Ico7 = AfxGetApp()->LoadIcon(IDI_ICON7);
	HICON Ico8 = AfxGetApp()->LoadIcon(IDI_ICON8);
	HICON Ico9 = AfxGetApp()->LoadIcon(IDI_ICON9);
	HICON Ico10 = AfxGetApp()->LoadIcon(IDI_ICON10);
	HICON Ico11 = AfxGetApp()->LoadIcon(IDI_ICON11);
	HICON Ico12 = AfxGetApp()->LoadIcon(IDI_ICON12);
	HICON Ico13 = AfxGetApp()->LoadIcon(IDI_ICON13);
	HICON Ico14 = AfxGetApp()->LoadIcon(IDI_ICON14);
	HICON Ico15 = AfxGetApp()->LoadIcon(IDI_ICON15);
	HICON Ico16 = AfxGetApp()->LoadIcon(IDI_ICON16);
	//���ͼ��
	HICON IconArr[15] = { Ico2, Ico3, Ico4, Ico5, Ico6, Ico7, Ico8, Ico9, Ico10, Ico11, Ico12
		, Ico13, Ico14, Ico15, Ico16 };
	for (int i = 0; i < 15; i++)
	{
		ImageList.Add(IconArr[i]);
	}

	//��CImageList������List�ؼ���
	m_ListCtrl.SetImageList(&ImageList, LVSIL_SMALL);                            

	//1 ���б������
	//�����������Ƕ��뷽ʽ ��������LVCFMT_RIGHT LVCFMT_LEFT LVCFMT_CENTER
	m_ListCtrl.InsertColumn(0, L"������", 0, 200);    
	m_ListCtrl.InsertColumn(1, L"ID", 0, 100);
	m_ListCtrl.InsertColumn(2, L"���ȼ�", 0, 100);
	m_ListCtrl.InsertColumn(3, L"������ID", 0, 100);
	m_ListCtrl.InsertColumn(4, L"�����߳���", 0, 100);

	//��������
	ProcessList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}



//�������̲�ˢ�µ��б�
void CDia1::ProcessList()
{
	//����������Ϣ�Ľṹ�� ����ṹ�峤����ʹ��ǰ��������
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	//����һ������ ����һ Ҫ��ȡ�Ŀ�������  ����2 ����ȡģ�� ����Ϣʱ ��Ҫָ��һ������ID ���Դ˴���Ч
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//�ҵ���һ�����̵���Ϣ ����1 ���վ�� ����2 ������Ϣ
	if (Process32First(hSnap, &pe) == TRUE)
	{
		//ѭ��������������
		do
		{
			int i = 0;
			CString ProcessID;	//����ID
			CString Priority;	//�������ȼ�
			CString FatherId;	//������ID
			CString ProcessNum; //�����߳���
			//��ID�����ȼ�ת���ַ���
			ProcessID.Format(L"%d", pe.th32ProcessID);
			Priority.Format(L"%d", pe.pcPriClassBase);
			FatherId.Format(L"%d", pe.th32ParentProcessID);
			ProcessNum.Format(L"%d", pe.cntThreads);

			//�ѽ�������ӵ���
			m_ListCtrl.InsertItem(i, pe.szExeFile, rand() % 15);
			//��ID��ӵ��� ���һ��������ͼ������
			m_ListCtrl.SetItemText(i, 1, ProcessID);
			//�����ȼ���ӽ��������
			m_ListCtrl.SetItemText(i, 2, Priority);
			//�Ѹ�����ID��ӽ�ȥ
			m_ListCtrl.SetItemText(i, 3, FatherId);
			//�����߳���
			m_ListCtrl.SetItemText(i, 4, ProcessNum);


			i++;
		} while (Process32Next(hSnap, &pe));
	}
	//������չ����
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

//��Ӧ�Ҽ������Ϣ
void CDia1::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//�õ���ǰ�еĽ���ID�浽m_ProcessId����
	m_ProcessId = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 1);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMenu m_Menu,*tMenu;
	m_Menu.LoadMenu(IDR_MENU2);

	//�õ���0�������˵�(�˵������кܶ��� ����������õ��ڼ���)
	tMenu = m_Menu.GetSubMenu(0);

	//��ȡ���λ��
	CPoint pt;
	GetCursorPos(&pt);

	//�����˵�
	TrackPopupMenu(tMenu->m_hMenu,
		TPM_LEFTALIGN,
		pt.x, pt.y,	//����
		0,m_hWnd   //���
		,0);
	*pResult = 0;
}


//ˢ�½���
void CDia1::OnRefresh()
{
	// TODO:  �ڴ���������������
	//ɾ��ȫ���˵�
	m_ListCtrl.DeleteAllItems();
	//�ٰѽ���ˢ�³���
	ProcessList();
}

//�˳�����
void CDia1::OnEixt()
{
	//�򿪽����õ����̾��
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _ttoi(m_ProcessId));
	//��������
	TerminateProcess(hProcess, 0);
}

//������List�ؼ�ʱ��Ӧ����Ϣ
void CDia1::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//�õ�����ID������
	m_ProcessId = m_ListCtrl.GetItemText(pNMItemActivate->iItem,1);

	*pResult = 0;
}


//�����鿴���̴߳���
void CDia1::OnThreadList()
{
	//�����̴߳���
	CDiaThread m_DiaThread;
	m_DiaThread.DoModal();
	
}

//�鿴ģ��
void CDia1::OnLookModule()
{
	//���ﲻ���þֲ����� ���򴰿ڻ�һ������ ��Ϊ����������� �����ͱ��ͷ���
	CDiaModule *DiaModule=new CDiaModule;
	//1����ID 2 thisָ��
	DiaModule->Create(IDD_DIALOG9, this);
	DiaModule->ShowWindow(SW_SHOW);
}

//�鿴��
void CDia1::OnLookPile()
{
	CDiaPile *DiaPile = new CDiaPile;
	DiaPile->Create(IDD_DIALOG10, this);
	DiaPile->ShowWindow(SW_SHOW);

}
