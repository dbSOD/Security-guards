#pragma once


// CAddressChange �Ի���

class CAddressChange : public CDialogEx
{
	DECLARE_DYNAMIC(CAddressChange)

public:
	CAddressChange(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddressChange();

// �Ի�������
	enum { IDD = IDD_AddressChange };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_VA;
	CString m_RVA;
	CString m_Offect;
	CString m_ExInfo;
	CString m_Section;
	CString m_Bytes;
	afx_msg void OnBnClickedButton2();

	static DWORD RVAtoFOA(DWORD dwRVA);	//��ַת������
	afx_msg void OnEnChangeEdit3();
};
