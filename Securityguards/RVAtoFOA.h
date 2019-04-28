#pragma once
#ifndef RVATOFOA_H_
#define RVATOFOA_H_
#include <afxwin.h>
#include <windows.h>

struct _SAVERVAOFOFFSET
{
	DWORD Rva;
	DWORD offset;
};

class RVAtoFOA{
public:
	RVAtoFOA();
	~RVAtoFOA();
public:
	// 定义全局文件名
	static char* g_NamelpBase;
	// 转换函数
	DWORD RVAofFOAs(const DWORD dwRVA);
	// 获取当前区段的名称
	_SAVERVAOFOFFSET GetRvaofFOA(const DWORD dwRVA);
};



#endif