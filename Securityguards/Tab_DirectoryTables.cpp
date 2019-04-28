// Tab_DirectoryTables.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_DirectoryTables.h"
#include "afxdialogex.h"
#include "Tab_ExportTable.h"
#include "Tab_ImportTable.h"
#include "Tab_RELOCATION.h"
#include "Tab_resourceTables.h"
#include "Tab_TlsTables.h"
#include "Tab_Dlltables.h"


// Tab_DirectoryTables 对话框

IMPLEMENT_DYNAMIC(Tab_DirectoryTables, CDialogEx)

Tab_DirectoryTables::Tab_DirectoryTables(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_DirectoryTables::IDD, pParent)
	, m_indexexport(_T(""))
	, m_indexexports(_T(""))
	, m_imports(_T(""))
	, m_import(_T(""))
	, m_indexSource(_T(""))
	, m_indexSources(_T(""))
	, m_Exect(_T(""))
	, m_Exects(_T(""))
	, m_Security(_T(""))
	, m_Securitys(_T(""))
	, m_Reactions(_T(""))
	, m_RELOCATION(_T(""))
	, m_Debug(_T(""))
	, m_Debugs(_T(""))
	, m_Copyright(_T(""))
	, m_Copyrights(_T(""))
	, m_Global(_T(""))
	, m_Globals(_T(""))
	, m_ThreadStorag(_T(""))
	, m_ThreadStorags(_T(""))
	, m_Load(_T(""))
	, m_Loads(_T(""))
	, m_Importaddress(_T(""))
	, m_importaddresses(_T(""))
	, m_BindDirectory(_T(""))
	, m_BindDirectorys(_T(""))
	, m_Delay(_T(""))
	, m_Delays(_T(""))
	, m_Com(_T(""))
	, m_Coms(_T(""))
	, m_Keeps(_T(""))
	, m_Keep(_T(""))
{

}

Tab_DirectoryTables::~Tab_DirectoryTables()
{
}

void Tab_DirectoryTables::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_STATICexport, m_indexexport);
	//  DDX_Text(pDX, IDC_STATICexports, m_indexexports);
	DDX_Text(pDX, IDC_STATICexport, m_indexexport);
	DDX_Text(pDX, IDC_STATICexports, m_indexexports);
	DDX_Text(pDX, IDC_STATICimport, m_imports);
	DDX_Text(pDX, IDC_STATICimports, m_import);
	DDX_Text(pDX, IDC_STATICsourc, m_indexSource);
	DDX_Text(pDX, IDC_STATICsourcs, m_indexSources);
	//  DDX_Control(pDX, IDC_Exect, m_Exect);
	//  DDX_Control(pDX, IDC_Exects, m_Exects);
	DDX_Text(pDX, IDC_Exect, m_Exect);
	DDX_Text(pDX, IDC_Exects, m_Exects);
	DDX_Text(pDX, IDC_SECURITY, m_Security);
	DDX_Text(pDX, IDC_SECURITYS, m_Securitys);
	DDX_Control(pDX, IDC_RELOCATION, m_Relocation);
	DDX_Text(pDX, IDC_RELOCATIONS, m_Reactions);
	DDX_Text(pDX, IDC_RELOCATION, m_RELOCATION);
	DDX_Text(pDX, IDC_DEBUG, m_Debug);
	DDX_Text(pDX, IDC_DEBUGS, m_Debugs);
	DDX_Text(pDX, IDC_COPYRIGHT, m_Copyright);
	DDX_Text(pDX, IDC_COPYRIGHTS, m_Copyrights);
	DDX_Text(pDX, IDC_GLOABLE, m_Global);
	DDX_Text(pDX, IDC_GLOABLES, m_Globals);
	DDX_Text(pDX, IDC_ThreadStorag, m_ThreadStorag);
	DDX_Text(pDX, IDC_ThreadStorags, m_ThreadStorags);
	DDX_Text(pDX, IDC_LOAD, m_Load);
	DDX_Text(pDX, IDC_LOADSS, m_Loads);
	DDX_Text(pDX, IDC_IMPORTADDRESS, m_Importaddress);
	DDX_Text(pDX, IDC_IMPORTADDRESSES, m_importaddresses);
	DDX_Text(pDX, IDC_BINDDIRECTORY, m_BindDirectory);
	DDX_Text(pDX, IDC_BINDDIRECTORYS, m_BindDirectorys);
	DDX_Text(pDX, IDC_DELAY, m_Delay);
	DDX_Text(pDX, IDC_DELAYS, m_Delays);
	DDX_Text(pDX, IDC_COM, m_Com);
	DDX_Text(pDX, IDC_COMS, m_Coms);
	DDX_Text(pDX, IDC_KEEP, m_Keeps);
	DDX_Text(pDX, IDC_KEEPS, m_Keep);
}

BEGIN_MESSAGE_MAP(Tab_DirectoryTables, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON11, &Tab_DirectoryTables::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON13, &Tab_DirectoryTables::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON16, &Tab_DirectoryTables::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON15, &Tab_DirectoryTables::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON12, &Tab_DirectoryTables::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON20, &Tab_DirectoryTables::OnBnClickedButton20)
END_MESSAGE_MAP()

// Tab_DirectoryTables 消息处理程序

BOOL Tab_DirectoryTables::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_lpBases;
	PIMAGE_NT_HEADERS32 pNt = (PIMAGE_NT_HEADERS32)(pDos->e_lfanew + m_lpBases);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &(pNt->OptionalHeader);
	// 1.获取数据表
	pDataDirectory = pOptionalHeader->DataDirectory;
	// 2.显示每个表的RVA和大小
	ShowDateofsize(pDataDirectory);
	return TRUE; 
}

// 显示数据RVA及大小
void Tab_DirectoryTables::ShowDateofsize(PIMAGE_DATA_DIRECTORY pDataDirectory)
{
	// 1.输出表
	m_indexexport.Format(L"%08X", pDataDirectory[0].VirtualAddress);
	m_indexexports.Format(L"%08X", pDataDirectory[0].Size);
	// 2.输入表
	m_imports.Format(L"%08X", pDataDirectory[1].VirtualAddress);
	m_import.Format(L"%08X", pDataDirectory[1].Size);
	// 3.资源表
	m_indexSource.Format(L"%08X", pDataDirectory[2].VirtualAddress);
	m_indexSources.Format(L"%08X", pDataDirectory[2].Size);
	// 4.异常处理
	m_Exect.Format(L"%08X", pDataDirectory[3].VirtualAddress);
	m_Exects.Format(L"%08X", pDataDirectory[3].Size);
	// 5.安全结构
	m_Security.Format(L"%08X", pDataDirectory[4].VirtualAddress);
	m_Securitys.Format(L"%08X", pDataDirectory[4].Size);
	// 6.重定向表
	m_RELOCATION.Format(L"%08X", pDataDirectory[5].VirtualAddress);
	m_Reactions.Format(L"%08X", pDataDirectory[5].Size);
	// 7.调试信息
	m_Debug.Format(L"%08X", pDataDirectory[6].VirtualAddress);
	m_Debugs.Format(L"%08X", pDataDirectory[6].Size);
	// 8.版权
	m_Copyright.Format(L"%08X", pDataDirectory[7].VirtualAddress);
	m_Copyrights.Format(L"%08X", pDataDirectory[7].Size);
	// 9.全局指针
	m_Global.Format(L"%08X", pDataDirectory[8].VirtualAddress);
	m_Globals.Format(L"%08X", pDataDirectory[8].Size);
	// 10.线程局部存储
	m_ThreadStorag.Format(L"%08X", pDataDirectory[9].VirtualAddress);
	m_ThreadStorags.Format(L"%08X", pDataDirectory[9].Size);
	// 11.载入配置
	m_Load.Format(L"%08X", pDataDirectory[10].VirtualAddress);
	m_Loads.Format(L"%08X", pDataDirectory[10].Size);
	// 12.导入地址表
	m_Importaddress.Format(L"%08X", pDataDirectory[11].VirtualAddress);
	m_importaddresses.Format(L"%08X", pDataDirectory[11].Size);
	// 13.绑定输入目录表
	m_BindDirectory.Format(L"%08X", pDataDirectory[12].VirtualAddress);
	m_BindDirectorys.Format(L"%08X", pDataDirectory[12].Size);
	// 14.延迟载入描述
	m_Delay.Format(L"%08X", pDataDirectory[13].VirtualAddress);
	m_Delays.Format(L"%08X", pDataDirectory[13].Size);
	// 15.COM
	m_Com.Format(L"%08X", pDataDirectory[14].VirtualAddress);
	m_Coms.Format(L"%08X", pDataDirectory[14].Size);
	// 16.保留
	m_Keeps.Format(L"%08X", pDataDirectory[15].VirtualAddress);
	m_Keep.Format(L"%08X", pDataDirectory[15].Size);
	UpdateData(FALSE);
}

// 导出表详细数据
void Tab_DirectoryTables::OnBnClickedButton11()
{
	// 获取pDataDuretory的表头
	if (!pDataDirectory->VirtualAddress){
		AfxMessageBox(L"exe没有导出表");
		return;
	}
	Tab_ExportTable exports;
	exports.m_lpBases = this->m_lpBases;
	exports.DoModal();
}

// 导入表详细数据
void Tab_DirectoryTables::OnBnClickedButton13()
{
	Tab_ImportTable imports;
	imports.m_ImlpBase = this->m_lpBases;
	imports.DoModal();
}

// 重定位表详细数据
void Tab_DirectoryTables::OnBnClickedButton16()
{
	Tab_RELOCATION relocation;
	relocation.m_lpBaseReLoc = this->m_lpBases;
	relocation.DoModal();
}

// 静态文本显示结构体中的数据(TLS表)
void Tab_DirectoryTables::OnBnClickedButton15()
{
	Tab_TlsTables tlsTable;
	tlsTable.m_lpBase = this->m_lpBases;
	tlsTable.DoModal();
}

// 资源表
void Tab_DirectoryTables::OnBnClickedButton12()
{
	// TODO:  在此添加控件通知处理程序代码
	Tab_resourceTables resour;
	resour.m_lpBase = this->m_lpBases;
	resour.DoModal();
}

// DLL延迟表
void Tab_DirectoryTables::OnBnClickedButton20()
{
	Tab_Dlltables dllLoad;
	dllLoad.m_lpBase = this->m_lpBases;
	dllLoad.DoModal();
}
