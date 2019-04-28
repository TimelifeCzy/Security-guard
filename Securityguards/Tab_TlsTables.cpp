// Tab_TlsTables.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_TlsTables.h"
#include "afxdialogex.h"
#include "RVAtoFOA.h"

// Tab_TlsTables 对话框

IMPLEMENT_DYNAMIC(Tab_TlsTables, CDialogEx)

Tab_TlsTables::Tab_TlsTables(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_TlsTables::IDD, pParent)
	, outputstr(_T(""))
{

}

Tab_TlsTables::~Tab_TlsTables()
{
}

void Tab_TlsTables::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditText);
	DDX_Text(pDX, IDC_EDIT1, outputstr);
}


BEGIN_MESSAGE_MAP(Tab_TlsTables, CDialogEx)
END_MESSAGE_MAP()


// Tab_TlsTables 消息处理程序


BOOL Tab_TlsTables::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	RVAtoFOA foa;
	// 1. 获取TLS表
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfarlc + m_lpBase);
	PIMAGE_DATA_DIRECTORY pData =  (PIMAGE_DATA_DIRECTORY)&pNt->OptionalHeader.DataDirectory[9];
	PIMAGE_TLS_DIRECTORY32 pTls = (PIMAGE_TLS_DIRECTORY32)(foa.RVAofFOAs(pData->VirtualAddress) + m_lpBase);

	// 2. 获取属性信息
	// 2.1 数据源起始的位置
	outputstr = "静态TLS结构体解析数据(TLS表)";
	outputstr += "\r\n";
	outputstr += "    PIAGE_TLS_DIRECTORY32\r\n";
	str.Format(L"%08X", pTls->StartAddressOfRawData);
	outputstr += "\tStartAddressOfRawData：";
	outputstr += ("0x" + str + "\r\n");
	// 2.2 数据源终止的位置
	str.Format(L"%08X", pTls->EndAddressOfRawData);
	outputstr += "\tEndAddressOfRawData：";
	outputstr += ("  0x" + str + "\r\n");
	// 2.3 保存TLS索引位置
	str.Format(L"%08X", pTls->AddressOfIndex);
	outputstr += "\tAddressOfIndex：";
	outputstr += ("               0x" + str + "\r\n");
	// 2.4 TLS回调函数的地址表位置
	str.Format(L"%08X", pTls->AddressOfCallBacks);
	outputstr += "\tAddressOfCallBacks：";
	outputstr += ("        0x" + str + "\r\n");
	// 2.5 TLS变量区域的大小
	str.Format(L"%08X", pTls->SizeOfZeroFill);
	outputstr += "\tSizeOfZeroFill：";
	outputstr += ("                   0x" + str + "\r\n");
	// 2.6 保留，恒为0
	str.Format(L"%08X", pTls->Characteristics);
	outputstr += "\tCharacteristics：";
	outputstr += ("                  0x" + str + "\r\n");
	UpdateData(FALSE);
	return TRUE;
}
