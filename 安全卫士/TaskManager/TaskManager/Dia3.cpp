// Dia3.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia3.h"
#include "afxdialogex.h"

#define ϵͳ���� 0
#define ��������� 1
#define VS���� 2

// CDia3 �Ի���

IMPLEMENT_DYNAMIC(CDia3, CDialogEx)

CDia3::CDia3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia3::IDD, pParent)
	, ExtendString(_T(".obj\\.tlog\\.log\\.pch\\.ilk\\.pdb"))
	, m_Radio(0)
{

}

CDia3::~CDia3()
{
}

void CDia3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, PathList);
	DDX_Control(pDX, IDC_LIST3, WaitDelete);
	DDX_Text(pDX, IDC_EDIT1, ExtendString);
	DDX_Control(pDX, IDC_LIST1, DeleteList);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
}


BEGIN_MESSAGE_MAP(CDia3, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CDia3::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDia3::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDia3::OnBnClickedButton3)
	ON_COMMAND_RANGE(IDC_RADIO1, IDC_RADIO3, &CDia3::OnClickedRadio1)

	ON_BN_CLICKED(IDC_BUTTON4, &CDia3::OnBnClickedButton4)
END_MESSAGE_MAP()


void CDia3::FindFile(CString  Path)
{
	// 1. ���Բ��ҵ�һ���ļ�
	WIN32_FIND_DATA FileInfo = { 0 };
	HANDLE File = FindFirstFile(Path + "\\*", &FileInfo);

	// 2. �����ļ��е������ļ�
	if (File != INVALID_HANDLE_VALUE)
	{
		do {
			// 3.1 ������ļ��о͵ݹ�
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 3.1.1 �ų������ļ���
				if (CString(".") != FileInfo.cFileName && CString("..") != FileInfo.cFileName)
					FindFile(Path + CString("\\") + FileInfo.cFileName);
			}
			// 3.2 ������ļ�
			else
			{
				// 3.2.0 �ۼ��ļ���С�͸���
				++FileCount;
				CountSize.QuadPart += FileInfo.nFileSizeLow;

				// 3.2.1 �жϺ�׺��
				if (-1 != ExtendString.Find(PathFindExtension(FileInfo.cFileName)))
				{
					++DestCount;
					DestSize.QuadPart += FileInfo.nFileSizeLow;
					WaitDelete.InsertItem(0, Path + CString("\\") + FileInfo.cFileName);
					DelStrings.push_back(Path + CString("\\") + FileInfo.cFileName);
				}
			}
		} while (FindNextFile(File, &FileInfo));
	}
}


// CDia3 ��Ϣ�������

//��Ӧ�ļ���ק��Ϣ
void CDia3::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//1 ȡ�ñ���ק���ļ���Ŀ
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	//ѭ����ȡÿһ���ļ�
	for (int i = 0; i < DropCount;i++)
	{
		//�洢�ļ�����·��������
		WCHAR wcStr[MAX_PATH];
		//��ȡÿ����ק�ļ����ļ�����·���ŵ�wcStr����
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);

		//�������һ���ļ��оͼӵ��б���
		if (GetFileAttributes(wcStr)&FILE_ATTRIBUTE_DIRECTORY)
		{
			//�����·�����������б����� 
			if (find(PathStrings.begin(), PathStrings.end(), wcStr) == PathStrings.end())
			{
				//��ӵ��������б���
				PathStrings.push_back(wcStr);
				PathList.InsertItem(0, wcStr);
			}
		}
	}
	//�ϷŽ����� �ͷ��ڴ�
	DragFinish(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}

//��ȡ��ɾ�����ļ���Ϣ
void CDia3::OnBnClickedButton2()
{
	//���������Ϣ
	WaitDelete.DeleteAllItems();
	DelStrings.clear();
	FileCount = DestCount = 0;
	CountSize.QuadPart = DestSize.QuadPart = 0;

	//ɾ��·���е������ļ�
	for (auto Path : PathStrings)
		FindFile(Path);

	// 2. ��ȡ�������Ĵ�С��Ϣ
	CString Msg;
	Msg.Format(L"�����ļ�����: %-3d ��С: %.2lfMB\n��ɾ�ļ�����: %-3d ��С: %.2lfMB",
		FileCount, CountSize.QuadPart / 1024.0 / 1024.0,
		DestCount, DestSize.QuadPart / 1024.0 / 1024.0);

	// 3. �����Ϣ
	MessageBox(Msg);
}

//�����ļ���
void CDia3::OnBnClickedButton1()
{
	UpdateData();
	MessageBox(L"�ļ������³ɹ�!");
}

//�������վ����
void CDia3::CleatRecycleBin()
{
	//��ʼ��SHQUERYRBINFO
	SHQUERYRBINFO RecycleBinInfo = {};
	RecycleBinInfo.cbSize = sizeof(RecycleBinInfo);
	//��ѯ����վ��Ϣ
	SHQueryRecycleBin(NULL, &RecycleBinInfo);
	//��ջ���վ
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

//ɾ���ļ�
void CDia3::OnBnClickedButton3()
{
	DeleteCount = 0;
	// 1. �������е�Ҫɾ�����ļ�·��
	for (auto File : DelStrings)
	{
		// 2. �ж��Ƿ�ɾ���ɹ�
		if (DeleteFile(File))
		{
			++DeleteCount;
			DeleteList.InsertItem(0, File);
		}
	}

	// 3. ��ʾɾ����Ϣ
	CString Msg;
	Msg.Format(L"��ɾ���ļ� %d ��\n��ɾ���ļ� %d ��",
		DestCount, DeleteCount);
	MessageBox(Msg);

	if (m_Radio==VS����)
	{
		
	}
	else if (m_Radio==ϵͳ����)
	{
		//�������վ����
		CleatRecycleBin();
	}
	else if (m_Radio == ���������)
	{

	}
}

//��Ӧ��ѡ��ĵ����ť
void CDia3::OnClickedRadio1(UINT id)
{
	UpdateData(TRUE);
	PathList.DeleteAllItems();
	ExtendString = "  ";
	PathStrings.clear();
	WaitDelete.DeleteAllItems();
	switch (m_Radio)
	{
	case ϵͳ����:
	{
		//����Ĭ��ɾ���ĺ�׺
		ExtendString = (_T(".obj\\.tlog\\.log\\.pch\\.ilk\\.pdb\\.exe"));
		//����Ĭ�������·��
		WCHAR wcPath1[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Temp");
		WCHAR wcPath2[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Microsoft\\Windows\\Explorer");
		WCHAR wcPath3[MAX_PATH] = _T("C:\\Users\\GuiShou\\Documents\\Tencent Files\\873217126\\Image\\Group");
		WCHAR wcPath4[MAX_PATH] = _T("C:\\Users\\GuiShou\\Documents\\WeChat Files\\crt873217126\\Data");
		WCHAR wcPath5[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Netease\\CloudMusic\\webapp");
	
	
		//��ӵ�����·��������
		PathStrings.push_back(wcPath1);
		PathStrings.push_back(wcPath2);
		PathStrings.push_back(wcPath3);
		PathStrings.push_back(wcPath4);
		PathStrings.push_back(wcPath5);
		//��ʾ��List
		PathList.InsertItem(0,L"����վ");
		PathList.InsertItem(1, wcPath1);
		PathList.InsertItem(2, wcPath2);
		PathList.InsertItem(3, wcPath3);
		PathList.InsertItem(4, wcPath4);
		PathList.InsertItem(5, wcPath5);
	}
		break;
	case ���������:
	{
		//����Ĭ�������·��
		WCHAR wcPath1[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Roaming\\SogouExplorer\\Webkit\\Default\\Cache");
		WCHAR wcPath2[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Microsoft\\Windows\\WebCache");
		WCHAR wcPath3[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Packages\\Microsoft.MicrosoftEdge_8wekyb3d8bbwe\\AC\\Temp");

		
		//��ӵ�����·��������
		PathStrings.push_back(wcPath1);
		PathStrings.push_back(wcPath2);
		PathStrings.push_back(wcPath3);
		//��ʾ��List
		PathList.InsertItem(0, wcPath1);
		PathList.InsertItem(1, wcPath2);
		PathList.InsertItem(2, wcPath3);
	}
		break;
	case VS����:
	{
		ExtendString=(_T(".obj\\.tlog\\.lastbuildstate\\.idb\\.pdb\\.pch\\.res\\.ilk\\.exe\\.sdf\\.ipch\\.log\\.db\\.suo\\"));
	}
		break;
	}
	UpdateData(FALSE);
}


BOOL CDia3::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//����Ĭ��ɾ���ĺ�׺
	ExtendString = (_T(".obj\\.tlog\\.log\\.pch\\.ilk\\.pdb\\.exe"));
	//����Ĭ�������·��
	WCHAR wcPath1[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Temp");
	WCHAR wcPath2[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Microsoft\\Windows\\Explorer");
	WCHAR wcPath3[MAX_PATH] = _T("C:\\Users\\GuiShou\\Documents\\Tencent Files\\873217126\\Image\\Group");
	WCHAR wcPath4[MAX_PATH] = _T("C:\\Users\\GuiShou\\Documents\\WeChat Files\\crt873217126\\Data");
	WCHAR wcPath5[MAX_PATH] = _T("C:\\Users\\GuiShou\\AppData\\Local\\Netease\\CloudMusic\\webapp");


	//��ӵ�����·��������
	PathStrings.push_back(wcPath1);
	PathStrings.push_back(wcPath2);
	PathStrings.push_back(wcPath3);
	PathStrings.push_back(wcPath4);
	PathStrings.push_back(wcPath5);
	//��ʾ��List
	PathList.InsertItem(0, L"����վ");
	PathList.InsertItem(1, wcPath1);
	PathList.InsertItem(2, wcPath2);
	PathList.InsertItem(3, wcPath3);
	PathList.InsertItem(4, wcPath4);
	PathList.InsertItem(5, wcPath5);

	//����Աģʽ��ȡ��������ק��Ϣ
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	UpdateData(FALSE);

	return TRUE;  
}

//��Ӧѡ���ļ���ť
void CDia3::OnBnClickedButton4()
{
	wchar_t szPath[MAX_PATH];
	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bInfo;
	bInfo.hwndOwner = m_hWnd;
	bInfo.pidlRoot = NULL;
	bInfo.pszDisplayName = szPath;
	bInfo.lpszTitle = L"��ѡ����Ҫ������ļ��У�";
	bInfo.ulFlags = 0;
	bInfo.lpfn = NULL;
	bInfo.lParam = 0;
	bInfo.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bInfo);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		// ��ȡ����ȷ���ļ���
		PathStrings.push_back(szPath);
		// ��ȡ��ǰ·������
		DWORD dwCount = PathList.GetItemCount();
		PathList.InsertItem(dwCount, szPath);
	}
	else
		AfxMessageBox(L"��Ч���ļ��У�������ѡ��");
}
