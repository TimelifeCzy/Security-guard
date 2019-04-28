// MFCsockclass.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCsockclass.h"
#include "Tab_LDCS.h"


// MFCsockclass
LPVOID MFCsockclass::Thisptr = nullptr;

MFCsockclass::MFCsockclass()
{
	// 1. 初始化模块
	WSADATA wsaData = { 0 };
	if (NULL != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		AfxMessageBox(L"初始化WSAStartup()失败");
	}
}

MFCsockclass::~MFCsockclass()
{
	// 释放套接字
	this->Close();
	// 清除WSA
	WSACleanup();
}

// MFCsockclass 成员函数

// 解析服务器发来的数据包
void MFCsockclass::OnReceive(int nErrorCode)
{
	// 1. 响应服务器发来的消息
	char* pData = NULL;
	pData = new char[1024]{0};
	this->Receive(pData, sizeof(char) * 1024, 0);

	// 2. 调用函数接收
	Tab_LDCS* ldcs = (Tab_LDCS *)Thisptr;
	ldcs->OnReceives(NULL, (LPARAM)pData);

	// 3. 释放掉堆空间
	delete pData;
	pData = NULL;
	CSocket::OnReceive(nErrorCode);
}
