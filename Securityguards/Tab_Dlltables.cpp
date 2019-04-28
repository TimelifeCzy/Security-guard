// Tab_Dlltables.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_Dlltables.h"
#include "afxdialogex.h"
#include "RVAtoFOA.h"

// Tab_Dlltables 对话框

IMPLEMENT_DYNAMIC(Tab_Dlltables, CDialogEx)

Tab_Dlltables::Tab_Dlltables(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_Dlltables::IDD, pParent)
{

}

Tab_Dlltables::~Tab_Dlltables()
{
}

void Tab_Dlltables::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtr);
}


BEGIN_MESSAGE_MAP(Tab_Dlltables, CDialogEx)
END_MESSAGE_MAP()


// Tab_Dlltables 消息处理程序


BOOL Tab_Dlltables::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 获取风格
	DWORD dwOldStyle;
	dwOldStyle = m_ListCtr.GetExtendedStyle();
	m_ListCtr.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect rc;
	m_ListCtr.GetClientRect(rc);
	int nWidth = rc.Width();
	// 插入行
	m_ListCtr.InsertColumn(0, L"DLL名字", LVCFMT_CENTER, nWidth / 5);
	m_ListCtr.InsertColumn(1, L"Module(RVA)", LVCFMT_CENTER, nWidth / 5);
	m_ListCtr.InsertColumn(2, L"延迟载入IAT(RVA)", LVCFMT_CENTER, nWidth / 5);
	m_ListCtr.InsertColumn(3, L"延迟载入RVA（INT）", LVCFMT_CENTER, nWidth / 5);
	m_ListCtr.InsertColumn(4, L"绑定IAT（RVA）", LVCFMT_CENTER, nWidth / 5);
	GetdllLoadofShow();

	return TRUE;
}

void Tab_Dlltables::GetdllLoadofShow()
{
	CString str;
	RVAtoFOA foa;
	// 1. 获取DLL表
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfarlc + m_lpBase);
	PIMAGE_DATA_DIRECTORY pData = (PIMAGE_DATA_DIRECTORY)&pNt->OptionalHeader.DataDirectory[13];
	PIMAGE_DELAYLOAD_DESCRIPTOR pDll = (PIMAGE_DELAYLOAD_DESCRIPTOR)(foa.RVAofFOAs(pData->VirtualAddress) + m_lpBase);

	// 2. 名称RVA
	str.Format(L"%08x", pDll->DllNameRVA);
	m_ListCtr.InsertItem(0, str);
	// 3. Module RVA
	str.Format(L"%08X", pDll->ModuleHandleRVA);
	m_ListCtr.SetItemText(0, 1, str);
	// 4. 延迟载入RVA
	str.Format(L"%08X", pDll->ImportAddressTableRVA);
	m_ListCtr.SetItemText(0, 2, str);
	// 5. 延迟载入INT
	str.Format(L"%08X", pDll->ImportNameTableRVA);
	m_ListCtr.SetItemText(0, 3, str);
	// 6. 绑定IAT的RVA
	str.Format(L"%08X", pDll->BoundImportAddressTableRVA);
	m_ListCtr.SetItemText(0, 4, str);
}