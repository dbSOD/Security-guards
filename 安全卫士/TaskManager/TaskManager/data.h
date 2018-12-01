#pragma once
//消息类型
typedef enum MessageType
{
	Md5Message,			//Md5消息
	CommitMessage,		//提交样本消息
}MsgType;


//发送的消息内容
typedef struct MESSAGECONTENT
{
	MsgType Msg_Type;	//消息类型
	union MyUnion
	{
		char MD5[36];
	}contect;
}Msg;



