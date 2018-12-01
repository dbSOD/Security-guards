// Disk.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Disk.h"
#include "afxdialogex.h"


// CDisk �Ի���
LPCTSTR TypeName[7] = {
	TEXT("δ֪�豸"),
	TEXT("��Ч��·��"),
	TEXT("���ƶ��豸"),
	TEXT("�����ƶ��豸"),
	TEXT("����������"),
	TEXT("CD-ROM ������"),
	TEXT("����洢��"),
};


IMPLEMENT_DYNAMIC(CDisk, CDialogEx)

CDisk::CDisk(LPCTSTR name, CWnd* pParent /*=NULL*/)
: CDialogEx(CDisk::IDD, pParent), m_path(name)
, m_Disk(_T(""))
, m_DiskName(_T(""))
, m_System(_T(""))
, m_FreeSpace(_T(""))
, m_TotalSpace(_T(""))
, m_Info(_T(""))
{

}

CDisk::~CDisk()
{
}

void CDisk::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Disk);
	DDX_Text(pDX, IDC_EDIT15, m_DiskName);
	DDX_Text(pDX, IDC_EDIT16, m_System);
	DDX_Text(pDX, IDC_EDIT17, m_FreeSpace);
	DDX_Text(pDX, IDC_EDIT11, m_TotalSpace);
	DDX_Text(pDX, IDC_EDIT2, m_Info);
}


BEGIN_MESSAGE_MAP(CDisk, CDialogEx)
END_MESSAGE_MAP()


// CDisk ��Ϣ�������


BOOL CDisk::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Disk = m_path;
	UINT type = GetDriveType(m_path);
	m_Info = TypeName[type];
	TCHAR Name[MAX_PATH] = { 0 }, System[MAX_PATH] = { 0 };
	GetVolumeInformation(m_path, Name, MAX_PATH, NULL,
		NULL, NULL, System, MAX_PATH);
	m_DiskName = Name;
	m_DiskName = L"DATA";

	m_System = System;
	DWORD a = 0, b = 0, c = 0, d = 0;
	GetDiskFreeSpace(m_path, &a, &b, &c, &d);
	double FreeSize = (c / 1024.0) * a * b / 1024 / 1024;
	double TotalSize = (d / 1024.0) * a * b / 1024 / 1024;
	m_FreeSpace.Format(L"%.2lfGB", FreeSize);
	m_TotalSpace.Format(L"%.2lfGB", TotalSize);


	UpdateData(FALSE);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
