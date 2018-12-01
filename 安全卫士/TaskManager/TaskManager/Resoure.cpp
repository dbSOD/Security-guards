// Resoure.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "Resoure.h"
#include "afxdialogex.h"
#include "Dia5.h"
#include "AddressChange.h"
#include <map>
std::map<HTREEITEM, PIMAGE_RESOURCE_DIRECTORY_ENTRY>tree_addr;

// CResoure 对话框

IMPLEMENT_DYNAMIC(CResoure, CDialogEx)

CResoure::CResoure(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResoure::IDD, pParent)
	, m_FirstEdit(_T("0000"))
	, m_SecondEidt(_T("0000"))
	, m_ThirdEidt(_T("0000"))
	, m_FouthEdit(_T("0000"))
	, m_RVAEdit(_T("00000000"))
	, m_FOAEdit(_T("00000000"))
	, m_SizeEdit(_T("00000000"))
{

}

CResoure::~CResoure()
{
}

void CResoure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_FirstEdit);
	DDX_Text(pDX, IDC_EDIT17, m_SecondEidt);
	DDX_Text(pDX, IDC_EDIT11, m_ThirdEidt);
	DDX_Text(pDX, IDC_EDIT22, m_FouthEdit);
	DDX_Text(pDX, IDC_EDIT20, m_RVAEdit);
	DDX_Text(pDX, IDC_EDIT23, m_FOAEdit);
	DDX_Text(pDX, IDC_EDIT24, m_SizeEdit);
}


BEGIN_MESSAGE_MAP(CResoure, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CResoure::OnClickTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CResoure::OnSelchangedTree1)
END_MESSAGE_MAP()


// CResoure 消息处理程序


BOOL CResoure::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//找到资源表
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CDia5::g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CDia5::g_lpBase);
	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[2];

	DWORD dwResFOA = CAddressChange::RVAtoFOA(pDir->VirtualAddress);

	//遍历 先找到资源表的第一个结构体
	PIMAGE_RESOURCE_DIRECTORY pResHead = (PIMAGE_RESOURCE_DIRECTORY)(dwResFOA + CDia5::g_lpBase);
	pRhead = (DWORD)pResHead;

	//第一层 种类
	WCHAR* arryResType[] = { L"", L"鼠标指针（Cursor）", L"位图（Bitmap）", L"图标（Icon）", L"菜单（Menu）"
		, L"对话框（Dialog）", L"字符串列表（String Table）", L"字体目录（Font Directory）", L"字体（Font）", L"快捷键（Accelerators）"
		, L"非格式化资源（Unformatted）", L"消息列表（Message Table）", L"鼠标指针组（Croup Cursor）", L"", L"图标组（Group Icon）", L""
		, L"版本信息（Version Information）" };
	//资源个数
	DWORD dwCount_1 = pResHead->NumberOfIdEntries + pResHead->NumberOfNamedEntries;
	CString Content;	//要显示的内容
	//显示第一个Edit的内容
	Content.Format(L"%04X", pResHead->NumberOfIdEntries);
	m_FirstEdit = Content;
	//显示第二个Edit的内容
	Content.Format(L"%04X", pResHead->NumberOfNamedEntries);
	m_SecondEidt = Content;
	//找到紧跟其后的结构体
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResHeadEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResHead+1);
	//遍历资源种类
	for (DWORD i_1 = 0; i_1 < dwCount_1; i_1++)
	{
		HTREEITEM hFirst = {0};	//第一层的根节点句柄
		//判断是否是字符串命名
		if (pResHeadEntry->NameIsString)
		{
			//如果是字符串命名 根据RVA找到保存字符串名字和长度的结构体
			PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)(pResHeadEntry->NameOffset + (DWORD)pResHead);
			//根据字符串长度申请空间
			WCHAR* wcName = new WCHAR[pName->Length + 1]{};
			//把名字拷贝到申请的空间里
			memcpy(wcName,pName->NameString,pName->Length * 2);

			//显示种类到树空间
			hFirst = m_TreeCtrl.InsertItem(wcName);
			tree_addr.insert({ hFirst, pResHeadEntry});
			//释放申请的空间
			delete []wcName;
		}
		else
		{
			//如果是以ID命名
			if (pResHeadEntry->Id <= 16 && pResHeadEntry->Id > 0)
			{
				hFirst = m_TreeCtrl.InsertItem(arryResType[pResHeadEntry->Id]);
				tree_addr.insert({ hFirst, pResHeadEntry});
			}
			else
			{
				Content.Format(L"%d", pResHeadEntry->Id);
				hFirst = m_TreeCtrl.InsertItem(Content);
				tree_addr.insert({ hFirst, pResHeadEntry});
			}
		}
		//遍历完第一层 查看是否有下一层
		if (pResHeadEntry->DataIsDirectory)
		{
			//第二层的第一个结构体
			PIMAGE_RESOURCE_DIRECTORY pRes2 = (PIMAGE_RESOURCE_DIRECTORY)(pResHeadEntry->OffsetToDirectory+(DWORD)pResHead);
			//第二层的个数
			DWORD dwCount_2 = pRes2->NumberOfIdEntries + pRes2->NumberOfNamedEntries;
			//第二层的第二个结构体
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pRes2Entry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes2 + 1);
			//遍历第二层
			for (DWORD i_2 = 0; i_2 < dwCount_2; i_2++)
			{
				HTREEITEM hSecond = {0};	//第二层的根节点

				//判断是否是字符串命名
				if (pRes2Entry->NameIsString)
				{
					//如果是字符串命名通过RVA找到指向字符串名字和长度的那个结构体
					PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)(pRes2Entry->NameOffset + (DWORD)pResHead);
					WCHAR* wcName = new WCHAR[pName->Length + 1]{};
					memcpy(wcName,pName->NameString,pName->Length*2);
					//往第一层的子节点添加内容显示第二层资源
					hSecond = m_TreeCtrl.InsertItem(wcName, hFirst);
					tree_addr.insert({ hSecond, pRes2Entry});
					delete []wcName;
				}
				else
				{
					//ID命名
					Content.Format(L"%d", pRes2Entry->Id);
					hSecond = m_TreeCtrl.InsertItem(Content, hFirst);
					tree_addr.insert({ hSecond, pRes2Entry});
				}
				//判断是否有下一层
				if (pRes2Entry->DataIsDirectory)
				{
					//找到第三层的第一个结构体
					PIMAGE_RESOURCE_DIRECTORY pRes3 = (PIMAGE_RESOURCE_DIRECTORY)(pRes2Entry->OffsetToDirectory + (DWORD)pResHead);
					//找到第三层的第二个结构体
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pRes3Entry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes3+1);
					//找到第二个联合体指向的资源结构体
					PIMAGE_RESOURCE_DATA_ENTRY pDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(pRes3Entry->OffsetToData+(DWORD)pResHead);
					//通过RVA找到具体的资源位置
					DWORD dwDataFOA = CAddressChange::RVAtoFOA(pDataEntry->OffsetToData);
					DWORD dwDataSize = pDataEntry->Size;
					unsigned char* pData = (unsigned char*)(dwDataFOA + CDia5::g_lpBase);

					//显示第三层资源的数据 
					//Content.Format(L"%02X", pData);
					//m_TreeCtrl.InsertItem(Content, hSecond);
				}
				//下一个资源
				pRes2Entry++;
			}//第二层
		}
		//下一种资源
		pResHeadEntry++;
	}//第一层
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//响应树控件的点击消息
void CResoure::OnClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//获取树控件选中的内容
	//CString Temp = m_TreeCtrl.GetItemText(m_TreeCtrl.GetSelectedItem());
	*pResult = 0;
}

//响应树控件的点击消息
void CResoure::OnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CString Content;
	//拿到选中的句柄
	HTREEITEM hTree=m_TreeCtrl.GetSelectedItem();
	//拿到对应的结构体
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResEntry = tree_addr[hTree];

	//首先往下找一层 如果下一层还有下一层显示资源数量
	//下一层的第一个结构体
	PIMAGE_RESOURCE_DIRECTORY pRes2 = (PIMAGE_RESOURCE_DIRECTORY)(pResEntry->OffsetToDirectory + pRhead);
	//下一层的第二个结构体
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pRes2Entry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes2 + 1);
	//显示第三个Edit的内容
	Content.Format(L"%04X", pRes2->NumberOfNamedEntries);
	m_ThirdEidt = Content;
	//显示第四个Edit的内容
	Content.Format(L"%04X", pRes2->NumberOfIdEntries);
	m_FouthEdit = Content;

	//如果下一层没有下一层 显示资源内容
	if (!(pRes2Entry->DataIsDirectory))
	{
		//找到第二个联合体指向的资源结构体
		PIMAGE_RESOURCE_DATA_ENTRY pDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(pRes2Entry->OffsetToData + pRhead);
		//显示RVA
		Content.Format(L"%08X", pDataEntry->OffsetToData);
		m_RVAEdit = Content;
		//显示FOA
		Content.Format(L"%08X", CAddressChange::RVAtoFOA(pDataEntry->OffsetToData));
		m_FOAEdit = Content;
		//显示Size
		Content.Format(L"%08X", pDataEntry->Size);
		m_SizeEdit = Content;
	}
	UpdateData(FALSE);
	*pResult = 0;
}
