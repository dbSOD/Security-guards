
// TaskManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "MyTabCtrl.h"
#include "Dia1.h"
#include "Dia2.h"
#include "Dia3.h"
#include "Dia4.h"
#include "Dia5.h"
#include "Dia6.h"
#include "Dia7.h"
#include "TaskManager.h"
#include "data.h"



// CTaskManagerDlg �Ի���
class CTaskManagerDlg : public CDialogEx
{
// ����
public:
	CTaskManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TASKMANAGER_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	HCURSOR m_Cursor;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMyTabCtrl m_MyTable;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();

	CStatusBar m_status;	//״̬��
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//����CPUռ��ʱ��
	static double FILETIME2Double(const _FILETIME& filetime);

	afx_msg void OnBnClickedButton5();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL m_IsWindowHide = TRUE;
	afx_msg void OnLockScreen();
	afx_msg void OnShutDown();
	afx_msg void OnRestart();
	afx_msg void OnCancle();
	afx_msg void OnDormancy();

	void UpLevel();	//��Ȩ
	void ClearMemory();
	//�����ڴ水ť


};
