#pragma once


// CDelay �Ի���

class CDelay : public CDialogEx
{
	DECLARE_DYNAMIC(CDelay)

public:
	CDelay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDelay();

// �Ի�������
	enum { IDD = IDD_Delay };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_AllAttributes;		//����
	CString m_DllAllName;			//�ӳټ��ص�DLL����
	CString m_ModuleHandleRVA;		//ģ����
	CString m_ImportAddressRVA;		//IAT
	CString m_ImportNameRVA;		//INT
	CString m_BoundImportAddress;	//��IAT
	CString m_UnloadInfo;			//ж����Ϣ
	CString m_TimeData;				//ʱ���
};
