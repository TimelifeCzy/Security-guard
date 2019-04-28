// Tab_FilePosition.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_FilePosition.h"
#include "afxdialogex.h"
#include "RVAtoFOA.h"

// Tab_FilePosition 对话框

IMPLEMENT_DYNAMIC(Tab_FilePosition, CDialogEx)

Tab_FilePosition::Tab_FilePosition(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_FilePosition::IDD, pParent)
	, m_ImageBase(0)
	, m_vritualAddress(0)
	, m_offsetS(0)
	, m_editupdate(_T(""))
{

}

Tab_FilePosition::~Tab_FilePosition()
{
}

void Tab_FilePosition::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ImageBase);
	DDX_Text(pDX, IDC_EDIT2, m_vritualAddress);
	DDX_Text(pDX, IDC_EDIT3, m_offsetS);
	DDX_Text(pDX, IDC_SUCCESS, m_editupdate);
}


BEGIN_MESSAGE_MAP(Tab_FilePosition, CDialogEx)
	ON_BN_CLICKED(IDOK, &Tab_FilePosition::OnBnClickedOk)
END_MESSAGE_MAP()


// Tab_FilePosition 消息处理程序

BOOL Tab_FilePosition::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

// 按下执行的时候
void Tab_FilePosition::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	RVAtoFOA Getrva;
	_SAVERVAOFOFFSET obj = { 0 };;
	// 1.获取编辑框输入
	UpdateData(TRUE);
	// 2.VA + RVA = 偏移量   BUG 十进制相加 应该是16进制
	obj = Getrva.GetRvaofFOA(m_vritualAddress);
	// m_offsetS = m_ImageBase + m_vritualAddress;
	m_offsetS = obj.offset;
	m_editupdate += "成功";
	// 3.更新到控件
	UpdateData(FALSE);

	// CDialogEx::OnOK();
}

