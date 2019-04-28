// Tab_ExportTable.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "Tab_ExportTable.h"
#include "afxdialogex.h"
#include "RVAtoFOA.h"


// Tab_ExportTable 对话框

IMPLEMENT_DYNAMIC(Tab_ExportTable, CDialogEx)

Tab_ExportTable::Tab_ExportTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_ExportTable::IDD, pParent)
	// , m_OutOffset(_T(""))
	, m_VAULES(_T(""))
	, m_exBase(_T(""))
	, m_exName(_T(""))
	// , m_exStr(_T(""))
	, m_exFunnumber(_T(""))
	, m_exFun(_T(""))
	, m_exFunaddress(_T(""))
	, m_exNameaddress(_T(""))
	, m_exNumbers(_T(""))
{

}

Tab_ExportTable::~Tab_ExportTable()
{
}

void Tab_ExportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// DDX_Text(pDX, IDC_OUTOFFSET, m_OutOffset);
	DDX_Text(pDX, IDC_VALUES, m_VAULES);
	DDX_Text(pDX, IDC_EXBASE, m_exBase);
	DDX_Text(pDX, IDC_EXNAME, m_exName);
	// DDX_Text(pDX, IDC_EXSTR, m_exStr);
	DDX_Text(pDX, IDC_EXFUNNUMBER, m_exFunnumber);
	DDX_Text(pDX, IDC_EXFUN, m_exFun);
	DDX_Text(pDX, IDC_EXFUNADDRESS, m_exFunaddress);
	DDX_Text(pDX, IDC_EXNAMEADDRESS, m_exNameaddress);
	DDX_Text(pDX, IDC_EXNUMBERS, m_exNumbers);
	DDX_Control(pDX, IDC_EXLIST1, m_exListCtr);
}

BEGIN_MESSAGE_MAP(Tab_ExportTable, CDialogEx)
END_MESSAGE_MAP()

// Tab_ExportTable 消息处理程序

BOOL Tab_ExportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 获取风格
	DWORD dwOldStyle;
	dwOldStyle = m_exListCtr.GetExtendedStyle();
	m_exListCtr.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect rc;
	m_exListCtr.GetClientRect(rc);
	int nWidth = rc.Width();
	// 插入行
	m_exListCtr.InsertColumn(0, L"序号", LVCFMT_CENTER, nWidth / 4);
	m_exListCtr.InsertColumn(1, L"RVA", LVCFMT_CENTER, nWidth / 4);
	m_exListCtr.InsertColumn(2, L"偏移量", LVCFMT_CENTER, nWidth / 4);
	m_exListCtr.InsertColumn(3, L"函数名", LVCFMT_CENTER, nWidth / 4);
	GetExportInfo(m_lpBases);
	return TRUE;
}

// 获取导出表信息
void Tab_ExportTable::GetExportInfo(char* lpBase)
{
	RVAtoFOA offset;
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpBase);
	PIMAGE_DATA_DIRECTORY pData = (PIMAGE_DATA_DIRECTORY)pNt->OptionalHeader.DataDirectory;
	// 1.RVA转换地址
	DWORD  pExprotFOA = offset.RVAofFOAs(pData->VirtualAddress);
	// 2.找到输出表地址
	PIMAGE_EXPORT_DIRECTORY pExportTable = (PIMAGE_EXPORT_DIRECTORY)(pExprotFOA + m_lpBases);
	// 特征值
	m_VAULES.Format(L"%08X", pExportTable->TimeDateStamp);
	// 序号基址
	m_exBase.Format(L"%08X", pExportTable->Base);
	// 名称
	m_exName.Format(L"%08X", pExportTable->Name);
	// 函数子串
	m_exStr;
	// 函数地址
	m_exFunaddress.Format(L"%08X", pExportTable->AddressOfFunctions);
	// 函数名称地址
	m_exNameaddress.Format(L"%08X", pExportTable->AddressOfNames);
	// 函数序号地址
	m_exNumbers.Format(L"%08X", pExportTable->AddressOfNameOrdinals);
	// 3.函数数量
	DWORD dwFunCount = pExportTable->NumberOfFunctions;
	m_exFunnumber.Format(L"%d", dwFunCount);
	// 4.函数名称数量
	DWORD dwOrdinalCount = pExportTable->NumberOfNames;
	m_exFun.Format(L"%d", dwFunCount);
	// 5.地址表
	DWORD* pFunAdd = (DWORD*)(offset.RVAofFOAs(pExportTable->AddressOfFunctions) + lpBase);
	// 6.函数名称表
	DWORD* pNameAddr = (DWORD*)(offset.RVAofFOAs(pExportTable->AddressOfNames) + lpBase);
	// 7.序号表
	DWORD* pOrdinalAddr = (DWORD*)(offset.RVAofFOAs(pExportTable->AddressOfNameOrdinals) + lpBase);
	// 8.循环函数信息
	CString str;
	_SAVERVAOFOFFSET obj = {};
	for (DWORD i = 0; i < dwFunCount; ++i)
	{
		// 9.如果是0则是无效地址 
		if (0 == pFunAdd[i]){
			continue;
		}
		// 10.遍历序号表是否有序号, 函数有名字
		BOOL bFlag = FALSE;
		for (DWORD j = 0; j < dwOrdinalCount; ++j)
		{
			bFlag = TRUE;
			DWORD dwNameRVA = pNameAddr[j];
			// 函数序号
			str.Format(L"%d", pOrdinalAddr[i]);
			m_exListCtr.InsertItem(i, str);
			// RVA
			str.Format(L"%08X", dwNameRVA);
			m_exListCtr.SetItemText(i, 1, str);
			// 函数名
			str = (char *)(offset.RVAofFOAs(dwNameRVA) + lpBase);
			m_exListCtr.SetItemText(i, 3, str);
			// offset
			obj = offset.GetRvaofFOA(dwNameRVA);
			str.Format(L"%08X", obj.offset);
			m_exListCtr.SetItemText(i, 2, str);

		}
		if (!bFlag){
			// 函数名NULL 函数序号 %04X
			str.Format(L"%d", (i + pExportTable->Base));
			m_exListCtr.SetItemText(i, 0, str);
		}
	}
	UpdateData(FALSE);
}
