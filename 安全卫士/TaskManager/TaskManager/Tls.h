#pragma once


// CTls �Ի���

class CTls : public CDialogEx
{
	DECLARE_DYNAMIC(CTls)

public:
	CTls(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTls();

// �Ի�������
	enum { IDD = IDD_TLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_StartAddress;		  //Դ���ݵ���ʼλ��
	CString m_EndAddress;		  //Դ���ݵ���ֹλ��
//	CString AddressOfIndex;		  
	CString m_AddressOfCallBacks; //TLS�ص�������ַ���λ��
	CString m_SizeOfZeroFill;	  //��0���TLS��������Ĵ�С
	CString m_Characteristics;	  //���� ��Ϊ0
	CString m_AddressOfIndex;	  //����TLS������λ��
};
