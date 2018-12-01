#pragma once
#include "afxcmn.h"
#include <vector>
using std::vector;

// CDia3 �Ի���

class CDia3 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia3)

public:
	CDia3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia3();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

private:
	vector<CString> PathStrings;// ����·��������
	vector<CString> DelStrings;	// Ҫɾ�����ļ�
	INT FileCount, DestCount;	// ���ļ������Ϳ�ɾ�ļ�����
	LARGE_INTEGER CountSize;	// �ļ��ܴ�С
	LARGE_INTEGER DestSize;		// ��ɾ���ļ��Ĵ�С
	INT DeleteCount;			// ��ɾ�����ļ�����

public:
	CListCtrl PathList;		//·���б�ؼ�
	afx_msg void OnBnClickedButton2();	//��ʾ��ɾ�����ļ���Ϣ
	CListCtrl WaitDelete;	//��ʾҪ������ļ����б�

	void FindFile(CString  Path);
	
	CString ExtendString;	//�洢�ļ���չ�����ַ���
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CListCtrl DeleteList;
	int m_Radio;	//��ѡ��ť
	afx_msg void OnClickedRadio1(UINT id);
	virtual BOOL OnInitDialog();
	void CleatRecycleBin();		//�������վ����
	afx_msg void OnBnClickedButton4();
};
