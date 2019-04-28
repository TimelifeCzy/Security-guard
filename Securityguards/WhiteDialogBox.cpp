// WhiteDialogBox.cpp : 实现文件
//

#include "stdafx.h"
#include "WhiteDialogBox.h"
#include "afxdialogex.h"


// WhiteDialogBox 对话框

vector<CString> WhiteDialogBox::WhiteName = { };

IMPLEMENT_DYNAMIC(WhiteDialogBox, CDialogEx)

WhiteDialogBox::WhiteDialogBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(WhiteDialogBox::IDD, pParent)
	, m_WhiteEdit(_T(""))
{

}

WhiteDialogBox::~WhiteDialogBox()
{
}

void WhiteDialogBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_WhiteEdit);
	DDX_Control(pDX, IDC_LIST1, m_ListArr);
}

BEGIN_MESSAGE_MAP(WhiteDialogBox, CDialogEx)
	ON_BN_CLICKED(IDOK, &WhiteDialogBox::OnBnClickedOk)
	ON_BN_CLICKED(IDC_WHITEBUUT, &WhiteDialogBox::OnBnClickedCancel)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// WhiteDialogBox 消息处理程序

BOOL WhiteDialogBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 1.获取宽度
	CRect rc;
	this->GetClientRect(rc);
	int nWidth = rc.Width();
	// 2.获取/设置风格
	DWORD dwOldStyle;
	dwOldStyle = m_ListArr.GetExtendedStyle();
	m_ListArr.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListArr.InsertColumn(0, L"白名单（免杀）", 0, nWidth / 1);
	UpdateListName();
	return TRUE;
}

// 更新List
void WhiteDialogBox::UpdateListName()
{
	m_ListArr.DeleteAllItems();
	for (SIZE_T i = 0; i < WhiteName.size(); ++i)
	{
		m_ListArr.InsertItem(i, WhiteName[i].GetBuffer(0));
	}
}

// 添加更新
void WhiteDialogBox::OnBnClickedOk()
{
	UpdateData(TRUE);
	UpdateListName();
	m_WhiteEdit = " ";
	UpdateData(FALSE);

	// CDialogEx::OnOK();
}

// 删除按钮
void WhiteDialogBox::OnBnClickedCancel()
{
	int index = m_ListArr.GetSelectionMark();
	CString strName = m_ListArr.GetItemText(index, 0);
	auto nDelete = find(WhiteName.begin(), WhiteName.end(), strName);
	if (nDelete != WhiteName.end())
		WhiteName.erase(nDelete);
	UpdateListName();
	// CDialogEx::OnCancel();
}

// 响应文件拖拽
void WhiteDialogBox::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CString str;
	// 1. 获取文件拖拽数目 如果第二个参数为-1则返回一个计数文件删除
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);
	// 2. 循环获取的每一个文件
	for (int i = 0; i < DropCount; ++i)
	{
		WCHAR wcStr[MAX_PATH] = { 0 };
		// 3. 获取拖拽第i个文件名
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);
		// 4. 不可以重复添加文件
		if (find(WhiteName.begin(), WhiteName.end(), wcStr) == WhiteName.end())
			WhiteName.push_back(wcStr);
		m_WhiteEdit.Append(wcStr);
	}
	UpdateData(FALSE);
	// 4. 释放内存
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}
