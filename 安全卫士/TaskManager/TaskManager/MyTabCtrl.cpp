// MyTabCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager.h"
#include "MyTabCtrl.h"



// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTabCtrl ��Ϣ�������

void CMyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�õ�ѡ������
	int nNum = GetCurSel();
	for (int i = 0; i < 7; i++)
	{
		if (i == nNum)
		{
			m_Dia[i]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_Dia[i]->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}
