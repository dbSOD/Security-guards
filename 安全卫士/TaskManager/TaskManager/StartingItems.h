#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

//��������Ϣ�ṹ��
typedef struct _BOOTINFO
{
	TCHAR szBootName[50];			//������
	TCHAR szBootOrder[255];			//����
	TCHAR szHKEY[50];				//������Ϣ
	TCHAR szBootPos[255];			//ɾ��ʱҪ�õ���
	TCHAR szBootPos_All[255];		//List��Ҫ��ʾ��
}BOOTINFO, *PBOOTINFO;

// CStartingItems �Ի���

class CStartingItems : public CDialogEx
{
	DECLARE_DYNAMIC(CStartingItems)

public:
	CStartingItems(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartingItems();

// �Ի�������
	enum { IDD = IDD_StartItem };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();

	//��ȡ��������Ϣ
	void GetBootInfo();
	//������������Ϣ������
	vector<BOOTINFO> m_vecBootInfo;
	afx_msg void OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddStart();
	afx_msg void OnRemoveStart();
	afx_msg void OnRefresh();

	//��ѡ���������
	int m_Select;
	CString strTemp;
};
