// Tab_ImportTable.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_ImportTable.h"
#include "afxdialogex.h"
#include "RVAtoFOA.h"

// Tab_ImportTable 对话框

IMPLEMENT_DYNAMIC(Tab_ImportTable, CDialogEx)

Tab_ImportTable::Tab_ImportTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_ImportTable::IDD, pParent)
{

}

Tab_ImportTable::~Tab_ImportTable()
{
}

void Tab_ImportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_ImportListCtr);
	DDX_Control(pDX, IDC_LIST2, m_ImportListCtr1);
}


BEGIN_MESSAGE_MAP(Tab_ImportTable, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &Tab_ImportTable::OnLvnItemchangedList3)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &Tab_ImportTable::OnClickList3)
END_MESSAGE_MAP()


// Tab_ImportTable 消息处理程序

BOOL Tab_ImportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 获取风格上半部分
	CRect rc;
	m_ImportListCtr.GetClientRect(rc);
	int nWidth = rc.Width();
	DWORD dwOldStyle = m_ImportListCtr.GetExtendedStyle();
	m_ImportListCtr.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ImportListCtr.InsertColumn(0, L"DLL名称", LVCFMT_CENTER, nWidth / 6);
	m_ImportListCtr.InsertColumn(1, L"OriginalFirstThunk", LVCFMT_CENTER, nWidth / 6);
	m_ImportListCtr.InsertColumn(2, L"日期时间标志", LVCFMT_CENTER, nWidth / 6);
	m_ImportListCtr.InsertColumn(3, L"ForwarderChain", LVCFMT_CENTER, nWidth / 6);
	m_ImportListCtr.InsertColumn(4, L"名称", LVCFMT_CENTER, nWidth / 6);
	m_ImportListCtr.InsertColumn(5, L"FirstThunk", LVCFMT_CENTER, nWidth / 6);
	// 获取风格下半部分
	m_ImportListCtr1.GetClientRect(rc);
	int nWidth1 = rc.Width();
	dwOldStyle = m_ImportListCtr1.GetExtendedStyle();
	m_ImportListCtr1.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ImportListCtr1.InsertColumn(0, L"ThunkRVA", LVCFMT_CENTER, nWidth1 / 5);
	m_ImportListCtr1.InsertColumn(1, L"Thunk偏移", LVCFMT_CENTER, nWidth1 / 5);
	m_ImportListCtr1.InsertColumn(2, L"Thunk 值", LVCFMT_CENTER, nWidth1 / 5);
	m_ImportListCtr1.InsertColumn(3, L"提示", LVCFMT_CENTER, nWidth1 / 5);
	m_ImportListCtr1.InsertColumn(4, L"API名称", 0, nWidth1 / 5);
	// 1.显示导出表解析
	ShowImportTable();
	// 2.设置默认选中第一行(FirstThunk)
	m_ImportListCtr.SetItemState(0, 5, LVIS_SELECTED | LVIS_FOCUSED);
	// 3.显示初始化API数据
	APIshow();
	return TRUE;
}

// 显示导入表详细数据
void Tab_ImportTable::ShowImportTable()
{
	RVAtoFOA offset;
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_ImlpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + m_ImlpBase);
	PIMAGE_DATA_DIRECTORY pDir = (PIMAGE_DATA_DIRECTORY)&pNt->OptionalHeader.DataDirectory[1];	
	DWORD pFOA = offset.RVAofFOAs(pDir->VirtualAddress);
	pImport = (PIMAGE_IMPORT_DESCRIPTOR)(pFOA + m_ImlpBase);
	int nCounts = 0;
	CString str;
	while (pImport->Name != NULL)
	{
		DWORD pFOAname = offset.RVAofFOAs(pImport->Name);
		// =========================List控件添加信息=============================
		// 1.DLL名称
		str = (pFOAname + m_ImlpBase);
		m_ImportListCtr.InsertItem(nCounts, str);
		// 2.导入名称的RVA
		str.Format(L"%08X", pImport->OriginalFirstThunk);
		m_ImportListCtr.SetItemText(nCounts, 1, str);
		// 3.时间插入
		str.Format(L"%08X", pImport->TimeDateStamp);
		m_ImportListCtr.SetItemText(nCounts, 2, str);
		// 4.转发插入
		str.Format(L"%08X", pImport->ForwarderChain);
		m_ImportListCtr.SetItemText(nCounts, 3, str);
		// 5.Name的RVA
		str.Format(L"%08X", pImport->Name);
		m_ImportListCtr.SetItemText(nCounts, 4, str);
		// 6.导入地址表的RVA
		str.Format(L"%08X", pImport->FirstThunk);
		m_ImportListCtr.SetItemText(nCounts, 5, str);
		// =========================List控件添加信息=============================
		// 下一个模块
		++nCounts;
		++pImport;
	}
	UpdateData(FALSE);
}

void Tab_ImportTable::OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

// 响应右击更换属性数据
void Tab_ImportTable::OnClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// DWORD* pINTaddr = (DWORD*)(offset.RVAofFOAs(pImport->OriginalFirstThunk) + m_ImlpBase);
	// 每次清空List列表
	m_ImportListCtr1.DeleteAllItems();
	// 显示API解析数据
	APIshow();
	*pResult = 0;
}

// 切换API函数
void Tab_ImportTable::APIshow()
{
	// 临时数据转换
	int nCount = 0;
	RVAtoFOA offset;
	CString str, str1;
	// 1. 获取当前选的FirstThunk 
	int index = m_ImportListCtr.GetSelectionMark();
	str = m_ImportListCtr.GetItemText(index, 5);
	index = m_ImportListCtr.GetSelectionMark();
	str1 = m_ImportListCtr.GetItemText(index, 1);
	// 2. 转换FirstThunk  DWORD
	DWORD FirstThunk = wcstol(str, NULL, 16);
	DWORD OrFirstThunk = wcstol(str1, NULL, 16);
	DWORD pTunks = offset.RVAofFOAs(FirstThunk);
	PIMAGE_THUNK_DATA32 pTunk = (PIMAGE_THUNK_DATA32)(pTunks + m_ImlpBase);
	// 2.1 算出Thunk RVA OFFSET FOA
	auto off = offset.GetRvaofFOA(OrFirstThunk);
	DWORD offs = off.Rva;
	while (pTunk->u1.Ordinal != 0)
	{
		DWORD FOA = offset.RVAofFOAs(OrFirstThunk);
		m_ImportListCtr1.InsertItem(nCount, 0);
		// 3.判断是否名称导入，如果是高位是0说明名称导入
		if (!IMAGE_SNAP_BY_ORDINAL32(pTunk->u1.Ordinal))
		{
			DWORD pNameFOA = (DWORD)offset.RVAofFOAs(pTunk->u1.Ordinal);
			PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(pNameFOA + m_ImlpBase);
			// =========================List控件添加信息=============================
			// （1）.函数名称  NAME 
			str = pName->Name;
			m_ImportListCtr1.SetItemText(nCount, 4, str);
			// （2）.函数序号  HINT
			str.Format(L"%04X", pTunk->u1.Ordinal);
			m_ImportListCtr1.SetItemText(nCount, 3, str);
			str.Format(L"%08X", OrFirstThunk);
			m_ImportListCtr1.SetItemText(nCount, 0, str);
			str.Format(L"%08X", offs);
			m_ImportListCtr1.SetItemText(nCount, 1, str);
			str.Format(L"%08X", FOA);
			m_ImportListCtr1.SetItemText(nCount, 2, str);
		}
		else
		{
			// 2.序号导入  HINT
			str.Format(L"%08X", pTunk->u1.Ordinal & 0xFFFF);
			m_ImportListCtr1.SetItemText(nCount, 3, str);
			str.Format(L"%08X", OrFirstThunk);
			m_ImportListCtr1.SetItemText(nCount, 0, str);
			str.Format(L"%08X", offs);
			m_ImportListCtr1.SetItemText(nCount, 1, str);
			str.Format(L"%08X", FOA);
			m_ImportListCtr1.SetItemText(nCount, 2, str);
		}
		// 4.下一个函数
		++nCount;
		++pTunk;
		OrFirstThunk += 4;
		offs += 4;
	}
}
