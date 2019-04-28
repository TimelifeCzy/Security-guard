// Tab_resourceTables.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_resourceTables.h"
#include "afxdialogex.h"
#include "RVAtoFOA.h"


// Tab_resourceTables 对话框

IMPLEMENT_DYNAMIC(Tab_resourceTables, CDialogEx)

Tab_resourceTables::Tab_resourceTables(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_resourceTables::IDD, pParent)
{

}

Tab_resourceTables::~Tab_resourceTables()
{
}

void Tab_resourceTables::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeFile);
}

BEGIN_MESSAGE_MAP(Tab_resourceTables, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &Tab_resourceTables::OnTvnSelchangedTree1)
END_MESSAGE_MAP()

// Tab_resourceTables 消息处理程序
void Tab_resourceTables::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

void Tab_resourceTables::ShowResourInfo()
{
	// 第一层树节点
	HTREEITEM m_hRoot = nullptr;
	// 第二层树节点
	HTREEITEM m_hRoot2 = nullptr;

	// 种类信息（第一层用）
	char* arryResType[] = { "", "鼠标指针（Cursor）", "位图（Bitmap）", "图标（Icon）", "菜单（Menu）"
		, "对话框（Dialog）", "字符串列表（String Table）", "字体目录（Font Directory）", "字体（Font）", "快捷键（Accelerators）"
		, "非格式化资源（Unformatted）", "消息列表（Message Table）", "鼠标指针组（Croup Cursor）", "", "图标组（Group Icon）", ""
		, "版本信息（Version Information）" };
	// 资源表准备地址获取
	CString str;
	RVAtoFOA rva;
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + m_lpBase);
	PIMAGE_DATA_DIRECTORY pData = (PIMAGE_DATA_DIRECTORY)&pNt->OptionalHeader.DataDirectory[2];
	
	// 1.地址
	PIMAGE_RESOURCE_DIRECTORY pResHead = (PIMAGE_RESOURCE_DIRECTORY)(rva.RVAofFOAs(pData->VirtualAddress) + m_lpBase);
	// 2.第一层种类数量
	WORD dwCount_1 = pResHead->NumberOfIdEntries + pResHead->NumberOfNamedEntries;
	// 2.1 属性
	str.Format(L"%08X", pResHead->Characteristics);
	GetDlgItem(IDC_EDIT1)->SetWindowTextW(str);
	// 2.2 时间
	str.Format(L"%08X", pResHead->TimeDateStamp);
	GetDlgItem(IDC_EDIT7)->SetWindowTextW(str);
	// 2.3 主版本
	str.Format(L"%08X", pResHead->MajorVersion);
	GetDlgItem(IDC_EDIT2)->SetWindowTextW(str);
	// 2.4 次版本
	str.Format(L"%08X", pResHead->MajorVersion);
	GetDlgItem(IDC_EDIT6)->SetWindowTextW(str);
	// 2.5 用字符串作为资源标识符数目
	str.Format(L"%08X", pResHead->NumberOfNamedEntries);
	GetDlgItem(IDC_EDIT3)->SetWindowTextW(str);
	// 2.6 用数字ID作为资源标识数目
	str.Format(L"%08X", pResHead->NumberOfNamedEntries);
	GetDlgItem(IDC_EDIT4)->SetWindowTextW(str);
	// 2.7 资源总数目
	str.Format(L"%08X", dwCount_1);
	GetDlgItem(IDC_EDIT5)->SetWindowTextW(str);
	// 3.获得entry(第一层)
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResourcEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResHead + 1);
	RVAtoFOA offset;
	// 4.遍历表
	for (DWORD i = 0; i < dwCount_1; ++i)
	{
		// 判断最高位
		if (pResourcEntry->NameIsString){
			// 计算出下一个唯一
			PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)(pResourcEntry->NameOffset + (DWORD)pResHead);
			WCHAR* wcName = new WCHAR[pName->Length + 1]{};
			memcpy(wcName, pName->NameString, pName->Length * 2);
			// 添加到第一层目录  Tree (Name)
			m_hRoot = m_TreeFile.InsertItem(wcName);
		}
		else{
			// 如果是零的话
			if ((pResourcEntry->Id <= 16) && (pResourcEntry->Id >0))
			{
				// 种类是什么  arrResType[]
				str = arryResType[pResourcEntry->Id];
				m_TreeFile.InsertItem(str);
			}
			else
			{
				str.Format(L"04X", pResourcEntry->Id);
				m_TreeFile.InsertItem(str);
			}
		}

		// 判断是否有下一层
		if (pResourcEntry->DataIsDirectory){
			// 第二层
			PIMAGE_RESOURCE_DIRECTORY pResources_2 = (PIMAGE_RESOURCE_DIRECTORY)(pResourcEntry->OffsetToDirectory + (DWORD)pResHead);
			// 获取当前的资源总数
			DWORD dwCount_2 = pResources_2->NumberOfIdEntries + pResources_2->NumberOfNamedEntries;
			// 找到Entry头地址
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry2 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResources_2 + 1);
			// 判断高位是否是字符串？
			for (DWORD i = 0; i < dwCount_2; ++i)
			{
				// 这是个字符串
				if (pEntry2->NameIsString){
					PIMAGE_RESOURCE_DIR_STRING_U pName = 
						(PIMAGE_RESOURCE_DIR_STRING_U)(pEntry2->NameOffset + (DWORD)pResHead);
					WCHAR* wcName = new WCHAR[pName->Length + 1]{};
					memcpy(wcName, pName->NameString, pName->Length * 2);
					m_hRoot2 = m_TreeFile.InsertItem(wcName, m_hRoot);
				}
				else{
					// 这不是一个字符串ID
					str.Format(L"%04X", pEntry2->Id);
					m_hRoot2 = m_TreeFile.InsertItem(str, m_hRoot);
				}

				// 是否有下一层
				if (pEntry2->DataIsDirectory)
				{
					PIMAGE_RESOURCE_DIRECTORY pResourc_3 = (PIMAGE_RESOURCE_DIRECTORY)(pEntry2->OffsetToDirectory + (DWORD)pResHead);
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry_3 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResourc_3 + 1);
					PIMAGE_RESOURCE_DATA_ENTRY PDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(pEntry_3->OffsetToData + (DWORD)pResHead);
					
					DWORD dwDataFOA = offset.RVAofFOAs(PDataEntry->OffsetToData);
					DWORD dwDataSize = PDataEntry->Size;
					// 第三层资源的具体位置偏移是加上加载基址
					unsigned char* pData = (unsigned char*)(dwDataFOA + m_lpBase);
					for (int i = 0; i < 10; i++)
					{
						// printf("%02X ", pData[i]);
						str.Format(L"%02X", pData[i]);
						m_TreeFile.InsertItem(str, m_hRoot2);
					}
				}
				++pResources_2;
			}// 第二层
		}
		++pResourcEntry;
	} // 第一次层
}

BOOL Tab_resourceTables::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 初始化树控件信息
	m_TreeFile.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS);
	// 显示资源表数据信息
	ShowResourInfo();
	return TRUE;
}
