// HeapInfoDialogBox.cpp : 实现文件
//

#include "stdafx.h"
#include "HeapInfoDialogBox.h"
#include "afxdialogex.h"
#include "TlHelp32.h"


// HeapInfoDialogBox 对话框

IMPLEMENT_DYNAMIC(HeapInfoDialogBox, CDialogEx)

HeapInfoDialogBox::HeapInfoDialogBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(HeapInfoDialogBox::IDD, pParent)
{

}

HeapInfoDialogBox::~HeapInfoDialogBox()
{
}

void HeapInfoDialogBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtr);
}


BEGIN_MESSAGE_MAP(HeapInfoDialogBox, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &HeapInfoDialogBox::OnLvnItemchangedList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &HeapInfoDialogBox::OnRclickList1)
END_MESSAGE_MAP()


// HeapInfoDialogBox 消息处理程序


BOOL HeapInfoDialogBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rc;
	m_ListCtr.GetClientRect(&rc);
	int nWindth = rc.Width();
	DWORD dwoldStyle;
	dwoldStyle = m_ListCtr.GetExtendedStyle();
	m_ListCtr.SetExtendedStyle(dwoldStyle | LVS_EX_FULLROWSELECT | LVS_EX_REGIONAL);
	m_ListCtr.InsertColumn(0, L"dwFlags", LVCFMT_CENTER, nWindth / 4);
	m_ListCtr.InsertColumn(1, L"th32HeapID", LVCFMT_CENTER, nWindth / 4);
	m_ListCtr.InsertColumn(2, L"dwSize", LVCFMT_CENTER, nWindth / 4);
	m_ListCtr.InsertColumn(3, L"th32ProcessID", LVCFMT_CENTER, nWindth / 4);
	// Get HeapInfo
	int ErrorCode = GetHeapInfo(this->m_ownerPid);
	if (ErrorCode == 0)
		MessageBox(L"GetHeaoInfo() failure");
	return TRUE;
}

// 获取堆信息数据
BOOL HeapInfoDialogBox::GetHeapInfo(DWORD pid)
{
	/*----------处理拒绝访问获取权限------------------*/
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	/*----------------------------------------------*/

	HEAPLIST32 he32 = { 0 };
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, pid);
	Heap32ListFirst(hSnap, &he32);
	int i = 0;
	do{
		// if (he32.th32ProcessID == pid)
		_stprintf_s(buf, MAX_PATH, L"%u", he32.dwFlags);
		m_ListCtr.InsertItem(i, buf);
		buf[0] = 0;
		_stprintf_s(buf, MAX_PATH, L"%u", he32.th32HeapID);
		m_ListCtr.SetItemText(i, 1, buf);
		buf[0] = 0;
		_stprintf_s(buf, MAX_PATH, L"%u", he32.dwSize);
		m_ListCtr.SetItemText(i, 2, buf);
		buf[0] = 0;
		_stprintf_s(buf, MAX_PATH, L"%u", he32.th32ProcessID);
		m_ListCtr.SetItemText(i, 3, buf);
		buf[0] = 0;
	} while (Heap32ListNext(hSnap, &he32));
	CloseHandle(hSnap);
	return 1;
}

void HeapInfoDialogBox::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

// 响应右击APP菜单
void HeapInfoDialogBox::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
}
