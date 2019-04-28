// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

HANDLE WINAPI MyOpenProcess(DWORD dwDesiredAccess,	BOOL bInheritHandle, DWORD dwProcessId);
DWORD Pid;
void InstallHOOK();
void UninstallHOOK();
// 保存原的属性
BYTE g_oldFun[5];
BYTE g_NewFun[5] = { 0xE9 };


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		MessageBox(NULL, L"X64任务管理器", L"注入", NULL);
		InstallHOOK();
	}
	break;
	// case DLL_THREAD_ATTACH:
	// case DLL_THREAD_DETACH:
		// 断开
	case DLL_PROCESS_DETACH:
		UninstallHOOK();
		break;
	}
	return TRUE;
}


HANDLE WINAPI MyOpenProcess(DWORD dwDesiredAccess,
	BOOL bInheritHandle, DWORD dwProcessId)
{
	if (dwProcessId == Pid)
	{
		::MessageBox(NULL, L"无权关闭守护进程", L"警告", NULL);
		return NULL;
	}
	else
	{
		UninstallHOOK();
		HANDLE hProc = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
		InstallHOOK();
		return hProc;
	}
}

// 安装HOOK
void InstallHOOK()
{
	HANDLE hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,L"Pid");
	LPVOID hAddr = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS,0,0,0x10);
	Pid = *(DWORD*)hAddr;

	// 1. 保存原来的OPcpde
	memcpy(g_oldFun, OpenProcess, 5);
	// 2. 计算偏移
	DWORD dwOffset = (DWORD)MyOpenProcess - (DWORD)OpenProcess - 5;
	// 3. 
	*(DWORD *)(g_NewFun + 1) = dwOffset;
	DWORD dwOldAttrubet = 0;
	// 4. 申请内存空间
	VirtualProtect(OpenProcess, 5, PAGE_EXECUTE_READWRITE, &dwOldAttrubet);
	// 5. 修改diamante
	memcpy(OpenProcess, g_NewFun, 5);
	// 6. 恢复原来的内存属性
	VirtualProtect(OpenProcess, 5, dwOldAttrubet, &dwOldAttrubet);


}

// 卸载HOOK
void UninstallHOOK()
{
	DWORD dwoldAttrubet;
	// 获取属性
	VirtualProtect(OpenProcess, 5, PAGE_EXECUTE_READWRITE, &dwoldAttrubet);

	memcpy(OpenProcess, g_oldFun, 5);
	
	VirtualProtect(OpenProcess, 5, dwoldAttrubet, &dwoldAttrubet);

}

