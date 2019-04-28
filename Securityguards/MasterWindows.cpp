// MasterWindows.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "MasterWindows.h"
#include "afxdialogex.h"
#include "Tab_CleanruDialogbox.h"
#include "Tab_MemoryDialogbox.h"
#include "Tab_NetworktoolDialogbox.h"
#include "Tab_PEfileanalDialogbox.h"
#include "Tab_ProcessmangDialogbox.h"
#include "Tab_SoftwarmangDialogbox.h"
#include "Tab_LDCS.h"
#include "PowrProf.h"

// MasterWindows 对话框

IMPLEMENT_DYNAMIC(MasterWindows, CDialogEx)

// 初始化TabName
const TCHAR MasterWindows::tab_Name[7][12] = { _T("内存优化"), _T("进程管理"), _T("PE文件分析"), _T("软件与服务管理"), _T("垃圾清理"), _T("网络攻防"), _T("零度查杀") };

MasterWindows::MasterWindows(CWnd* pParent /*=NULL*/)
	: CDialogEx(MasterWindows::IDD, pParent)
{

}

MasterWindows::~MasterWindows()
{

}

void MasterWindows::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab_control);
	DDX_Control(pDX, IDC_Wolf, m_PictrueWolf);
}


BEGIN_MESSAGE_MAP(MasterWindows, CDialogEx)

	ON_COMMAND(ID_40021, &MasterWindows::On40021)
	ON_COMMAND(ID_40022, &MasterWindows::Reboot)
	ON_COMMAND(ID_40024, &MasterWindows::Lockscreen)
	ON_COMMAND(ID_40032, &MasterWindows::Cancellation)
	ON_COMMAND(ID_40035, &MasterWindows::BOOSKEY)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// MasterWindows 消息处理程序
BOOL MasterWindows::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bush.CreateSolidBrush(RGB(255, 255, 255));
	// 1.初始化Tab窗口
	this->SetdwCount(7);
	m_Tab_control.InitData(m_dwCount, tab_Name[0], tab_Name[1], tab_Name[2], tab_Name[3], tab_Name[4], tab_Name[5], tab_Name[6]);
	// 2.添加窗口资源
	m_Tab_control.AddData(m_dwCount,
		IDD_Memory_optimization, new Tab_MemoryDialogbox,
		IDD_Process_management, new Tab_ProcessmangDialogbox,
		IDD_PE_Fille_analysis, new Tab_PEfileanalDialogbox,
		IDD_Softwar_management, new Tab_SoftwarmangDialogbox,
		IDD_Clean_rubbish, new Tab_CleanruDialogbox,
		IDD_Network_tool, new Tab_NetworktoolDialogbox,
		IDD_TABLDCS, new Tab_LDCS
		);
	// 3.设置窗口Log小图标
	SetIcon(LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_WinSmall)), FALSE);
	// 4.设置主窗口界面图片
	CBitmap* p = new CBitmap();
	p->LoadBitmapW(IDB_BITMAP1);
	m_PictrueWolf.SetBitmap((HBITMAP)p->m_hObject);
	return TRUE;
}

// 获取系统权限
void MasterWindows::GetSystempermissions()
{
	HANDLE hToken = NULL;
	// 获取伪句柄（该线程）
	HANDLE hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	// 特权计数
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}

/*-----------------【系统操作】--------------------------*/

// 1. 关机
void MasterWindows::On40021()
{
	if (IDOK == MessageBox(L"Whether to turn it off", L"Warning", MB_ICONQUESTION | MB_OKCANCEL))
	{
		GetSystempermissions();
		ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
	}
}

// 2. 重启
void MasterWindows::Reboot()
{
	if (IDOK == MessageBox(L"Whether to restart", L"Warning", MB_ICONQUESTION | MB_OKCANCEL))
	{
		GetSystempermissions();
		ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
	}
}

// 3. 休眠
//void MasterWindows::Dormancy()
//{
//	if (IDOK == MessageBox(L"Whether to restart", L"Warning", MB_ICONQUESTION | MB_OKCANCEL))
//	{
//		GetSystempermissions();
//		SetSuspendState(TRUE, FALSE, FALSE);
//	}
//}

// 4. 锁屏
void MasterWindows::Lockscreen()
{
	if (IDOK == MessageBox(L"Whether to restart", L"Warning", MB_ICONQUESTION | MB_OKCANCEL))
	{
		GetSystempermissions();
		LockWorkStation();
	}
}

// 5. 注销
void MasterWindows::Cancellation()
{
	if (IDOK == MessageBox(L"Whether to restart", L"Warning", MB_ICONQUESTION | MB_OKCANCEL))
	{
		GetSystempermissions();
		ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
	}
}

// 6. 老板键
void MasterWindows::BOOSKEY()
{
	// 注册一个快捷键 Ctrl + shift + H(字母大写)
	::RegisterHotKey(this->m_hWnd, 0x1234, MOD_CONTROL | MOD_SHIFT, 'H');
}

//  响应快捷键消息（老板键）
BOOL MasterWindows::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类

	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 0x1234))
	{
		if (::IsWindowVisible(m_hWnd) == TRUE)
			ShowWindow(SW_HIDE);
		else
			ShowWindow(SW_SHOW);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH MasterWindows::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return  m_bush;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
