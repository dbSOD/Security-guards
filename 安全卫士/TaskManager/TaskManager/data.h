#pragma once
//��Ϣ����
typedef enum MessageType
{
	Md5Message,			//Md5��Ϣ
	CommitMessage,		//�ύ������Ϣ
}MsgType;


//���͵���Ϣ����
typedef struct MESSAGECONTENT
{
	MsgType Msg_Type;	//��Ϣ����
	union MyUnion
	{
		char MD5[36];
	}contect;
}Msg;



