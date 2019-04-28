#pragma once

// MFCsockclass 命令目标

class MFCsockclass : public CSocket
{
public:
	MFCsockclass();
	virtual ~MFCsockclass();
	virtual void OnReceive(int nErrorCode);

public:
	// 保存 响应this指针
	static LPVOID Thisptr;

private:
	const int m_port = 1234;
};


