// Tab_SoftwarmangDialogbox.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_SoftwarmangDialogbox.h"
#include "afxdialogex.h"


// Tab_SoftwarmangDialogbox 对话框

IMPLEMENT_DYNAMIC(Tab_SoftwarmangDialogbox, CDialogEx)

Tab_SoftwarmangDialogbox::Tab_SoftwarmangDialogbox(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_SoftwarmangDialogbox::IDD, pParent)
{

}

Tab_SoftwarmangDialogbox::~Tab_SoftwarmangDialogbox()
{
}

void Tab_SoftwarmangDialogbox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ServicesoList);
	DDX_Control(pDX, IDC_LIST2, m_ServerInfo);
}

BEGIN_MESSAGE_MAP(Tab_SoftwarmangDialogbox, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Tab_SoftwarmangDialogbox::OnLvnItemchangedList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &Tab_SoftwarmangDialogbox::OnClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &Tab_SoftwarmangDialogbox::OnRclickList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &Tab_SoftwarmangDialogbox::OnLvnItemchangedList2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &Tab_SoftwarmangDialogbox::OnRclickList2)
	ON_COMMAND(ID_40036, &Tab_SoftwarmangDialogbox::StartService)
	ON_COMMAND(ID_40037, &Tab_SoftwarmangDialogbox::StopService)
	ON_COMMAND(ID_40038, &Tab_SoftwarmangDialogbox::SofwUninstall)
	ON_BN_CLICKED(IDC_BUTTON1, &Tab_SoftwarmangDialogbox::OnBnClickedButton1)
	ON_WM_HOTKEY()
END_MESSAGE_MAP()

// Tab_SoftwarmangDialogbox 消息处理程序

BOOL Tab_SoftwarmangDialogbox::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rc;
	m_ServicesoList.GetClientRect(rc);
	int nWidth = rc.Width();
	DWORD dwOldStyle;
	dwOldStyle = m_ServicesoList.GetExtendedStyle();
	m_ServicesoList.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ServicesoList.InsertColumn(0, L"全部软件", LVCFMT_CENTER, nWidth);
	m_ServicesoList.InsertItem(0, L"全部软件");
	m_ServicesoList.InsertItem(1, L"最近安装");
	m_ServicesoList.InsertItem(2, L"不常用软件");
	m_ServicesoList.InsertItem(3, L"系统服务");
	/*====================================================================================*/
	// 初始化软件风格
	ListStyleFuns();
	return TRUE;
}

// 测试List1
void Tab_SoftwarmangDialogbox::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

// 响应鼠标List选中消息
void Tab_SoftwarmangDialogbox::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString str;
	// 1. 获取选中文本小
	int index = m_ServicesoList.GetSelectionMark();
	str = m_ServicesoList.GetItemText(index, 0);
	// 1.1 全部软件
	if ("全部软件" == str)
	{
		m_ServerInfo.DeleteAllItems();
		this->pGetofShowAllSofwInfo();
		return;
	}
	//// 1.2 最近安装
	//else if ("最近安装" == str)
	//{
	//	this->pGetofShowZJSofwInfo();
	//	return;
	//}
	//// 1.3 不常用软件
	//else if ("不常用软件" == str)
	//{
	//	this->pGetofShowBCYSofwInfo();
	//	return;
	//}
	// 1.4 系统服务
	else if ("系统服务" == str)
	{
		m_ServerInfo.DeleteAllItems();
		this->pGetofShowSystemService();
		return;
	}
	else
		AfxMessageBox(L"好像么有选中！");
	*pResult = 0;
}

// 获取显示全部软件信息
BOOL Tab_SoftwarmangDialogbox::GetofShowAllSofwInfo()
{
	ListStyleFuns();
	SOFTINFO SoftInfo = { 0 };
	FILETIME ftLastWriteTimeA;					// last write time 
	// 1. 打开一个已存在的注册表键
	LONG LReturn = RegOpenKeyEx(RootKey, lpSubKey, 0, KEY_ENUMERATE_SUB_KEYS | KEY_WOW64_32KEY | KEY_QUERY_VALUE, &hkResult);
	// 2. 计算大小注册表
	// TCHAR    achKey[MAX_PATH] = {};			// buffer for subkey name
	DWORD    cbName = 0;						// size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");		// buffer for class name 
	DWORD    cchClassName = MAX_PATH;			// size of class string 
	DWORD    cSubKeys = 0;						// number of subkeys 
	DWORD    cbMaxSubKey;						// longest subkey size 
	DWORD    cchMaxClass;						// longest class string 
	DWORD    cValues;							// number of values for key 
	DWORD    cchMaxValue;						// longest value name 
	DWORD    cbMaxValueData;					// longest value data 
	DWORD    cbSecurityDescriptor;				// size of security descriptor 
	FILETIME ftLastWriteTime;					// last write time 
	DWORD	retCode;
	// TCHAR  achValue[MAX_PATH] = {};
	DWORD cchValue = MAX_PATH;
	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hkResult,                // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 
	// 3. 循环遍历Uninstall目录下的子健
	int nCount = 1;
	DWORD dwIndex = 0;
	DWORD dwKeyLen = 255;
	DWORD dwType = 0;
	WCHAR szNewKeyName[MAX_PATH] = {};		// 注册表名称
	WCHAR strMidReg[MAX_PATH] = {};
	DWORD dwNamLen = 255;					// 获取键值
	HKEY hkValueKey = 0;
	LONG lRrturn = ERROR_SUCCESS;
	for (SIZE_T i = 0; i < cSubKeys; i++)
	{
		dwKeyLen = MAX_PATH;
		lRrturn = RegEnumKeyEx(hkResult, dwIndex, szNewKeyName, &dwKeyLen, 0, NULL, NULL, &ftLastWriteTimeA);
		// 2.1 通过得到子健的名称重新组合成新的子健路径
		swprintf_s(strMidReg, L"%s%s%s", lpSubKey, L"\\", szNewKeyName);
		// 2.2 打开新的子健, 获取其句柄
		RegOpenKeyEx(RootKey, strMidReg, 0, KEY_QUERY_VALUE, &hkValueKey);
		// 名字
		dwNamLen = 255;
		RegQueryValueEx(hkValueKey, L"DisplayName", 0, &dwType, (LPBYTE)SoftInfo.szSoftName, &dwNamLen);
		m_ServerInfo.InsertItem(nCount, SoftInfo.szSoftName);
		// 版本号
		dwNamLen = 255;
		RegQueryValueEx(hkValueKey, L"VersionNumber", 0, &dwType, (LPBYTE)SoftInfo.szSoftVer, &dwNamLen);
		m_ServerInfo.SetItemText(nCount, 3, SoftInfo.szSoftVer);
		// 安装时间
		dwNamLen = 255;
		RegQueryValueEx(hkValueKey, L"Time", 0, &dwType, (LPBYTE)SoftInfo.szSoftDate, &dwNamLen);
		m_ServerInfo.SetItemText(nCount, 2, SoftInfo.szSoftDate);
		// 大小
		dwNamLen = 255;
		RegQueryValueEx(hkValueKey, L"Sizeof", 0, &dwType, (LPBYTE)SoftInfo.szSoftSize, &dwNamLen);
		m_ServerInfo.SetItemText(nCount, 4, SoftInfo.szSoftSize);
		// 发布商
		dwNamLen = 255;
		RegQueryValueEx(hkValueKey, L"Sizeof", 0, &dwType, (LPBYTE)SoftInfo.strSoftVenRel, &dwNamLen); 
		// 卸载路径
		dwNamLen = 255;
		RegQueryValueEx(hkValueKey, L"UninstallString", 0, &dwType, (LPBYTE)SoftInfo.strSoftUniPath, &dwNamLen);
		m_ServerInfo.SetItemText(nCount, 5, SoftInfo.strSoftUniPath);
		dwNamLen = 255;
		++dwIndex;
	}
	return TRUE;
}

// 获取显示系统服务信息
BOOL Tab_SoftwarmangDialogbox::GetofShowSystemService()
{
	ListStyleFun();
	// 1. 打开远程计算机服务器控制管理器
	SC_HANDLE hSCMs = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	// 2. 第一次调用，需要获取内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCMs, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);
	// 3. 申请需要的内存， 第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	// 4. 第二次枚举
	BOOL bStatus = FALSE;
	bStatus = EnumServicesStatusEx(hSCMs, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, (PBYTE)pEnumService, dwSize, &dwSize, &dwServiceNum, NULL, NULL);
	// 5. 遍历信息
	for (DWORD i = 0; i < dwServiceNum; ++i)
	{
		// 5.1 名称
		m_ServerInfo.InsertItem(i, pEnumService[i].lpServiceName);
		// 5.2 状态
		switch (pEnumService[i].ServiceStatusProcess.dwCurrentState)
		{
			// 停止-->为空
		case SERVICE_STOPPED:
			m_ServerInfo.SetItemText(i, 2, L" ");
			break;
			// 已启动
		case SERVICE_RUNNING:
			m_ServerInfo.SetItemText(i, 2, L"已启动");
			break;
			// 暂停
		case SERVICE_PAUSED:
			m_ServerInfo.SetItemText(i, 2, L"暂停");
			break;
		}
		// 获取服务的详细信息
		SC_HANDLE hServices = OpenService(hSCMs, pEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		// 第一次调用缓冲区大小
		QueryServiceConfig(hServices, NULL, 0, &dwSize);
		// 分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		// 第二次调用， 获取信息
		QueryServiceConfig(hServices, pServiceConfig, dwSize, &dwSize);
		// 5.3 设备类型（描述信息）
		DWORD sType = pServiceConfig->dwServiceType;
		switch (sType)
		{
			// 文件系统驱动服务
		case SERVICE_FILE_SYSTEM_DRIVER:
			m_ServerInfo.SetItemText(i, 1, L"文件系统驱动服务");
			break;
			// 驱动程序服务
		case SERVICE_KERNEL_DRIVER:
			m_ServerInfo.SetItemText(i, 1, L"驱动程序服务");
			break;
			// 本进程服务
		case SERVICE_WIN32_OWN_PROCESS:
			m_ServerInfo.SetItemText(i, 1, L"本进程服务");
			break;
			// 0x0000020服务
		case SERVICE_WIN32_SHARE_PROCESS:
			m_ServerInfo.SetItemText(i, 1, L"0x0000020 服务");
			break;
		}
		// 5.4 启动类型
		DWORD startType = pServiceConfig->dwStartType;
		switch (startType)
		{
			// 自动
		case SERVICE_AUTO_START:
			m_ServerInfo.SetItemText(i, 3, L"自动");
			break;
			// 自动(延迟启动)
		case SERVICE_BOOT_START:
			m_ServerInfo.SetItemText(i, 3, L"自动(延迟启动)");
			break;
			// 
		case SERVICE_DEMAND_START:
			m_ServerInfo.SetItemText(i, 3, L"手动");
			break;
			// 禁用
		case SERVICE_DISABLED:
			m_ServerInfo.SetItemText(i, 3, L"禁用");
			break;
		default:
			break;
		}
		// 5.5 获取路径信息
		m_ServerInfo.SetItemText(i, 5, pServiceConfig->lpBinaryPathName);
		// 5.6 登录为
		m_ServerInfo.SetItemText(i, 4, pServiceConfig->lpServiceStartName);
	}
	return FALSE;
}

// 系统风格
void Tab_SoftwarmangDialogbox::ListStyleFun()
{
	int count = m_ServerInfo.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < count; ++i)
	{
		m_ServerInfo.DeleteColumn(0);
	}
	CRect rc;
	CString str;
	// DWORD dwOldStyle;
	// m_ServerInfo.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ServerInfo.GetClientRect(rc);
	int nWidth = rc.Width();
	m_ServerInfo.InsertColumn(0, L"名称", 0, nWidth / 6);
	m_ServerInfo.InsertColumn(1, L"描述", 0, nWidth / 6);
	m_ServerInfo.InsertColumn(2, L"状态", 0, nWidth / 6);
	m_ServerInfo.InsertColumn(3, L"启动类型", 0, nWidth / 6);
	m_ServerInfo.InsertColumn(4, L"登录为", 0, nWidth / 6);
	m_ServerInfo.InsertColumn(5, L"路径", 0, nWidth / 6);
}

// 软件减
void Tab_SoftwarmangDialogbox::ListStyleFuns()
{
	int count = m_ServerInfo.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < count; ++i)
	{
		m_ServerInfo.DeleteColumn(0);
	}
	CRect rc;
	m_ServerInfo.GetClientRect(rc);
	int nWidth = rc.Width();
	m_ServerInfo.InsertColumn(0, L"软件名称", LVCFMT_CENTER, nWidth / 6);
	m_ServerInfo.InsertColumn(1, L"发布者", LVCFMT_CENTER, nWidth / 6);
	m_ServerInfo.InsertColumn(2, L"安装时间", LVCFMT_CENTER, nWidth / 6);
	m_ServerInfo.InsertColumn(3, L"大小", LVCFMT_CENTER, nWidth / 6);
	m_ServerInfo.InsertColumn(4, L"版本", LVCFMT_CENTER, nWidth / 6);
	m_ServerInfo.InsertColumn(5, L"卸载路径", LVCFMT_CENTER, nWidth / 6);
}

// 右击响应(全部软件)
void Tab_SoftwarmangDialogbox::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	AfxMessageBox(L"别右击了没啥用！");
	*pResult = 0;
}

void Tab_SoftwarmangDialogbox::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

// 右击响应弹出卸载菜单
void Tab_SoftwarmangDialogbox::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu PopMen;
	PopMen.LoadMenuW(IDR_MENU5);
	POINT point;
	GetCursorPos(&point);
	CMenu *p = PopMen.GetSubMenu(0);
	p->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

// 启动服务
void Tab_SoftwarmangDialogbox::StartService()
{
	// 1. 获取服务名称
	int index = m_ServerInfo.GetSelectionMark();
	CString str = m_ServerInfo.GetItemText(index, 0);
	// 2. 打开服务管理器（拿到当前名称服务）
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	SC_HANDLE hSvc = ::OpenService(hSC, str,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		TRACE("Open www erron。");
		::CloseServiceHandle(hSC);
		return;
	}
	// 3. 获取服务状态
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		TRACE("Get Service state error。");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	// 4. 启动服务
	if (status.dwCurrentState == SERVICE_STOPPED)
	{
		if (::StartService(hSvc, NULL, NULL) == FALSE)
		{
			TRACE("服务可能已启动");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// 等待服务启动
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				AfxMessageBox(L"启动服务成功");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
}

// 关闭服务
void Tab_SoftwarmangDialogbox::StopService()
{
	// 1. 获取服务名称
	int index = m_ServerInfo.GetSelectionMark();
	CString str = m_ServerInfo.GetItemText(index, 0);
	// 2. 打开服务管理器（拿到当前名称服务）
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	SC_HANDLE hSvc = ::OpenService(hSC, str,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		TRACE("Open www erron。");
		::CloseServiceHandle(hSC);
		return;
	}
	// 3. 获取服务状态
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		TRACE("Get Service state error。");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	//如果处于停止状态则启动服务，否则停止服务。
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// 停止服务
		if (::ControlService(hSvc,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			TRACE("stop service error。");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// 等待服务停止
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				AfxMessageBox(L"服务关闭成功");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
}

// 卸载软件
void Tab_SoftwarmangDialogbox::SofwUninstall()
{
	// TODO:  在此添加命令处理程序代码
	char Path[MAX_PATH] = { 0 };
	int index = m_ServerInfo.GetSelectionMark();
	CString strPath = m_ServerInfo.GetItemText(index, 5);
	WideCharToMultiByte(CP_ACP, NULL, strPath, strPath.GetLength(), Path, sizeof(Path), NULL, NULL);
	WinExec(Path, SW_SHOW);
}

// 刷新服务
void Tab_SoftwarmangDialogbox::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	GetofShowAllSofwInfo();
	GetofShowSystemService();
}


void Tab_SoftwarmangDialogbox::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
