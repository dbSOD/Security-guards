#pragma once


// CDirectory �Ի���

class CDirectory : public CDialogEx
{
	DECLARE_DYNAMIC(CDirectory)

public:
	CDirectory(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDirectory();

// �Ի�������
	enum { IDD = IDD_Directory };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_ExportRVA;		//������RVA
	CString m_ExportSize;		//������Size
	CString m_ImportRVA;		//�����RVA
	CString m_ImportSize;		//�����Size
	CString m_ResourceRVA;		//��Դ��RVA
	CString m_ResourceSize;		//��Դ��Size
	CString m_RelocRVA;			//�ض�λ��RVA
	CString m_RelocSize;		//�ض�λ��Size
	CString m_TLSRVA;			//TLS��RVA
	CString m_TLSSize;			//TLS��Size
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton5();
	CString m_DelayRVA;			//�ӳ������RVA
	CString m_DelaySize;		//�ӳ������Size
	afx_msg void OnBnClickedButton10();
};
