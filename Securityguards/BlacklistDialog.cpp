// BlacklistDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "BlacklistDialog.h"
#include "afxdialogex.h"


// BlacklistDialog 对话框

vector<CString> BlacklistDialog::nBlackList = {};

IMPLEMENT_DYNAMIC(BlacklistDialog, CDialogEx)

BlacklistDialog::BlacklistDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(BlacklistDialog::IDD, pParent)
	, m_BlackList(_T(""))
{

}

BlacklistDialog::~BlacklistDialog()
{
}

void BlacklistDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_blacklsit);
	DDX_Text(pDX, IDC_EDIT1, m_BlackList);
}


BEGIN_MESSAGE_MAP(BlacklistDialog, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &BlacklistDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &BlacklistDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// BlacklistDialog 消息处理程序


BOOL BlacklistDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 1.获取宽度
	CRect rc;
	this->GetClientRect(rc);
	int nWidth = rc.Width();
	// 2.获取/设置风格
	DWORD dwOldStyle;
	dwOldStyle = m_blacklsit.GetExtendedStyle();
	m_blacklsit.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_blacklsit.InsertColumn(0, L"黑名单（必杀）", 0, nWidth / 1);
	UpdateList();
	return TRUE;
}

// 响应拖拽消息
void BlacklistDialog::OnDropFiles(HDROP hDropInfo)
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
		if (find(nBlackList.begin(), nBlackList.end(), wcStr) == nBlackList.end())
			nBlackList.push_back(wcStr);
		m_BlackList.Append(wcStr);
	}
	UpdateData(FALSE);
	// 4. 释放内存
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}

// 更新黑名单列表
void BlacklistDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	UpdateList();
	m_BlackList = " ";
	UpdateData(FALSE);
	// CDialogEx::OnOK();
}

// 删除黑名单列表
void BlacklistDialog::OnBnClickedCancel()
{
	CString strName;
	int index = m_blacklsit.GetSelectionMark();
	strName = m_blacklsit.GetItemText(index, 0);
	auto nDelete = find(nBlackList.begin(), nBlackList.end(), strName);
	nBlackList.erase(nDelete);
	// 删除完之后更新列表
	UpdateList();
	// CDialogEx::OnCancel();
}

// 封装
void BlacklistDialog::UpdateList()
{
	for (size_t i = 0; i < nBlackList.size(); ++i)
	{
		m_blacklsit.InsertItem(i, nBlackList[i].GetBuffer(0));
	}
}
