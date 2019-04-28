// Tab_MemoryDialogbox.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_MemoryDialogbox.h"
#include "afxdialogex.h"
#include "Psapi.h"

// Tab_MemoryDialogbox 对话框

// 线程回调（解决CPU卡顿的问题
DWORD WINAPI ThreadProc(LPVOID lparameter)
{
	Tab_MemoryDialogbox *pDlg = (Tab_MemoryDialogbox*)lparameter;
	pDlg->GetCpuUsage();
	return 0;
}

IMPLEMENT_DYNAMIC(Tab_MemoryDialogbox, CDialogEx)

Tab_MemoryDialogbox::Tab_MemoryDialogbox(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_MemoryDialogbox::IDD, pParent)
	, m_Cpusyl(_T(""))
	, m_MemoryBFB(_T(""))
	, m_Pymemory(_T(""))
	, m_Pagesize(_T(""))
	, m_Memorysize(_T(""))
	, m_Kymemorysize(_T(""))
{

}

Tab_MemoryDialogbox::~Tab_MemoryDialogbox()
{
}

void Tab_MemoryDialogbox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtr);
	DDX_Text(pDX, IDC_CPUSYL, m_Cpusyl);
	DDX_Text(pDX, IDC_MEMORYBFB, m_MemoryBFB);
	DDX_Text(pDX, IDC_PHMEMORY, m_Pymemory);
	DDX_Text(pDX, IDC_PAGESIZE, m_Pagesize);
	DDX_Text(pDX, IDC_MEMORYSIZE, m_Memorysize);
	DDX_Text(pDX, IDC_KYMEMORYSIZE, m_Kymemorysize);
}


BEGIN_MESSAGE_MAP(Tab_MemoryDialogbox, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON8, &Tab_MemoryDialogbox::OnBnClickedButton8)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Tab_MemoryDialogbox 消息处理程序

BOOL Tab_MemoryDialogbox::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rc;
	m_ListCtr.GetClientRect(rc);
	int nWidth = rc.Width();
	DWORD dwOldsytle;
	dwOldsytle = m_ListCtr.GetExtendedStyle();
	m_ListCtr.SetExtendedStyle(dwOldsytle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ListCtr.InsertColumn(0, L"状态	", LVCFMT_CENTER, nWidth / 5);
	m_ListCtr.InsertColumn(1, L"BLOCK RVA", LVCFMT_CENTER, nWidth / 5);
	m_ListCtr.InsertColumn(2, L"内存类型", LVCFMT_CENTER, nWidth / 5);
	m_ListCtr.InsertColumn(3, L"内存分页属性", LVCFMT_CENTER, nWidth / 5);
	m_ListCtr.InsertColumn(4, L"块大小", LVCFMT_CENTER, nWidth / 5);
	// 显示虚拟内存信息
	queryViryualMemoryStatue(GetCurrentProcess());
	// 显示CPU使用率
	GetCpuUsage();
	// 显示虚拟内存信息
	GetMemoryInfo();
	// 更新变量到控件
	UpdateData(FALSE);
	// Settimer
	SetTimer(1, 1000, NULL);
	return TRUE; 
}

// 时间转换
double Tab_MemoryDialogbox::FILETIMEDouble(const _FILETIME & filetime)
{
	return double(filetime.dwHighDateTime * 4.294967296e9) + double(filetime.dwLowDateTime);
}

// 获取CPU的占用率
void Tab_MemoryDialogbox::GetCpuUsage()
{
	// 获取空闲时间 内核 用户
	_FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	// Creates or opens a named or unnamed event object.
	// 创建或打开一个命名的或无名的事件对象。
	// failure 0  | sucess handle
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// 等待1000毫秒，内核对象会更精确
	WaitForSingleObject(hEvent, 1000);
	// 获取新的时间
	_FILETIME newidleTime, newkernelTime, newuserTime;
	GetSystemTimes(&newidleTime, &newkernelTime, &newuserTime);
	// 转换时间
	double	doldidleTime = FILETIMEDouble(idleTime);
	double	doldkernelTime = FILETIMEDouble(kernelTime);
	double	dolduserTime = FILETIMEDouble(userTime);
	double	dnewidleTime = FILETIMEDouble(newidleTime);
	double	dnewkernelTime = FILETIMEDouble(newkernelTime);
	double	dnewuserTime = FILETIMEDouble(newuserTime);
	double	Times = dnewidleTime - doldidleTime;
	double	Kerneltime = dnewkernelTime - doldkernelTime;
	double	usertime = dnewuserTime - dolduserTime;
	// 计算使用率
	double Cpurate = (100.0 - Times / (Kerneltime + usertime) * 100.0);
	m_Cpusyl.Format(L"%0.2lf", Cpurate);
	m_Cpusyl += "%";
}

// 获取虚拟内存信息
void Tab_MemoryDialogbox::GetMemoryInfo()
{
	// 创建结构体对象 获取内存信息函数
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	// 当前占用率 Occupancy rate
	m_MemoryBFB.Format(L"%u", memStatus.dwMemoryLoad);
	m_MemoryBFB += "%";
	// 已使用物理内存大小 Physical memory size
	size_t memPhysize = memStatus.dwTotalPhys - memStatus.dwAvailPhys;
	m_Pymemory.Format(L"%u", (memPhysize / 1024 / 1024 / 8));
	m_Pymemory += " MB";
	// 文件交换大小 Size of the file exchange
	m_Pagesize.Format(L"%u", (memStatus.dwAvailPageFile / 1024 / 1024 / 8));
	m_Pagesize += " MB";
	// 虚拟内存大小 Virtual memory size
	m_Memorysize.Format(L"%u", (memStatus.dwTotalVirtual / 1024 / 1024 / 8));
	m_Memorysize += " MB";
	// 可用虚拟内存大小 Available virtual memory size
	m_Kymemorysize.Format(L"%d", (memStatus.dwAvailVirtual / 1024 / 1024 / 8));
	m_Kymemorysize += " MB";
}

// 一键优化内存
void Tab_MemoryDialogbox::OnBnClickedButton8()
{
	CString str, str1;
	str = "一键加速成功！ 节省了空间：  ";
	// 1. 获取当前已用物理内存状态
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	// 2. 清理内存
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); ++i)
	{
		HANDLE hProccess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProccess, -1, -1);
	}
	// 3. 获取清理后的内存状态
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG afterCleanUserdMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	// 4. 计算并弹出清理成功
	DWORDLONG CleanofSuccess = preUsedMem - afterCleanUserdMem;
	str1.Format(L"%d", (CleanofSuccess / 1024 / 1024 / 8));
	str = str + str1 + " MB";
	if (CleanofSuccess > 0)
		::MessageBox(NULL, str, L"加速成功▂▃▅▆█ ", 0);
}

// 获取虚内存遍历
void Tab_MemoryDialogbox::queryViryualMemoryStatue(HANDLE hProccess)
{
	m_ListCtr.DeleteAllItems();
	int count = 0;
	CString str;
	MEMORY_BASIC_INFORMATION	mbi = { 0 };
	VMINFO						statue = { 0 };
	DWORD						dwAddress = 0;
	DWORD						dwSize = 0;
	BOOL						bRet = FALSE;
	while (1) {

		bRet = VirtualQueryEx(hProccess,
			(LPCVOID)dwAddress,
			&mbi,
			sizeof(MEMORY_BASIC_INFORMATION));
		if (bRet == FALSE)
			break;

		statue.address = dwAddress;
		statue.state = (MEMORYSTATE)mbi.State;
		dwSize = mbi.RegionSize;

		// 输出内存状态, 
		// 内存状态用于描述虚拟内存有没有和物理存储器进行关联.
		// 或是否被预定.
		// free   : 闲置,没有预定,没有和物理存储器关联
		// reserve: 保留,被预定,没有和物理存储器关联
		// commit : 提交,已经和物理存储器关联
		switch (statue.state) {
		case e_stat_free:
			str.Format(L"free:          0x%08X", statue.address);
			m_ListCtr.InsertItem(count, str);
			break;
		case e_stat_reserve:
			str.Format(L"reserve:   0x%08X", statue.address);
			m_ListCtr.InsertItem(count, str);
			break;
		case e_stat_commit:
			str.Format(L"commit:   0x%08X", statue.address);
			m_ListCtr.InsertItem(count, str);
			break;
		}

		// 如果内存地址已经提交到物理内存,则遍历提交到的每一个内存块.
		if (statue.state == e_stat_commit) {

			dwSize = 0;
			LPVOID	dwAllocationBase = mbi.AllocationBase;
			DWORD	dwBlockAddress = (DWORD)dwAddress;
			while (1) {

				bRet = VirtualQueryEx(hProccess,
					(LPCVOID)dwBlockAddress,
					&mbi,
					sizeof(MEMORY_BASIC_INFORMATION));
				if (bRet == FALSE) {
					break;
				}


				// 判断遍历出来的内存块是否是同一块.(看它们的分配的首地址是否相等.)
				// 如果不是,则跳出循环.
				if (mbi.AllocationBase != dwAllocationBase)
					break;
				// 添加RVA
				str.Format(L"0x%08X", dwBlockAddress);
				m_ListCtr.SetItemText(count, 1, str);

				// 输出内存类型
				// 内存类型表示虚拟内存是以何种方式和物理存储器进行关联
				// image  : 是从影像文件中映射而来
				// mapped : 内存映射
				// private: 私有内存,其它进程无法访问.
				switch (mbi.Type) {
				case e_type_image:
					m_ListCtr.SetItemText(count, 2, L"image");
					break;
				case e_type_mapped:
					m_ListCtr.SetItemText(count, 2, L"mapped");
					break;
				case e_type_private:
					m_ListCtr.SetItemText(count, 2, L"private");
					break;
				default:
					break;
				}

				// 输出内存分页属性
				// 内存分页属性用于表示内存分页能够进行何种访问,如读,写,执行,写时拷贝.
				if (mbi.Protect == 0)
					m_ListCtr.SetItemText(count, 3, L"---");
				else if (mbi.Protect & PAGE_EXECUTE)
					m_ListCtr.SetItemText(count, 3, L"E--");
				else if (mbi.Protect & PAGE_EXECUTE_READ)
					m_ListCtr.SetItemText(count, 3, L"ER-");
				else if (mbi.Protect & PAGE_EXECUTE_READWRITE)
					m_ListCtr.SetItemText(count, 3, L"ERW");
				else if (mbi.Protect & PAGE_READONLY)
					m_ListCtr.SetItemText(count, 3, L"-R-");
				else if (mbi.Protect & PAGE_READWRITE)
					m_ListCtr.SetItemText(count, 3, L"-RW");
				else if (mbi.Protect & PAGE_WRITECOPY)
					m_ListCtr.SetItemText(count, 3, L"WCOPY");
				else if (mbi.Protect & PAGE_EXECUTE_WRITECOPY)
					m_ListCtr.SetItemText(count, 3, L"EWCOPY");

				// 输出内存块的大小.
				// printf(" 大小:0x%X\n", mbi.RegionSize);
				str.Format(L"0x%X", mbi.RegionSize);
				m_ListCtr.SetItemText(count, 4, str);
				// 索引到下一个内存块
				dwBlockAddress += mbi.RegionSize;

				// 累加内存块的大小
				dwSize += mbi.RegionSize;
			}
		}

		statue.size = dwSize;
		this->vmlist.push_back(statue);
		// 遍历下一块虚拟内存.
		dwAddress += dwSize;
		++count;
	}
}

// 每ms级别更新数据
void Tab_MemoryDialogbox::OnTimer(UINT_PTR nIDEvent)
{
	m_ListCtr.SetRedraw(FALSE);
	// 更新CPU使用率 写入线程回调
	HANDLE thProcces = CreateThread(NULL, NULL, ThreadProc, (LPVOID)this, NULL, NULL);
	// 更新虚拟内存数据
	GetMemoryInfo();
	// 虚拟内存遍历
	queryViryualMemoryStatue(GetCurrentProcess());
	//更新内容
	UpdateData(FALSE);
	m_ListCtr.SetRedraw(TRUE);
	m_ListCtr.Invalidate();
	m_ListCtr.UpdateWindow();
	// CloseHandle(thProcces);
	CDialogEx::OnTimer(nIDEvent);
}
