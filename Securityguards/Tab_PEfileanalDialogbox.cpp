// Tab_PEfileanalDialogbox.cpp : 实现文件
//
#include "stdafx.h"
#include "Tab_PEfileanalDialogbox.h"
#include "afxdialogex.h"
#include "Tab_FilePosition.h"
#include "Tab_SectionView.h"
#include "Tab_DirectoryTables.h"
#include "RVAtoFOA.h"
#include "FileInfo.h"
#include "windows.h"
#


// Tab_PEfileanalDialogbox 对话框

IMPLEMENT_DYNAMIC(Tab_PEfileanalDialogbox, CDialogEx)

Tab_PEfileanalDialogbox::Tab_PEfileanalDialogbox(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_PEfileanalDialogbox::IDD, pParent)
	, m_entryPoint(_T(""))
	, m_baseAddres(_T(""))
	, m_codeStart(_T(""))
	, m_fileSize(_T(""))
	, m_fileAli(_T(""))
	, m_messageFlag(_T(""))
	, m_fileTime(_T(""))
	, m_textPath(_T(""))
	, m_blocks(_T(""))
	, m_childsystems(_T(""))
	, m_dataStarts(_T(""))
	, m_rvaNames(_T(""))
{

}

Tab_PEfileanalDialogbox::~Tab_PEfileanalDialogbox()
{
}

void Tab_PEfileanalDialogbox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_START, m_entryPoint);
	DDX_Text(pDX, IDC_BASEADDRESS, m_baseAddres);
	DDX_Text(pDX, IDC_CODESTART, m_codeStart);
	DDX_Text(pDX, IDC_FILESIZE, m_fileSize);
	DDX_Control(pDX, IDC_DATASTART, m_dataStart);
	DDX_Control(pDX, IDC_BLOCK, m_block);
	DDX_Control(pDX, IDC_CHILDSYSTEM, m_childSystem);
	DDX_Control(pDX, IDC_FLAG, m_flag);
	DDX_Text(pDX, IDC_FILEALI, m_fileAli);
	DDX_Control(pDX, IDC_RVANAME, m_rvaName);
	DDX_Text(pDX, IDC_MESSAGEFLAG, m_messageFlag);
	DDX_Text(pDX, IDC_FILETIME, m_fileTime);
	DDX_Text(pDX, IDC_EXEPATH, m_textPath);
	DDX_Text(pDX, IDC_BLOCK, m_blocks);
	DDX_Text(pDX, IDC_CHILDSYSTEM, m_childsystems);
	DDX_Text(pDX, IDC_DATASTART, m_dataStarts);
	DDX_Text(pDX, IDC_RVANAME, m_rvaNames);
}

BEGIN_MESSAGE_MAP(Tab_PEfileanalDialogbox, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON15, &Tab_PEfileanalDialogbox::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON19, &Tab_PEfileanalDialogbox::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON14, &Tab_PEfileanalDialogbox::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON18, &Tab_PEfileanalDialogbox::OnBnClickedButton18)
END_MESSAGE_MAP()

// Tab_PEfileanalDialogbox 消息处理程序

BOOL Tab_PEfileanalDialogbox::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;
}

// 响应拖拽PE信息
void Tab_PEfileanalDialogbox::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	// 1.获得拖拽数目
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);
	// 2.保存获取的路径信息
	char wcStr[MAX_PATH] = {};
	for (int i = 0; i < DropCount; i++)
	{
		// 3.记得清空字符串
		wcStr[0] = 0;
		// 4.获取路径名
		DragQueryFileA(hDropInfo, i, wcStr, MAX_PATH);
		m_textPath = wcStr;
	}
	// 5.把文件加载到内存
	m_lpBase = FileLoadMemory(wcStr);
	if (!m_lpBase)
	{
		AfxMessageBox(L"FileLoadMemory() faulier!");
		return;
	}
	if (!IsPEfile(m_lpBase))
	{
		AfxMessageBox(L"file not`s PE");
		return;
	}
	// 6.参数更新到变量
	ShowHeaderInfo(m_lpBase);
	// 7.更新到窗口上
	UpdateData(FALSE);
	// 8.释放内存
	DragFinish(hDropInfo);
	// 
	// SetBkColor();
	CDialogEx::OnDropFiles(hDropInfo);
}

// 内存加载文件
char* Tab_PEfileanalDialogbox::FileLoadMemory(char* lpFilePath)
{
	// 获取文件句柄
	HANDLE hFile = CreateFileA(lpFilePath, GENERIC_READ, FALSE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(L"CreateFileA() failuer!");
		exit(EXIT_FAILURE);
	}
	// 获取文件大小
	DWORD dwSize = GetFileSize(hFile, NULL);
	m_fileSize.Format(L"%d", dwSize);
	// 缓冲区
	m_lpBase = new char[dwSize]{};
	// 读文件
	DWORD dwCount = 1;
	BOOL  bRet = ReadFile(hFile, m_lpBase, dwSize, &dwCount, NULL);
	if (bRet){
		// 赋值给全局变量
		RVAtoFOA::g_NamelpBase = this->m_lpBase;
		return m_lpBase;
	}
	CloseHandle(hFile);
	delete m_lpBase;
	return 0;

}

// PE文件判断
BOOL Tab_PEfileanalDialogbox::IsPEfile(char* lpBase)
{
	// 1.获取MZ校验(e_magic)
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)this->m_lpBase;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
		return FALSE;
	// 2.获取PE校验(signature)
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + m_lpBase);
	if (pNt->Signature != IMAGE_NT_SIGNATURE)
		return FALSE;

	return TRUE;
}

// 显示PE头部信息
void Tab_PEfileanalDialogbox::ShowHeaderInfo(char* lpBase)
{
	DWORD swap;
	WORD swaps;
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpBase);
	// 入口点
	swap = pNt->OptionalHeader.AddressOfEntryPoint + pNt->OptionalHeader.ImageBase;
	m_entryPoint.Format(L"%08X", swap);
	// 基地址s
	swap = pNt->OptionalHeader.ImageBase;
	e_addrssbase = swap;
	m_baseAddres.Format(L"%08X", swap);
	// 区段个数
	swap = pNt->FileHeader.NumberOfSections;
	// 代码起始
	swap = pNt->FileHeader.TimeDateStamp;
	m_fileTime.Format(L"%d", swap);
	// 文件大小
	swap = pNt->OptionalHeader.SizeOfCode;
	m_fileTime.Format(L"%d", swap);
	// 文件对齐
	swap = pNt->OptionalHeader.FileAlignment;
	m_fileAli.Format(L"%X", swap);
	// 块对齐
	swap = pNt->OptionalHeader.SectionAlignment;
	m_blocks.Format(L"%X", swap);
	// 子系统
	swaps = pNt->OptionalHeader.Subsystem;
	m_childsystems.Format(L"%d", swaps);
	// 信息标志
	swaps = pNt->OptionalHeader.Magic;
	m_messageFlag.Format(L"%X", swaps);
	// 区段目录
	swap = pNt->FileHeader.NumberOfSections;
	m_codeStart.Format(L"%d", swap);
	// 数据起始
	swap = pNt->OptionalHeader.BaseOfData;
	m_dataStarts.Format(L"%08X", swap);
	// 数据目录个数
	swap = pNt->OptionalHeader.NumberOfRvaAndSizes;
	m_rvaNames.Format(L"%d", swap);
}

// 目录表信息
void Tab_PEfileanalDialogbox::OnBnClickedButton15()
{
	// 弹出目录表信息
	Tab_DirectoryTables direct;
	direct.m_lpBases = m_lpBase;
	direct.DoModal();
}

// 位置计算器(BUG修复)
void Tab_PEfileanalDialogbox::OnBnClickedButton19()
{
	if (0 == m_lpBase)
	{
		AfxMessageBox(L"请先拖拽文件");
		return;
	}
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + m_lpBase);
	// 公式  RVA(转) - RVA(区段) + OFFSET(区段) = OFFSET(文件)
	// 1.声明对象
	Tab_FilePosition tab_FOA;
	// 2.VR加载基址 = OEP + ImaBase
	tab_FOA.m_ImageBase = pNt->OptionalHeader.ImageBase + pNt->OptionalHeader.AddressOfEntryPoint;
	tab_FOA.DoModal();
}

// 区段表查看
void Tab_PEfileanalDialogbox::OnBnClickedButton14()
{
	// 弹出新窗口
	Tab_SectionView section;
	section.SetlpBase(m_lpBase);
	section.DoModal();
}

// 文件信息属性
void Tab_PEfileanalDialogbox::OnBnClickedButton18()
{
	UpdateData(TRUE);
	FileInfo getFileInfo;
	getFileInfo.cFilePath = m_textPath;
	getFileInfo.DoModal();
}
