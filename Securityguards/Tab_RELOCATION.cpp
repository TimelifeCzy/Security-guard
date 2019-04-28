// Tab_RELOCATION.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "Tab_RELOCATION.h"
#include "afxdialogex.h"
#include "RVAtoFOA.h"


// Tab_RELOCATION 对话框

IMPLEMENT_DYNAMIC(Tab_RELOCATION, CDialogEx)

Tab_RELOCATION::Tab_RELOCATION(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_RELOCATION::IDD, pParent)
{

}

Tab_RELOCATION::~Tab_RELOCATION()
{
}

void Tab_RELOCATION::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RELIST1, m_ReListCtr);
	DDX_Control(pDX, IDC_RELIST2, m_ReListCtrs);
}

BEGIN_MESSAGE_MAP(Tab_RELOCATION, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_RELIST1, &Tab_RELOCATION::OnLvnItemchangedRelist1)
END_MESSAGE_MAP()

// Tab_RELOCATION 消息处理程序

BOOL Tab_RELOCATION::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ==================风格获取List=========================/
	CRect rc;
	m_ReListCtr.GetClientRect(rc);
	int nWidth = rc.Width();
	DWORD dwOldStyle = m_ReListCtr.GetExtendedStyle();
	// 设置区段信息
	m_ReListCtr.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ReListCtr.InsertColumn(0, L"索引", LVCFMT_CENTER, nWidth / 4);
	m_ReListCtr.InsertColumn(1, L"区段", LVCFMT_CENTER, nWidth / 4);
	m_ReListCtr.InsertColumn(2, L"RVA", LVCFMT_CENTER, nWidth / 4);
	m_ReListCtr.InsertColumn(3, L"项目", LVCFMT_CENTER, nWidth / 4);

	// 设置块项目
	m_ReListCtrs.GetClientRect(rc);
	int nWidth1 = rc.Width();
	dwOldStyle = m_ReListCtrs.GetExtendedStyle();
	m_ReListCtrs.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ReListCtrs.InsertColumn(0, L"索引", LVCFMT_CENTER, nWidth1 / 6);
	m_ReListCtrs.InsertColumn(1, L"RVA", LVCFMT_CENTER, nWidth1 / 6);
	m_ReListCtrs.InsertColumn(2, L"偏移", LVCFMT_CENTER, nWidth1 / 6);
	m_ReListCtrs.InsertColumn(3, L"类型", LVCFMT_CENTER, nWidth1 / 6);
	m_ReListCtrs.InsertColumn(4, L"FAR地址", LVCFMT_CENTER, nWidth1 / 6);
	m_ReListCtrs.InsertColumn(6, L"数据解释", LVCFMT_CENTER, nWidth1 / 6);
	// 显示区域解析属性
	SectionAnal();
	return TRUE;
}

// 区段属性解析 analysis
void Tab_RELOCATION::SectionAnal()
{
	RVAtoFOA offset;
	// 1.定义一个结构体辅助
	typedef struct _OFFSET{
		WORD Offset : 12;
		WORD Type : 4;
	}Offset;
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_lpBaseReLoc;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + m_lpBaseReLoc);
	// 2.获取数组中的RVA
	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[5];
	DWORD dwResFOA = offset.RVAofFOAs(pDir->VirtualAddress);
	// 3.获取重定基址
	PIMAGE_BASE_RELOCATION pResHead = (PIMAGE_BASE_RELOCATION)(dwResFOA + m_lpBaseReLoc);
	// 4.循环
	int nCount = 0;
	int index = 1;
	CString str;
	while (pResHead->SizeOfBlock != 0)
	{
		// 插入一空行
		m_ReListCtr.InsertItem(nCount, NULL);
		Offset* pOffset = (Offset *)(pResHead + 1);
		// 5.需要重定位 (SizeofBlock - 8) / 2 = 需要重定位的个数
		DWORD dwCount = ((pResHead->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2);
		// ============================List控件插入================================
		// （1） 索引插入
		str.Format(L"%d", index);
		m_ReListCtr.SetItemText(nCount, 0, str);
		// （2） RVA（区段）
		auto rva = GetSection(pNt, pDir->VirtualAddress);
		str.Format(L"%08X", rva);
		m_ReListCtr.SetItemText(nCount, 2, str);
		// （3） 区域的名称
		m_ReListCtr.SetItemText(nCount, 1, strName);
		// （4） 需要重定向个数
		str.Format(L"%d", dwCount);
		m_ReListCtr.SetItemText(nCount, 3, str);
		// 6.循环
		for (DWORD i = 0; i < dwCount; ++i)
		{
			if (pOffset->Type == 3){
				// （1） 插入索引
				m_ReListCtrs.InsertItem(i, NULL);
				str.Format(L"%d", i);
				m_ReListCtrs.SetItemText(i, 0, str);
				// 7.偏移地址 + 相对虚拟地址(Base的RVA)
				DWORD dwDataFOA = offset.RVAofFOAs(pOffset->Offset + pResHead->VirtualAddress);
				// （2） 偏移地址RVA
				str.Format(L"%08X", pResHead->VirtualAddress);
				m_ReListCtrs.SetItemText(i, 1, str);
				// （3） 偏移地址offset
				str.Format(L"%08X", pOffset->Offset);
				m_ReListCtrs.SetItemText(i, 2, str);
				// （4） 类型 pOffset->Type
				str.Format(L"%08X", pOffset->Type);
				m_ReListCtrs.SetItemText(i, 3, str);
				// （5） FAR 地址 
				DWORD* fra = (DWORD *)(pOffset->Offset + m_lpBaseReLoc);
				str.Format(L"08X", *fra);
				m_ReListCtrs.SetItemText(i, 4, str);
				// （6） 数据解释
				// 预留
				DWORD* pData = (DWORD*)(dwDataFOA + m_lpBaseReLoc);
				// 重定位数据插入
			}
			++pOffset;
		}
		// 8.下一个重定位
		pResHead = (PIMAGE_BASE_RELOCATION)(pResHead->SizeOfBlock + (DWORD)pResHead);
		++index;
		++nCount;
	}
	UpdateData(FALSE);
}
// 块项目属性解析
void Tab_RELOCATION::BloackAnal()
{
}
// 解析
DWORD Tab_RELOCATION::GetSection(PIMAGE_NT_HEADERS const pNt, DWORD const dwRVA)
{
	// 区段个数
	DWORD dwCount = pNt->FileHeader.NumberOfSections;
	// 获取区段头
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	for (DWORD i = 0; i < dwCount; ++i)
	{
		// 在这个区段
		if ((dwRVA >= pSection->VirtualAddress) && (dwRVA < (pSection->VirtualAddress + pSection->SizeOfRawData))){
			// 获取区段的名称
			strName.Format(L"%d", i);
			strName += "  ";
			strName += (char *)pSection->Name;
			// 获取区段的RVA
			return pSection->VirtualAddress;
		}
		++pSection;
	}
	AfxMessageBox(L"获取区段失败!");
	return 0;
}

void Tab_RELOCATION::OnLvnItemchangedRelist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}
