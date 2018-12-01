// Dia2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia2.h"
#include "afxdialogex.h"
#include "StartingItems.h"
#include "SoftManage.h"

CListCtrl CDia2::m_ListCtrl = {};
// CDia2 �Ի���

IMPLEMENT_DYNAMIC(CDia2, CDialogEx)

CDia2::CDia2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia2::IDD, pParent)
{

}

CDia2::~CDia2()
{
}

void CDia2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDia2, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDia2::OnRclickList1)
	ON_COMMAND(ID_32787, &CDia2::OnReFresh)
	ON_BN_CLICKED(IDC_BUTTON1, &CDia2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON8, &CDia2::OnBnClickedButton8)
END_MESSAGE_MAP()


// CDia2 ��Ϣ�������

//ö�ٴ��ڵĻص�����
BOOL CALLBACK CDia2::EnumWinProc(HWND hWnd, LPARAM lParam)
{
	//���ݴ��ھ����ȡ������      
	TCHAR WindowsName[200];                                           
	::GetWindowText(hWnd, WindowsName, 200);
	//�жϴ����Ƿ�����
	if (::IsWindowVisible(hWnd)==TRUE&&wcslen(WindowsName)!=0)
	{
		//����û�б������Ҵ��ڱ��ⳤ�Ȳ�Ϊ0 �򽫴�����Ϣ��ʾ��ҳ��
		//�����
		static int i = 0;
		m_ListCtrl.InsertItem(i, WindowsName);
	}
	return TRUE;
}


BOOL CDia2::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//������
	m_ListCtrl.InsertColumn(0, L"������", 0, 200);
	m_ListCtrl.InsertColumn(1, L"���ھ��", 0, 200);
	//ö�ٴ���
	EnumWindows(&EnumWinProc, NULL);
	//������չ����
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDia2::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMenu m_Menu, *tMenu;
	m_Menu.LoadMenu(IDR_MENU2);

	//�õ���0�������˵�(�˵������кܶ��� ����������õ��ڼ���)
	tMenu = m_Menu.GetSubMenu(1);

	//��ȡ���λ��
	CPoint pt;
	GetCursorPos(&pt);

	//�����˵�
	TrackPopupMenu(tMenu->m_hMenu,
		TPM_LEFTALIGN,
		pt.x, pt.y,	//����
		0, m_hWnd   //���
		, 0);
	*pResult = 0;
}

//ˢ�´���
void CDia2::OnReFresh()
{
	//ɾ��ȫ��
	m_ListCtrl.DeleteAllItems();
	//ö�ٴ���
	EnumWindows(&EnumWinProc, NULL);
}


//��������������
void CDia2::OnBnClickedButton1()
{
	CSoftManage* SoftDlg = new CSoftManage;
	SoftDlg->Create(IDD_Soft, this);
	SoftDlg->ShowWindow(SW_SHOW);
}

//��������������
void CDia2::OnBnClickedButton8()
{
	CStartingItems *StatDig = new CStartingItems;
	StatDig->Create(IDD_StartItem, this);
	StatDig->ShowWindow(SW_SHOW);
}
