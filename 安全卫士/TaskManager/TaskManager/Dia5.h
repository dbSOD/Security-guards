#pragma once


// CDia5 �Ի���

class CDia5 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia5)

public:
	CDia5(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia5();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString ExtendString;				//������չ�����ַ���
	CString m_NumberOfSection;			//������
	CString m_TimeDateStamp;		    //����ʱ��
	CString m_SizeOfOptionalHeader;		//��ѡͷ��С
	CString m_characteristic;			//����ֵ Ҳ���ļ�����
	CString m_AddressOfEntryPoint;		//������ڵ�
	CString m_SizeOfHeaders;			//���е�ͷ���Ĵ�С
	CString m_FileAlignment;			//�ļ�����
	CString m_SizeOfImage;				//ӳ���С
	CString m_ImageBase;				//Ĭ�ϼ��ػ�ַ
	CString m_SectionAlignment;			//�ڴ����
	afx_msg void OnBnClickedButton7();
	
	static char* g_lpBase;	//�ļ����ڴ��е��׵�ַ
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton4();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();

	void SavePid();		//����PID
};
