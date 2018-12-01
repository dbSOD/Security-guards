// Import.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Import.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"


// CImport 对话框

IMPLEMENT_DYNAMIC(CImport, CDialogEx)

CImport::CImport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImport::IDD, pParent)
{

}

CImport::~CImport()
{
}

void CImport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrlUp);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrlDown);
}


BEGIN_MESSAGE_MAP(CImport, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CImport::OnClickList1)
END_MESSAGE_MAP()


// CImport 消息处理程序


BOOL CImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//初始化列信息
	m_ListCtrlUp.InsertColumn(0, L"DLL名称",0,100);
	m_ListCtrlUp.InsertColumn(1, L"OriginalFirstThunk", 0, 100);
	m_ListCtrlUp.InsertColumn(2, L"日期时间标志", 0, 100);
	m_ListCtrlUp.InsertColumn(3, L"ForwarderChain", 0, 100);
	m_ListCtrlUp.InsertColumn(4, L"名称", 0, 100);
	m_ListCtrlUp.InsertColumn(5, L"FirstThunk", 0, 100);
	m_ListCtrlUp.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ListCtrlDown.InsertColumn(0, L"ThunkRVA", 0, 120);
	m_ListCtrlDown.InsertColumn(1, L"Thunk偏移", 0, 120);
	m_ListCtrlDown.InsertColumn(2, L"Thunk值", 0, 120);
	m_ListCtrlDown.InsertColumn(3, L"提示", 0, 120);
	//m_ListCtrlDown.InsertColumn(4, L"API名称", 0, 120);

	//找到导入表
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + (DWORD)CDia5::g_lpBase);
	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[1];

	DWORD dwImportFOA = CAddressChange::RVAtoFOA(pDir->VirtualAddress);

	//导入表在文件中的位置
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(dwImportFOA + (DWORD)CDia5::g_lpBase);
	int i = 0;
	//遍历导入表
	while (pImport->Name)
	{
		DWORD dwNameFOA = CAddressChange::RVAtoFOA(pImport->Name);
		//----------------------------------显示到控件----------------------------
		//显示DLL名称
		CString sName;
		sName.Format(L"%S", dwNameFOA + CDia5::g_lpBase);
		m_ListCtrlUp.InsertItem(i, sName);
		//显示name的RVA
		CString sNameRVA;
		sNameRVA.Format(L"%08X", pImport->Name);
		m_ListCtrlUp.SetItemText(i, 4, sNameRVA);
		//显示OriginalFirstThunk导入表名称的RVA
		CString sOriginalFirstThunk;
		sOriginalFirstThunk.Format(L"%08X", pImport->OriginalFirstThunk);
		m_ListCtrlUp.SetItemText(i, 1, sOriginalFirstThunk);
		//显示日期时间标志(没用)
		CString sTimeDate;
		sTimeDate.Format(L"%08X", pImport->TimeDateStamp);
		m_ListCtrlUp.SetItemText(i, 2, sTimeDate);
		//显示转发机制(没用)
		CString sForwarderChain;
		sForwarderChain.Format(L"%08X", pImport->ForwarderChain);
		m_ListCtrlUp.SetItemText(i, 3, sForwarderChain);
		//显示FirstThunk导入地址表的RVA
		CString sFirstThunk;
		sFirstThunk.Format(L"%08X", pImport->FirstThunk);
		m_ListCtrlUp.SetItemText(i, 5, sFirstThunk);

		i++;
		//下一个模块
		pImport++;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//相应列表的点击消息
void CImport::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//删除所有项
	m_ListCtrlDown.DeleteAllItems();
	//获取选择的行号
	int nRow = m_ListCtrlUp.GetSelectionMark();
	//定义一个数组来存放获取到的RVA
	WCHAR wFirstThunk[9] = {};
	//获取导入名称表的RVA
	m_ListCtrlUp.GetItemText(nRow, 1, wFirstThunk, 9);
	DWORD dwFirstThunk = 0;
	//CSting转换成十六进制的DWORD
	_stscanf_s(wFirstThunk,L"%X",&dwFirstThunk);
	//算出导入名称表的FOA
	DWORD dwThunkFOA = CAddressChange::RVAtoFOA(dwFirstThunk);
	//找到导入名称表
	DWORD* pINTAddr = (DWORD*)(dwThunkFOA + CDia5::g_lpBase);
	int row = 0;	//要插入的行号和循环计数
	CString Content;
	//开始遍历导入名称表
	while (*pINTAddr)
	{
		//显示FirstThunk的RVA
		Content.Format(L"%08X", dwFirstThunk + 4 * row);
		m_ListCtrlDown.InsertItem(row, Content);
		//显示FirstThunk的文件偏移
		Content.Format(L"%08X", dwThunkFOA + 4 * row);
		m_ListCtrlDown.SetItemText(row,1,Content);
		//显示FristThunk的所在偏移中的内容
		Content.Format(L"%08X", *pINTAddr);
		m_ListCtrlDown.SetItemText(row, 2, Content);

		PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)pINTAddr;
		//判断是否是名称导入
		if (!IMAGE_SNAP_BY_ORDINAL(pThunk->u1.Ordinal))
		{
			//通过函数名的RVA 算出FOA 得到指向名字的结构体
			DWORD pNameFOA = (DWORD)CAddressChange::RVAtoFOA(pThunk->u1.Ordinal);
			auto pName = (PIMAGE_IMPORT_BY_NAME)(pNameFOA + CDia5::g_lpBase);
			//显示序号和名字
			Content.Format(L"%04X", pName->Hint);
			m_ListCtrlDown.SetItemText(row, 3, Content);
			Content = CString(pName->Name);
			m_ListCtrlDown.SetItemText(row, 4, Content.GetBuffer());
		}
		else
		{
			//如果是序号导入
			Content.Format(L"%04X", pThunk->u1.Ordinal & 0xFFFF);
			m_ListCtrlDown.SetItemText(row, 3, Content);
		}
		row++;
		pINTAddr++;
	}
	*pResult = 0;
}
