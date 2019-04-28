/*作用为全局唯一*/
#pragma once
#include "windows.h"

// 枚举结构体记录消息类型
enum SockEnum
{
	MD5MEEAGE = 2,		// MD5上传
	MD5SELECT = 3		// MD5查询
};

// MD5响应消息
typedef struct _MESSAGECONTENT
{
	LONG ulFileSize;
	char sMD5[36];
}MESSAGECONTENT, *PMESSAGECONTENT;

// 客户端发送数据包到服务器
typedef struct _SOCKETSEND
{
	HANDLE hWnd;				// 窗口句柄
	SockEnum eEnum;				// 消息类型
	union _SENDMESSAGE
	{
		MESSAGECONTENT	m_Md5;	// md5结构体
	}u1;
	char PathName[100];			// 存储名字
}SOCKETSEND, *PSOCKETSEND;

// 响应服务器发来的数据包
typedef struct _SERVERSTRUCT
{
	HANDLE hWnd;
	SockEnum eEnum;
	INT OpenCode;				// 成功(1)或失败(2)返回值
	union _SENDMESSAGE
	{
		MESSAGECONTENT	m_Md5;	// md5结构体
		SOCKETSEND		se;		// 接收服务器端发来消息 iii data(SOCKETSEND类型)
	}u1;
	char PathName[100];		// 存储名字
}SERVERSTRUCT, *PSERVERSTRUCT;