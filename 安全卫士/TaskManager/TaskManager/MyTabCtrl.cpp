// MyTabCtrl.cpp : 实现文件
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



// CMyTabCtrl 消息处理程序

void CMyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	//拿到选项卡的序号
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
