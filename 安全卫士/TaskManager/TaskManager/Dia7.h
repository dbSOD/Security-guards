#pragma once
#include <vector>
#include "afxcmn.h"
#include "Md5.h"
#include "data.h"
#include "resource.h"

#define WM_MYUPDATEDATA				WM_USER+100 
using std::vector;

//������Ϣ�ṹ��
typedef struct _VIRINFO
{
	TCHAR szVirName[50];		//��������
	TCHAR szVirPath[MAX_PATH];	//����·��
	TCHAR szVirSize[50];		//������С
}VIRINFO, *PVIRINFO;

class CDia7 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia7)

public:
	CDia7(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia7();

// �Ի�������
	enum { IDD = IDD_DIALOG7 }; 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitSelect();		//��ʼ����ɱѡ��
	void StartScan();		//��ʼɨ��
	CString BrowseFolder(HWND hWnd, LPCTSTR lpTitle);	//�����ļ���ѡ���
	void Scan_Single(LPCTSTR szPath);	//�����ļ���ɱ
	void Scam_All(LPCTSTR szPath);		//ȫ·����ɱ
	BOOL MD5Scan(LPCTSTR szPath);		// MD5��ɱ
	CString GetShowSize(DOUBLE dwSize);	//��ȡ������С
	BOOL WhiteScan(LPCTSTR szPath);		//��������ɱ
	void InitSocket();				//��ʼ��������scoket

	CString m_szMD5;		//�����ļ���MD5ֵ
	CListCtrl m_ListCtrl;
	BOOL IsSelect[6];	//ɱ��ѡ���Ƿ�ѡ��
	BOOL IsSingleScan;	//�Ƿ�Ϊ�����ļ���ɱ	����Ϊȫ·��
	BOOL IsMD5;			//�Ƿ�ΪMD5��ɱ		����Ϊ������
	BOOL IsLocal;		//�Ƿ�Ϊ���ز�ɱ		����Ϊ�ƶ�

	//��ɱ·������
	CString m_SinglePath;	//���浥���ļ���ɱ·�����ַ���
	CString m_AllPath;		//����ȫ·����ɱ��·���ַ���
	afx_msg void OnBnClickedButton9();
	
	vector<VIRINFO> m_vecVirInfo;	//���没����ϢVector
	//�����������Ϣ
	vector<CString> m_LocalMD5;		//����MD5��
	vector<CString> m_LocalWhite;	//���ذ�������
	vector<CString> m_ServerMD5;	//�ƶ�MD5��
	vector<CString> m_ServerWhite;	//�ƶ˰�������
	CString m_szStatus;		//��ʾ��ʾ��Ϣ��static
	vector<CString> m_ProcessId;	//�������ID�Ľṹ�� ��������������
	afx_msg void OnBnClickedButton8();

	
	CSocket m_Socket;		//�����ƶ˷����������
	void ProcessList();	//�������̲�ˢ�µ��б�ؼ�
protected:

	afx_msg LRESULT OnMyupdatedata(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton2();
};
