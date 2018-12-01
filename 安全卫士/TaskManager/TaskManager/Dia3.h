#pragma once
#include "afxcmn.h"
#include <vector>
using std::vector;

// CDia3 对话框

class CDia3 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia3)

public:
	CDia3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia3();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

private:
	vector<CString> PathStrings;// 保存路径的容器
	vector<CString> DelStrings;	// 要删除的文件
	INT FileCount, DestCount;	// 总文件数量和可删文件数量
	LARGE_INTEGER CountSize;	// 文件总大小
	LARGE_INTEGER DestSize;		// 可删除文件的大小
	INT DeleteCount;			// 已删除的文件个数

public:
	CListCtrl PathList;		//路径列表控件
	afx_msg void OnBnClickedButton2();	//显示待删除的文件信息
	CListCtrl WaitDelete;	//显示要输出的文件的列表

	void FindFile(CString  Path);
	
	CString ExtendString;	//存储文件扩展名的字符串
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CListCtrl DeleteList;
	int m_Radio;	//单选框按钮
	afx_msg void OnClickedRadio1(UINT id);
	virtual BOOL OnInitDialog();
	void CleatRecycleBin();		//清理回收站垃圾
	afx_msg void OnBnClickedButton4();
};
