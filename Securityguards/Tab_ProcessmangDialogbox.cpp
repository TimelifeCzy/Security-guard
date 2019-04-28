// Tab_ProcessmangDialogbox.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_ProcessmangDialogbox.h"
#include "afxdialogex.h"
#include "ThreadDialogbox.h"
#include "HeapInfoDialogBox.h"
#include "ModuleInfoDialogBox.h"
#include "FileInfo.h"




TCHAR Tab_ProcessmangDialogbox::buff[MAX_PATH] = { 0 };
TCHAR Tab_ProcessmangDialogbox::buff1[MAX_PATH] = { 0 };
int Tab_ProcessmangDialogbox::count = 0;
CImageList* Tab_ProcessmangDialogbox::Icon = NULL;

// 窗口获取的回调函数
BOOL CALLBACK Tab_ProcessmangDialogbox::EnumWinProc(HWND hwnd, LPARAM lParam)
{
	// 定义对象调用控件变量
	Tab_ProcessmangDialogbox* p = (Tab_ProcessmangDialogbox *)lParam;
	// 获取窗口名字
	::GetWindowText(hwnd, buff, 200);
	// 窗口为真 且大小不为0 插入
	if ((::IsWindowVisible(hwnd) == TRUE) && (wcslen(buff) != 0))
	{
		// 获取进程id
		DWORD wPid;
		GetWindowThreadProcessId(hwnd, &wPid);
		// 打开进程获取句柄  ALL
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, wPid);
		// 获取文件路径
		TCHAR Path[MAX_PATH] = {};
		GetModuleFileNameEx(handle, NULL, Path, MAX_PATH);
		// 获取信息
		SHFILEINFOW shfileinfo;
		SHGetFileInfo(Path, 0, &shfileinfo, sizeof(SHFILEINFOW), SHGFI_ICON);
		// 初始化信息List列表
		INT index = Icon->Add(shfileinfo.hIcon);
		p->m_ListCtrApp.SetImageList(Icon, LVSIL_SMALL);
		p->m_ListCtrApp.InsertItem(count, buff, index);
		p->m_ListCtrApp.SetItemText(count, 1, L"正在运行");
		++count;
	}
	return 1;
}

IMPLEMENT_DYNAMIC(Tab_ProcessmangDialogbox, CDialogEx)

const TCHAR Tab_ProcessmangDialogbox::m_ListNameArr[g_MaxName][g_MaxName] = { L"ID", L"映像名称", L"父进程ID", L"基本优先级", L"线程数", L"路径" };

Tab_ProcessmangDialogbox::Tab_ProcessmangDialogbox(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_ProcessmangDialogbox::IDD, pParent)
{

}

Tab_ProcessmangDialogbox::~Tab_ProcessmangDialogbox()
{
}

void Tab_ProcessmangDialogbox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtr);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrApp);
}


BEGIN_MESSAGE_MAP(Tab_ProcessmangDialogbox, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &Tab_ProcessmangDialogbox::OnRclickList2)
ON_COMMAND(ID_40014, &Tab_ProcessmangDialogbox::CloseProcess)
ON_COMMAND(ID__40018, &Tab_ProcessmangDialogbox::Filelocation)
ON_COMMAND(ID_40015, &Tab_ProcessmangDialogbox::GetThreadInfo)
ON_COMMAND(ID__40019, &Tab_ProcessmangDialogbox::GetHeapInfoss)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Tab_ProcessmangDialogbox::OnLvnItemchangedList1)
ON_BN_CLICKED(IDC_BUTTON1, &Tab_ProcessmangDialogbox::OnBnClickedButton1)
ON_COMMAND(ID__40020, &Tab_ProcessmangDialogbox::GetModule)
ON_NOTIFY(NM_RCLICK, IDC_LIST1, &Tab_ProcessmangDialogbox::OnRclickList1)
ON_COMMAND(ID_40031, &Tab_ProcessmangDialogbox::AppList)
ON_COMMAND(ID_ATTRES, &Tab_ProcessmangDialogbox::OnAttres)
END_MESSAGE_MAP()


// Tab_ProcessmangDialogbox 消息处理程序

BOOL Tab_ProcessmangDialogbox::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 1.获取宽度
	CRect rc;
	this->GetClientRect(rc);
	int nWidth = rc.Width();
	// 2.获取/设置风格
	DWORD dwOldStyle;
	dwOldStyle = m_ListCtr.GetExtendedStyle();
	m_ListCtr.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 3.设置初始化名字
	for (int i = 0; i < g_MaxName; i++)
	{
		m_ListCtr.InsertColumn(i, m_ListNameArr[i], LVCFMT_CENTER, nWidth / g_MaxName);
	}
	// 4.获取进程信息
	this->GetProcessInfo();
	/*加载进程菜单信息*/
	men.LoadMenuW(IDR_MENU2);
	/*==========================================================*/
	// TODO:  在此添加额外的初始化
	dwOldStyle = m_ListCtrApp.GetExtendedStyle();
	m_ListCtrApp.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_REGIONAL);
	m_ListCtrApp.GetClientRect(&rc);
	nWidth = rc.Width();
	m_ListCtrApp.InsertColumn(0, L"任务", 0, nWidth / 2);
	m_ListCtrApp.InsertColumn(1, L"状态", 0, nWidth / 2);
	// 静态或者全局 然后传入this指针
	// 如何获取应用窗口小图标？
	// 初始化设置图片大小
	Icon = new CImageList;
	Icon->Create(25, 25, ILC_COLOR32, 0, 4);
	// 调用回调函数
	EnumWindows(EnumWinProc, (LPARAM)this);
	return TRUE;
}

// 获取进程
BOOL Tab_ProcessmangDialogbox::GetProcessInfo()
{
	m_ListCtr.DeleteAllItems();
	// 初始化无效的句柄值
	HANDLE hprocess = INVALID_HANDLE_VALUE;
	PROCESSENTRY32W p_32 = { 0 };
	// 1.创建进程快照
	hprocess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE != hprocess)
	{
		p_32.dwSize = sizeof(PROCESSENTRY32W);
		// 计算当前循环次数
		int count = 0;
		// 开始遍历进程
		if (!Process32First(hprocess, &p_32))
		{
			AfxMessageBox(L"Process32First falure!");
			CloseHandle(hprocess);
			return FALSE;
		}
		do
		{
			/*
			if (0 >= GetProcessImageFileName(hprocess, buf, MAX_PATH))
				AfxMessageBox(L"GetProcessImageFileName() failure!");
				*/
			GetProcessImageFileName(hprocess, buf, MAX_PATH);
			this->SetProcessInfo(&p_32, count, buf);
		} while (Process32Next(hprocess, &p_32));
	}
	else
	{
		AfxMessageBox(L"CreateToolhelp32Snapshot failure!");
		return FALSE;
	}
	return TRUE;
}

// 显示进程
void Tab_ProcessmangDialogbox::SetProcessInfo(const LPPROCESSENTRY32W P_32, const int count, const TCHAR* buf)
{
	TCHAR bufs[MAX_PATH] = {};
	// 进程ID
	_stprintf_s(bufs, MAX_PATH, L"%d", P_32->th32ProcessID);
	m_ListCtr.InsertItem(count, bufs);
	// 进程名
	m_ListCtr.SetItemText(count, 1, P_32->szExeFile);
	// 父进程ID
	_stprintf_s(bufs, MAX_PATH, L"%d", P_32->th32ParentProcessID);
	m_ListCtr.SetItemText(count, 2, bufs);
	// 基本优先级
	if (P_32->pcPriClassBase == 31)
		m_ListCtr.SetItemText(count, 3, L"实时(real-time)");
	else if (P_32->pcPriClassBase >= 15)
		m_ListCtr.SetItemText(count, 3, L"高(High)");
	else if (P_32->pcPriClassBase >= 11)
		m_ListCtr.SetItemText(count, 3, L"高于标准(above normal)");
	else if (P_32->pcPriClassBase >= 8)
		m_ListCtr.SetItemText(count, 3, L"标准(normal)");
	else if (P_32->pcPriClassBase >= 5)
		m_ListCtr.SetItemText(count, 3, L"低于标准(below Normal)");
	else if (P_32->pcPriClassBase >= 0)
		m_ListCtr.SetItemText(count, 3, L"最低(idle)");
	// 线程数
	_stprintf_s(bufs, MAX_PATH, L"%d", P_32->cntThreads);
	m_ListCtr.SetItemText(count, 4, bufs);
	// 路径
	m_ListCtr.SetItemText(count, 5, buf);
}

// 右击弹出菜单
void Tab_ProcessmangDialogbox::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// 菜单需要Pop模式（注：顶层菜单弹不出来），否则GetSubMenu是返回不了菜单句柄，返回为0;
	// 1.获取h_Menu句柄（加载到的菜单句柄）
	p = men.GetSubMenu(0);
	// 2.获取当前右击位置
	GetCursorPos(&point);
	// 3.显示一个浮动菜单
	p->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
}

// 结束进程
void Tab_ProcessmangDialogbox::CloseProcess()
{
	if (MessageBox(L"确定结束进程?", L"提示", MB_OKCANCEL | MB_ICONQUESTION))
	{
		// 1.获取当前选中行数
		int index = m_ListCtr.GetSelectionMark();
		// 2.获取进程ID
		CString str = m_ListCtr.GetItemText(index, 0);
		// 3.获取进程句柄
		int process_id = _ttol(str);
		HANDLE hprocess = OpenProcess(PROCESS_TERMINATE, FALSE, process_id);
		TerminateProcess(hprocess, 0);
		CloseHandle(hprocess);
	}
	else;
}

// 打开文件位置
void Tab_ProcessmangDialogbox::Filelocation()
{
	// 1.获取进程ID
	int index = m_ListCtr.GetSelectionMark();
	CString str = m_ListCtr.GetItemText(index, 0);
	// 2.获取进程句柄
}

// 获取线程
void Tab_ProcessmangDialogbox::GetThreadInfo()
{
	// 调用窗口
	ThreadDialogbox threadobj;
	int index = m_ListCtr.GetSelectionMark();
	CString str = m_ListCtr.GetItemText(index, 0);
	threadobj.m_ownperProcess = _ttol(str);
	threadobj.DoModal();
}

// 获取堆
void Tab_ProcessmangDialogbox::GetHeapInfoss()
{
	// TODO:  在此添加命令处理程序代码
	HeapInfoDialogBox heapObj;
	int index = m_ListCtr.GetSelectionMark();
	CString str = m_ListCtr.GetItemText(index, 0);
	heapObj.m_ownerPid = _ttol(str);
	heapObj.DoModal();
}

// 获取模块
void Tab_ProcessmangDialogbox::GetModule()
{
	ModuleInfoDialogBox moduleObj;
	int index = m_ListCtr.GetSelectionMark();
	CString str = m_ListCtr.GetItemText(index, 0);
	moduleObj.m_ownerPid = _ttol(str);
	moduleObj.DoModal();
}

// lISTAPPCtrol
void Tab_ProcessmangDialogbox::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

// 刷新全部按钮
void Tab_ProcessmangDialogbox::OnBnClickedButton1()
{
	// -----------****更新窗口APP***------------------
	count = 0;
	m_ListCtrApp.DeleteAllItems();
	EnumWindows(EnumWinProc, (LPARAM)this);

	// -----------****进程APP***------------------
	GetProcessInfo();
}

// APP右击弹出菜单
void Tab_ProcessmangDialogbox::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu PopMen;
	PopMen.LoadMenuW(IDR_MENU4);
	POINT point;
	GetCursorPos(&point);
	CMenu *p = PopMen.GetSubMenu(0);
	p->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

// 响应App桌面进程跳转
void Tab_ProcessmangDialogbox::AppList()
{
	CString str;
	DWORD nPid = 0;
	// 1. 获取当前行内容
	int index = m_ListCtrApp.GetSelectionMark();
	str = m_ListCtrApp.GetItemText(index, 0);
	int k = m_ListCtrApp.GetSelectionMark();
	m_ListCtr.SetItemState(k, 0, LVIS_SELECTED | LVIS_FOCUSED);

	// 2.获取当前进程的句柄
	CWnd *pwnd = FindWindow(NULL, str);
	HWND hApp = pwnd->GetSafeHwnd();
	GetWindowThreadProcessId(hApp, &nPid);

	// 3.循环遍历mList中PID  找到相符合光标设置到该行
	int nCount = m_ListCtr.GetItemCount();
	for (int i = 0; i < nCount; ++i)
	{
		str = m_ListCtr.GetItemText(i, 0);
		DWORD ProccessPid = wcstol(str, NULL, 10);
		if (ProccessPid == nPid)
		{
			LVITEMINDEX aaaa = { i, 0 };
			// 先取消选中另一个
			m_ListCtrApp.SetItemState(index, 0, LVIS_SELECTED | LVIS_FOCUSED);
			// 设置光标到该行
			m_ListCtr.SetFocus();
			m_ListCtr.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			return;
		}
	}
}

// 响应文件属性
void Tab_ProcessmangDialogbox::OnAttres()
{
	DWORD nPid = 0;
	CString str;
	// 1. 获取当前名称
	int index = m_ListCtrApp.GetSelectionMark();
	str = m_ListCtrApp.GetItemText(index, 0);
	// 2.获取当前进程的句柄
	CWnd *pwnd = FindWindow(NULL, str);
	HWND hApp = pwnd->GetSafeHwnd();
	GetWindowThreadProcessId(hApp, &nPid);
	// 3. 打开进程获取句柄  ALL
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);
	// 4. 获取文件路径
	TCHAR Path[MAX_PATH] = {};
	GetModuleFileNameEx(handle, NULL, Path, MAX_PATH);
	// 5. 获取文件路径
	FileInfo file;
	str = Path;
	file.cFilePath = str;
	file.DoModal();
}
