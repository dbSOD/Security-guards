#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

#define Win32PATH _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
#define Win64PATH _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
//�����Ϣ�ṹ��
typedef struct _SOFTINFO
{
	TCHAR szSoftName[50];				//�������
	TCHAR szSoftVer[50];				//����汾��
	TCHAR strSoftVenRel[50];			//�����������
	TCHAR szSoftData[20];				//�����װ����
	TCHAR strSoftInsPath[MAX_PATH];		//�����װ·��
	TCHAR strSoftUniPath[MAX_PATH];		//���ж��·��
}SOFTINFO, *PSOFTINFO;
// CSoftManage �Ի���

class CSoftManage : public CDialogEx
{
	DECLARE_DYNAMIC(CSoftManage)

public:
	CSoftManage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSoftManage();

// �Ի�������
	enum { IDD = IDD_Soft };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrl;
	//���������Ϣ
	void GetSoftwareInfo();
	//������������Ϣ
	vector<SOFTINFO> m_vecSoftInfo;
	//�ж�ϵͳ�Ƿ���64λ
	BOOL Is64();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	//��ѡ������
	int m_Select;
	CString strTemp;
	afx_msg void OnUninstall();
	afx_msg void OnFlush();
	afx_msg void OnOpenSoft();
	BOOL OpenFolderAndSelectFile(TCHAR* szPath);
};
