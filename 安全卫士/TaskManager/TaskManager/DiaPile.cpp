// DiaPile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "DiaPile.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "Dia1.h"


// CDiaPile �Ի���

IMPLEMENT_DYNAMIC(CDiaPile, CDialogEx)

CDiaPile::CDiaPile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaPile::IDD, pParent)
{

}

CDiaPile::~CDiaPile()
{
}

void CDiaPile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDiaPile, CDialogEx)
END_MESSAGE_MAP()


// CDiaPile ��Ϣ�������

//������
BOOL CDiaPile::PileList(DWORD dwPid)
{
	//�����̶߳ѿ���
	HANDLE hPileSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwPid);

	if (hPileSnap==INVALID_HANDLE_VALUE)
	{
		MessageBox(L"Ȩ�޲��� �޷��鿴�ѣ�����");
		return FALSE;
	}
	//��ʼ������Ϣ�ṹ��
	HEAPLIST32 hl32 = { sizeof(HEAPLIST32) };
	int i = 0;	//�к�
	if (Heap32ListFirst(hPileSnap, &hl32))
	{
		do 
		{
			HEAPENTRY32 he32 = { sizeof(HEAPENTRY32) };
			if (Heap32First(&he32, dwPid,hl32.th32HeapID))
			{
				do 
				{
					//�������������
					CString PileId;		 //��ID
					CString PileSize;	 //�Ѵ�С
					CString PileAddress; //��ʼ��ַ
					CString PileStatus;	 //״̬
					//��ʽ��
					PileId.Format(L"%d", he32.th32HeapID);
					PileSize.Format(L"%d", he32.dwBlockSize);
					PileAddress.Format(L"%d", he32.dwAddress);
					PileStatus.Format(L"%d", he32.dwFlags);
					//��������
					m_ListCtrl.InsertItem(i, PileId);
					m_ListCtrl.SetItemText(i, 1, PileSize);
					m_ListCtrl.SetItemText(i, 2, PileAddress);
					m_ListCtrl.SetItemText(i, 3, PileStatus);
				} while (Heap32Next(&he32));
			}
		} while (Heap32ListNext(hPileSnap,&hl32));
	}
	CloseHandle(hPileSnap);
	return TRUE;
}


BOOL CDiaPile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListCtrl.InsertColumn(0, L"ID", 0, 100);
	m_ListCtrl.InsertColumn(1, L"��С", 0, 100);
	m_ListCtrl.InsertColumn(2, L"��ʼ��ַ", 0, 100);
	m_ListCtrl.InsertColumn(3, L"״̬", 0, 100);

	//�����Ѳ���ʾ���б�
	PileList(_ttoi(CDia1::m_ProcessId));

	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
