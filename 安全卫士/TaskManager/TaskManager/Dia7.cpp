// Dia7.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Dia7.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <atlstr.h>


// CDia7 �Ի���

IMPLEMENT_DYNAMIC(CDia7, CDialogEx)

CDia7::CDia7(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia7::IDD, pParent)

	, m_szStatus(_T(""))
{
	
}

CDia7::~CDia7()
{
}

void CDia7::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Text(pDX, IDC_STATIC1, m_szStatus);
}


BEGIN_MESSAGE_MAP(CDia7, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON9, &CDia7::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON8, &CDia7::OnBnClickedButton8)
	ON_MESSAGE(WM_MYUPDATEDATA, &CDia7::OnMyupdatedata)
	ON_BN_CLICKED(IDC_BUTTON2, &CDia7::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDia7 ��Ϣ�������



BOOL CDia7::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//����Աģʽ��ȡ��������ק��Ϣ
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	//��ʼ��List
	m_ListCtrl.InsertColumn(0, _T("������/�ļ���"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("����ID/·��"), 0, 190);
	m_ListCtrl.InsertColumn(2, _T("���ȼ�/��С"), 0, 100);
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//��ʼ������socket
	InitSocket();
	return TRUE;  
}

//��ʼ��ɨ��ѡ��
void CDia7::InitSelect()
{
	if (IsSelect[0])
	{
		//���ļ���ɱ
		IsSingleScan = TRUE;
	}
	else
	{
		//ȫ·����ɱ
		IsSingleScan = FALSE;
	}
	if (IsSelect[2])
	{
		//MD5��ɱ
		IsMD5 = TRUE;
	}
	else
	{
		//��������ɱ
		IsMD5 = FALSE;
	}
	if (IsSelect[4])
	{
		//���ز�ɱ
		IsLocal = TRUE;
	}
	else
	{
		//�ƶ˲�ɱ
		IsLocal = FALSE;
	}
}


//�̻߳ص�����
DWORD WINAPI ScanThread(LPVOID lpVoid)
{
	CDia7* pScanCtrl = (CDia7*)lpVoid;
	//��ɱ·���ж�
	if (pScanCtrl->IsSingleScan)
	{
		//���ļ���ɱ
		pScanCtrl->Scan_Single(pScanCtrl->m_SinglePath);
	}
	else
	{
		//ȫ·����ɱ
		//����·��ɨ�� ����·������
		pScanCtrl->Scam_All(pScanCtrl->m_AllPath);
	}

	pScanCtrl->m_szStatus.Format(_T("ɨ����ϣ����η��ֲ���:%d"), pScanCtrl->m_vecVirInfo.size());
	pScanCtrl->SendMessage(WM_MYUPDATEDATA, FALSE);
	return 0;

}

//��ʼɨ��
void CDia7::StartScan()
{
	//1 ���ж�ɨ��·��
	if (IsSingleScan&&IsMD5)
	{
		m_SinglePath.Empty();
		//���Ϊ�����ļ���ɱ �����ļ�ѡ��� ·��������m_SinglePath
		CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),NULL);
		if (dlg.DoModal()==IDOK)
		{
			//�򿪳ɹ� �����ļ�·��
			m_SinglePath = dlg.GetPathName();
		}
		else
		{
			MessageBox(L"���ļ�ʧ�ܣ�");
			return;
		}
		if (m_SinglePath.IsEmpty())
		{
			MessageBox(L"·������Ϊ��!");
			return;
		}
	}
	else
	{
		if (IsMD5)
		{
			//���Ϊȫ·����ɱ �����ļ���ѡ��� ·��������m_AllPath
			m_AllPath.Empty();
			m_AllPath = BrowseFolder(this->m_hWnd, _T("��ѡ���ɱ·��"));
			if (m_AllPath.IsEmpty())
			{
				MessageBox(L"��ɱ·������Ϊ�գ�");
				return;
			}
		}
		
	}
	m_ListCtrl.DeleteAllItems();
	//2 �ж��Ǳ��ػ����ƶ�
	if (IsLocal)
	{
		//����Ǳ��� ���ж���MD5���ǰ����� 
		if (IsMD5)
		{
			//����MD5 ���ر���MD5������
			//�Ȼ�ȡ��������MD5�ĸ���
			CString Temp;
			//��ȡ.ini�ļ�
			GetPrivateProfileString(
				_T("Num"),						 //INI�ļ��е�һ���ֶ���
				_T("MD5"), 						 //lpAppName �µ�һ��������Ҳ�����������ı�����
				_T("DefaultName"),				 //ָ������Ŀû���ҵ�ʱ���ص�Ĭ��ֵ������Ϊ�գ�""��
				Temp.GetBuffer(MAX_PATH),		 //����INI�ļ��е�ֵ��CString���󣬼����ջ�����
				MAX_PATH, 						 //���ջ������Ĵ�С
				_T(".\\LocalVirBase\\Vir.ini")	 //������INI�ļ�·����
				);
			Temp.ReleaseBuffer();
			int n = _ttoi(Temp);
			//�ٸ��ݸ�������MD5��Ϣ ���ص�Vector
			for (int i = 0; i < n; i++)
			{
				CString tempCount;
				tempCount.Format(_T("%d"), i);
				GetPrivateProfileString(_T("MD5"), tempCount, _T("DefaultName"), Temp.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\LocalVirBase\\Vir.ini"));
				Temp.ReleaseBuffer();
				m_LocalMD5.push_back(Temp);
			}
			if (m_LocalMD5.size() == 0)
			{
				MessageBox(L"��ȡ���ز�����ʧ��");
				return;
			}
		}
		else
		{
			//��������ɱ ���ر��ذ�����������
			//�Ȼ�ȡ�������а������ĸ���
			CString Temp;
			GetPrivateProfileString(_T("Num"), _T("White"), _T("DefaultName"), Temp.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\LocalVirBase\\Vir.ini"));
			Temp.ReleaseBuffer();
			int n = _ttoi(Temp);
			//�ٸ��ݸ�������MD5��Ϣ ���ص�Vector��
			for (int i = 0; i < n; i++)
			{
				CString tempCount;
				tempCount.Format(L"%d", i);
				GetPrivateProfileString(_T("White"),
					tempCount, _T("DefaultName"),
					Temp.GetBuffer(MAX_PATH), MAX_PATH,
					_T(".\\LocalVirBase\\Vir.ini"));
				Temp.ReleaseBuffer();
				m_LocalWhite.push_back(Temp);
			}
			if (m_LocalWhite.size()==0)
			{
				MessageBox(L"��ȡ������ʧ��!");
				return;
			}
		}
	}
	else
	{
		//������ƶ� ���ж���MD5���ǰ������������ƶ˲�������Ϣ
		if (IsMD5)
		{
//--------------------------�ƶ�MD5��ɱ �����ƶ�MD5������---------------------------------------------------
			//������Ϣ
			Msg msg = { Md5Message };
			m_Socket.Send((char*)&msg, sizeof(msg));

			//�����û��ȴ�һ��
			m_szStatus = L"�� �� �� �� �� �� ��!";
			UpdateData(FALSE);
			Sleep(1000);

			//ѭ��������Ϣ
			char szMD5[36];		//��Ϣ����
			int nNumber=0;		//��Ϣ����
			m_Socket.Receive(&nNumber, sizeof(nNumber));
			for (int i = 0; i < nNumber; i++)
			{
				m_Socket.Receive(szMD5, 36);
				m_ServerMD5.push_back((CString)szMD5);
			}		
		}
		else
		{
			//�ƶ˰�������ɱ 
		}
	}


	//3 ����ɨ���߳� ��ʼɨ�財�� ��ֹ�������߳�
	CloseHandle(CreateThread(NULL, 0, ScanThread, this, 0, NULL));

}



//�����ļ���ɱ
void CDia7::Scan_Single(LPCTSTR szPath)
{
	WIN32_FIND_DATA wfd;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	FindFirstFile(sFindFilter,&wfd);

	//�ж���MD5��ɱ���ǰ�������ɱ
	if (IsMD5)
	{
		//��MD5��ɱ �ж��Ƿ��ǲ���
		if (MD5Scan(szPath))
		{
			//���뵽List
			m_ListCtrl.InsertItem(0, wfd.cFileName);	//�ļ���
			m_ListCtrl.SetItemText(0, 1, szPath);		//�ļ�·��
			m_ListCtrl.SetItemText(0, 2, GetShowSize(wfd.nFileSizeLow));	//�ļ���С
			//���浽Vector
			VIRINFO VirInfo;
			_tcscpy_s(VirInfo.szVirName,wfd.cFileName);
			_tcscpy_s(VirInfo.szVirPath, szPath);
			_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
			m_vecVirInfo.push_back(VirInfo);
		}
	}
	else
	{
		//��������ɱ
		//����ǲ��� ���ļ���Ϣ���浽����Vector
		if (WhiteScan(szPath))
		{

			if (!IsLocal)
			{
				//���뵽List
				m_ListCtrl.InsertItem(0, wfd.cFileName);	//�ļ���
				m_ListCtrl.SetItemText(0, 1, szPath);		//�ļ�·��
				m_ListCtrl.SetItemText(0, 2,				//�ļ���С
					GetShowSize(wfd.nFileSizeLow));
				//���浽Vector
				VIRINFO VirInfo;
				_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
				_tcscpy_s(VirInfo.szVirPath, szPath);
				_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
				m_vecVirInfo.push_back(VirInfo);
			}
			
		}
	}

}

//MD5��ɱ
BOOL CDia7::MD5Scan(LPCTSTR szPath)
{
	//LPCTSTR ·����תCHAR*
	int num = WideCharToMultiByte(CP_OEMCP, NULL, szPath, -1, NULL, 0, NULL, FALSE);
	char* pChar = new char[num];
	WideCharToMultiByte(CP_OEMCP, NULL, szPath, -1, pChar, num, NULL, FALSE);

	//��ȡ�ļ���MD5ֵ
	char* md5 = md5FileValue(pChar);
	//CHAR* תLPCTSTR
	num = MultiByteToWideChar(0, 0, md5, -1, NULL, 0);
	wchar_t* wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, md5, -1, wide, num);
	m_szMD5 = wide;
	delete[]pChar;
	delete[]wide;

	//��ȡ���ļ���MD5��Ϣ ���ж��Ǳ��ػ����ƶ˲�ɱ
	if (IsLocal)
	{
		//���ز�ɱ ��m_LocalMD5���Ա�
		for (DWORD i = 0; i < m_LocalMD5.size(); i++)
		{
			if (m_LocalMD5[i]==m_szMD5)
			{
				//���ƥ�� ˵���ǲ���
				return TRUE;
			}
		}
	}
	else
	{
		//�ƶ�MD5��ɱ		��m_ServerMD5���Ա�
		for (DWORD i = 0; i < m_ServerMD5.size(); i++)
		{
			if (m_ServerMD5[i] == m_szMD5)
			{
				//�ǲ��� ������
				return TRUE;
			}
		}
		
	}
	return FALSE;
}


//���ɨ�谴ť��Ӧ��Ϣ
void CDia7::OnBnClickedButton9()
{
	//��ȡ��ť״̬
	IsSelect[0] = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();
	IsSelect[1] = ((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck();
	IsSelect[2] = ((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck();
	IsSelect[3] = ((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck();
	IsSelect[4] = ((CButton*)GetDlgItem(IDC_RADIO5))->GetCheck();
	IsSelect[5] = ((CButton*)GetDlgItem(IDC_RADIO6))->GetCheck();

	//�жϰ�ť�Ƿ�ѡ��
	if (IsSelect[0] == 0 && IsSelect[1]==0)
	{
		MessageBox(L"��ѡ���ɱ·����");
		return;
	}
	if (IsSelect[2] == 0 && IsSelect[3] == 0)
	{
		MessageBox(L"��ѡ���ɱ��ʽ��");
		return;
	}
	if (IsSelect[4] == 0 && IsSelect[5] == 0)
	{
		MessageBox(L"��ѡ�񲡶��⣡");
		return;
	}

	//��ʼ��ѡ������
	m_vecVirInfo.clear();
	m_LocalMD5.clear();
	m_LocalWhite.clear();
	m_ServerMD5.clear();
	m_ServerWhite.clear();
	InitSelect();
	//��ʼɨ��
	StartScan();
}
//����ѡ���ļ��в˵� ����·��
CString CDia7::BrowseFolder(HWND hWnd, LPCTSTR lpTitle)
{
	TCHAR szPath[MAX_PATH] = {0};
	BROWSEINFO m_bi;

	m_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	m_bi.hwndOwner = m_hWnd;
	m_bi.pidlRoot = NULL;
	m_bi.lpszTitle = lpTitle;
	m_bi.lpfn = NULL;
	m_bi.lParam = NULL;
	m_bi.pszDisplayName = szPath;

	LPITEMIDLIST pidl = ::SHBrowseForFolder(&m_bi);
	if (pidl)
	{
		if (!::SHGetPathFromIDList(pidl, szPath)) szPath[0] = 0;
		IMalloc* pMalloc = NULL;
		if (SUCCEEDED(::SHGetMalloc(&pMalloc)))	 // ȡ��IMalloc�������ӿ�
		{
			pMalloc->Free(pidl); // �ͷ��ڴ�
			pMalloc->Release(); // �ͷŽӿ�
		}

	}
	return szPath;
}

//��ȡ������С
CString CDia7::GetShowSize(DOUBLE dwSize)
{
	CString szSize;
	if (dwSize<1024)
	{
		//B
		szSize.Format(L"%d B", (DWORD)dwSize);
		return szSize;
	}
	else if (dwSize<1024*1024)
	{
		//KB
		szSize.Format(L"%.2f KB", ((DOUBLE)dwSize) / 1024);
		return szSize;
	}
	else if (dwSize<1024*1024*1024)
	{
		//MB
		szSize.Format(L"%.2f MB", (DOUBLE)dwSize / 1024 / 1024);
	}
	if (dwSize>1024 * 1024 * 1024)
	{
		//GB
		szSize.Format(_T("%.2f GB"), (DOUBLE)dwSize / 1024 / 1024 / 1024);
		return szSize;
	}
	return _T("");
}
//�������̲�ˢ�µ��б�ؼ�
void CDia7::ProcessList()
{
	//����������Ϣ�Ľṹ�� ����ṹ�峤����ʹ��ǰ��������
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	//����һ������ ����һ Ҫ��ȡ�Ŀ�������  ����2 ����ȡģ�� ����Ϣʱ ��Ҫָ��һ������ID ���Դ˴���Ч
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//�ҵ���һ�����̵���Ϣ ����1 ���վ�� ����2 ������Ϣ
	if (Process32First(hSnap, &pe) == TRUE)
	{
		//ѭ��������������
		int index = 0;
		do
		{
			CString ProcessID;	//����ID
			CString Priority;	//�������ȼ�
			//��ID�����ȼ�ת���ַ���
			ProcessID.Format(L"%d", pe.th32ProcessID);
			Priority.Format(L"%d", pe.pcPriClassBase);
			//�����������������ƥ�� ����ӵ��б� ���ұ������ID��Vecvtor
			for (DWORD i = 0; i < m_LocalWhite.size();i++)
			{
				if (m_LocalWhite[i] == pe.szExeFile)
				{
					//�ȶԳɹ�˵���ں������� �ǲ���
					//�ѽ�������ӵ���
					m_ListCtrl.InsertItem(index, pe.szExeFile);
					//��ID��ӵ��� ���һ��������ͼ������
					m_ListCtrl.SetItemText(index, 1, ProcessID);
					//��ID��ӵ�����
					m_ProcessId.push_back(ProcessID);
					//�����ȼ���ӽ��������
					m_ListCtrl.SetItemText(index++, 2, Priority);
					
					//���浽Vector
					VIRINFO VirInfo;
					_tcscpy_s(VirInfo.szVirName, pe.szExeFile);
					_tcscpy_s(VirInfo.szVirPath, ProcessID);
					_tcscpy_s(VirInfo.szVirSize,Priority);
					m_vecVirInfo.push_back(VirInfo);
				}
			}	
		} while (Process32Next(hSnap, &pe));
	}
	//������չ����
}
//��������ɱ
BOOL CDia7::WhiteScan(LPCTSTR szPath)
{
	//�ж��Ǳ��ز�ɱ�����ƶ˲�ɱ
	if (IsLocal)
	{
		//��������
		ProcessList();

	}
	else
	{
		//�ƶ˲�ɱ ��m_ServerWhite���Ա�
		for (DWORD i = 0; i < m_ServerWhite.size(); i++)
		{
			if (m_ServerWhite[i] == szPath)
			{
				//�ȶԳɹ�˵���ڰ������� ���ǲ���
				return FALSE; 
			}
		}
	}
	return TRUE;
}

//ȫ·����ɱ
void CDia7::Scam_All(LPCTSTR szPath)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes=0;

	sFindFilter = szPath;
	sFindFilter += TEXT("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
		return;

	do 
	{
		if (_tcscmp(wfd.cFileName, TEXT(".")) == 0 ||
			_tcscmp(wfd.cFileName, TEXT("..")) == 0)
		{
			continue;
		}

		//��ȡ������·����
		sFullPath = szPath;
		sFullPath += TEXT("\\");
		sFullPath += wfd.cFileName;

		//�����ǰ·�����ļ��� ����Ҫ�ݹ��ļ����е��ļ�
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			Scam_All(sFullPath);
		}
		//���� ����һ���ļ�
		else
		{
			//��������������ʼ�·������ ���û�ֱ�۵Ŀ�������ɨ����ļ�·��
			m_szStatus = _T("����ɨ�� : ") + sFullPath;
			SendMessage(WM_MYUPDATEDATA, FALSE);

			//�жϲ�ɱ��ʽ
			if (IsMD5)
			{
				//����ǲ��� ���ļ���Ϣ���浽����Vector
				if (MD5Scan(sFullPath))
				{
					//����List����ʾ
					m_ListCtrl.InsertItem(0, wfd.cFileName);	//�ļ���
					m_ListCtrl.SetItemText(0, 1, sFullPath);	//�ļ�·��
					m_ListCtrl.SetItemText(0, 2,				//�ļ���С
						GetShowSize(wfd.nFileSizeLow));
					//���没����Ϣ��Vector����
					VIRINFO VirInfo;
					_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
					_tcscpy_s(VirInfo.szVirPath, sFullPath);
					_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
					m_vecVirInfo.push_back(VirInfo);

				}
			}
			else
			{
				//��������ɱ
				if (WhiteScan(sFullPath))
				{
					//����List����ʾ
					m_ListCtrl.InsertItem(0, wfd.cFileName);	//�ļ���
					m_ListCtrl.SetItemText(0, 1, sFullPath);	//�ļ�·��
					m_ListCtrl.SetItemText(0, 2,				//�ļ���С
						GetShowSize(wfd.nFileSizeLow));
					//���没����Ϣ��Vector����
					VIRINFO VirInfo;
					_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
					_tcscpy_s(VirInfo.szVirPath, sFullPath);
					_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
					m_vecVirInfo.push_back(VirInfo);
				}
			}
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
}

//��Ӧ�����ť
void CDia7::OnBnClickedButton8()
{
	if (IsLocal&&!IsMD5)
	{
		//��������������
		for (DWORD i = 0; i < m_ProcessId.size(); i++)
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _ttoi(m_ProcessId[i]));
			//��������
			TerminateProcess(hProcess, 0);
		}
	}
	else
	{
		//����ɾ���ļ�
		//���жϲ���Vector�Ƿ�Ϊ�� ��Ϊ�� ����ʾû�п���������ļ�
		if (m_vecVirInfo.size() == 0)
		{
			MessageBox(L"û�п���������ļ���");
			return;
		}
		//ɾ������
		for (DWORD i = 0; i < m_vecVirInfo.size(); i++)
		{
			DeleteFile(m_vecVirInfo[i].szVirPath);
		}
		m_vecVirInfo.clear();
		
	}
	m_ListCtrl.DeleteAllItems();
	MessageBox(L"�� �� �� �� �� �� !");
	
}




//��ʼ��������scoket
void CDia7::InitSocket()
{
	//��ʼ��winsock��
	WSADATA wa = {};
	int nIsSuccess = WSAStartup(
		MAKEWORD(2, 2),		//�汾��Ϣ
		&wa					//��̬���йص���Ϣ
		);
	if (nIsSuccess != 0)
	{
		MessageBox(L"��ʼ��ʧ�ܣ�");
	}

	//����һ��socket
	BOOL bRet = m_Socket.Create();
	//�ж��Ƿ����ӳɹ�
	DWORD dwErr = GetLastError();
	if (!bRet&&WSANOTINITIALISED == dwErr)
	{
		CString str = _T("δ����WSAStartup,����WSAStartupʧ��");
		MessageBox(str);
	}
	m_Socket.Connect(L"127.0.0.1", 1234);
}

//������ʾ�ı�
afx_msg LRESULT CDia7::OnMyupdatedata(WPARAM wParam, LPARAM lParam)
{
	return UpdateData(wParam);
}

//�ύ���������ݿ�
void CDia7::OnBnClickedButton2()
{
	//�Ѵ洢�����ļ�·�����������
	m_SinglePath.Empty();
	//�����ļ�ѡ��� ·��������m_SinglePath
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)_TEXT("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		//�򿪳ɹ� �����ļ�·��
		m_SinglePath = dlg.GetPathName();
	}
	else
	{
		MessageBox(L"���ļ�ʧ�ܣ�");
		return;
	}
	if (m_SinglePath.IsEmpty())
	{
		MessageBox(L"·������Ϊ��!");
		return;
	}

	//�����ļ�·�� ��ȡ�ļ���MD5ֵ
	char* md5 = md5FileValue(CW2A((LPTSTR)(LPCTSTR)m_SinglePath));

	//�����˷�����Ϣ
	Msg msg = { CommitMessage };

	memcpy(msg.contect.MD5, md5, 36);

	
	//������Ϣ
	m_Socket.Send((char*)&msg, sizeof(msg));

	//�����û��ȴ�һ��
	m_szStatus = L"�� �� �� �� �� �� ��!";
	UpdateData(FALSE);
	Sleep(1000);

	//������Ϣ
	int nSuccess = 0;		
	m_Socket.Receive(&nSuccess, sizeof(nSuccess));
	
	if (nSuccess)
	{
		MessageBox(L"��������³ɹ���");
		m_szStatus = L" ��������³ɹ���";
		UpdateData(FALSE);
	}
}
