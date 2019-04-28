// ModuleInfoDialogBox.cpp : 实现文件
//

#include "stdafx.h"
#include "ModuleInfoDialogBox.h"
#include "afxdialogex.h"
#include "TlHelp32.h"

// ModuleInfoDialogBox 对话框

IMPLEMENT_DYNAMIC(ModuleInfoDialogBox, CDialogEx)

ModuleInfoDialogBox::ModuleInfoDialogBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(ModuleInfoDialogBox::IDD, pParent)
{

}

ModuleInfoDialogBox::~ModuleInfoDialogBox()
{
}

void ModuleInfoDialogBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST110, m_ListCtr);
}


BEGIN_MESSAGE_MAP(ModuleInfoDialogBox, CDialogEx)
END_MESSAGE_MAP()


// ModuleInfoDialogBox 消息处理程序


BOOL ModuleInfoDialogBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Get/Set 列表风格
	DWORD dwOldStyle;
	dwOldStyle = m_ListCtr.GetExtendedStyle();
	m_ListCtr.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 获取列表宽度
	CRect rc;
	m_ListCtr.GetClientRect(&rc);
	int nWidth = rc.Width();
	// 设置列表列
	m_ListCtr.InsertColumn(0, L"Module ID", 0, nWidth / 7);
	m_ListCtr.InsertColumn(1, L"Process ID", 0, nWidth / 7);
	m_ListCtr.InsertColumn(2, L"Module Size", 0, nWidth / 7);
	m_ListCtr.InsertColumn(3, L"Process Usage", 0, nWidth / 7);
	m_ListCtr.InsertColumn(4, L"HMODULE hModule", 0, nWidth / 7);
	m_ListCtr.InsertColumn(5, L"szModule", 0, nWidth / 7);
	m_ListCtr.InsertColumn(6, L"szExePath", 0, nWidth / 7);
	// 遍历进程模块
	GetProcessModule(this->m_ownerPid);

	return TRUE;
}

// 获取进程模块信息
void ModuleInfoDialogBox::GetProcessModule(const DWORD idProcess)
{
	TCHAR buf[MAX_PATH] = { 0 };
	HANDLE hSnap;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, idProcess);
	MODULEENTRY32 module32 = { sizeof(MODULEENTRY32) };
	Module32First(hSnap, &module32);
	int i = 0;
	do{
		// 模块ID
		_stprintf_s(buf, MAX_PATH, _T("%d"), module32.th32ModuleID);
		m_ListCtr.InsertItem(i, buf);
		buf[0] = 0;
		// 父进程ID
		_stprintf_s(buf, MAX_PATH, _T("%d"), module32.th32ProcessID);
		m_ListCtr.SetItemText(i, 1, buf);
		// 模块大小
		buf[0] = 0;
		_stprintf_s(buf, MAX_PATH, _T("%d"), module32.dwSize);
		m_ListCtr.SetItemText(i, 2, buf);
		// 全局依赖模块
		buf[0] = 0;
		m_ListCtr.SetItemText(i, 3, buf);
		// Module句柄   HMODULE hModule --> The hModule of this module in th32ProcessID's context 
		_stprintf_s(buf, MAX_PATH, _T("%d"), module32.hModule);
		m_ListCtr.SetItemText(i, 4, buf);
		// WCHAR   szModule[MAX_MODULE_NAME32 + 1];
		m_ListCtr.SetItemText(i, 5, module32.szModule);
		// 路径 WCHAR   szExePath[MAX_PATH];
		m_ListCtr.SetItemText(i, 5, module32.szExePath);
	} while (Module32Next(hSnap, &module32));
	CloseHandle(hSnap);
	buf[0] = 0;
}

