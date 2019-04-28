// Tab_LDCS.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_LDCS.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MD5.h"
#include <atlconv.h>
#include "WhiteDialogBox.h"
#include "BlacklistDialog.h"
#include "StructDataofStream.h"
#include "MFCsockclass.h"

// Tab_LDCS 对话框

int Tab_LDCS::Flag = 0;

CString Tab_LDCS::strFindNamPathp[5] = { "" };
int Tab_LDCS::nCounts = 0;
// 73a1227aa8834557a2af6e12192d1840
CString Tab_LDCS::m_MDEValues = "7c0531dc2c5911130e5d7de39b813766";
int Tab_LDCS::nCountList = 0;

// 线程回调（本地查杀）
DWORD WINAPI ThreadProcLDCS(LPVOID lparameter)
{
	Tab_LDCS *pDlg = (Tab_LDCS*)lparameter;
	if ((1 == Tab_LDCS::Flag) || (3 == Tab_LDCS::Flag))
	{
		for (int i = 0; i < Tab_LDCS::nCounts; ++i)
		{
			pDlg->FindFile(Tab_LDCS::strFindNamPathp[i], Tab_LDCS::m_MDEValues);
		}
		return 0;
	}
	if (2 == Tab_LDCS::Flag)
	{
		// pDlg->m_PathListCtr.DeleteAllItems();
		pDlg->FindFile(L"C:\\项目演示\\", Tab_LDCS::m_MDEValues);
		AfxMessageBox(L"查杀完毕，请尽快处理病毒");
		return 0;
	}
	return 0;
}

IMPLEMENT_DYNAMIC(Tab_LDCS, CDialogEx)

Tab_LDCS::Tab_LDCS(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_LDCS::IDD, pParent)
	, m_buttQP(0)
	, m_NetworkStatus(_T(""))
	, m_Editvalues(_T(""))
	// mdValues = L""0c2c1f16c6cdd97492599755ed3c8267
	, m_MDEditValse(_T("73a1227aa8834557a2af6e12192d1840"))
{

}

Tab_LDCS::~Tab_LDCS()
{
}

void Tab_LDCS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, m_PathListCtr);
	DDX_Control(pDX, IDC_LIST6, m_ListMessageInfo);
	DDX_Radio(pDX, IDC_RADIO1, m_buttQP);
	DDX_Control(pDX, IDC_RADIO1, m_ButtCX);
	DDX_Text(pDX, IDC_NETWORKSTATIC, m_NetworkStatus);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_CMFeditBriwse);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_Editvalues);
	DDX_Text(pDX, IDC_EDIT1, m_MDEditValse);
	// DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
}

BEGIN_MESSAGE_MAP(Tab_LDCS, CDialogEx)
	ON_BN_CLICKED(IDOK, &Tab_LDCS::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &Tab_LDCS::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &Tab_LDCS::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &Tab_LDCS::OnBnClickedRadio3)
	ON_COMMAND(IDC_RADIO2, &Tab_LDCS::OnRadio2)
	ON_COMMAND(IDC_RADIO3, &Tab_LDCS::OnRadio3)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &Tab_LDCS::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &Tab_LDCS::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &Tab_LDCS::OnBnClickedButton6)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST5, &Tab_LDCS::OnLvnItemchangedList5)
	ON_NOTIFY(NM_RCLICK, IDC_LIST5, &Tab_LDCS::OnRclickList5)
	ON_COMMAND(ID_40043, &Tab_LDCS::On40043)
	ON_BN_CLICKED(IDC_BUTTON3, &Tab_LDCS::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &Tab_LDCS::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &Tab_LDCS::OnBnClickedButton5)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &Tab_LDCS::OnEnChangeMfceditbrowse1)
	ON_BN_CLICKED(IDC_BUTTON7, &Tab_LDCS::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &Tab_LDCS::OnBnClickedButton8)
	ON_COMMAND(ID_40042, &Tab_LDCS::On40042)
	ON_STN_CLICKED(IDC_VALUESTAT, &Tab_LDCS::OnStnClickedValuestat)
END_MESSAGE_MAP()

// 设置风格（查杀路径）
void Tab_LDCS::SetStylePathList()
{
	m_PathListCtr.DeleteAllItems();
	m_PathListCtr.DeleteColumn(0);
	m_PathListCtr.GetClientRect(rc);
	int nWdith = rc.Width();
	dwOldStyle = m_PathListCtr.GetExtendedStyle();
	m_PathListCtr.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_PathListCtr.InsertColumn(0, L"☻ 查杀路径", LVCFMT_CENTER, nWdith);
}

// 设置可疑文件ListCtr风格
void Tab_LDCS::SetSaveSuspectedFileList()
{
	m_PathListCtr.DeleteAllItems();
	m_PathListCtr.DeleteColumn(0);
	m_PathListCtr.GetClientRect(rc);
	int nWdith = rc.Width();
	dwOldStyle = m_PathListCtr.GetExtendedStyle();
	m_PathListCtr.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_PathListCtr.InsertColumn(0, L"㊝ 可疑文件！", LVCFMT_CENTER, nWdith);
}

// Tab_LDCS 消息处理程序
BOOL Tab_LDCS::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetStylePathList();
	m_CMFeditBriwse.EnableFileBrowseButton();
	m_NetworkStatus = "云查杀未开启,建议开启";
	UpdateData(FALSE);
	ConnentFlag = FALSE;
	// 设置杀毒详情风格
	int nWdith = rc.Width();
	dwOldStyle = m_ListMessageInfo.GetExtendedStyle();
	m_ListMessageInfo.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListMessageInfo.InsertColumn(0, L" ิ.ั 扫描文件详情, 请稍等", LVCFMT_CENTER, nWdith / 1);
	// 获取盘符信息
	Flag = 1;
	nDriverCount = this->GetDriveStringName();
	// 赋值this
	MFCsockclass sockthis;
	sockthis.Thisptr = this;

	return TRUE;
}

// 动态获取盘符信息
int Tab_LDCS::GetDriveStringName()
{
	m_PathListCtr.DeleteAllItems();
	WCHAR szName[MAX_PATH] = { 0 };
	GetLogicalDriveStrings(MAX_PATH, szName);
	int nCount = 0;
	for (int i = 0; szName[i] != '0' && i < MAX_PATH; ++i)
	{
		OutputDebugString(&szName[i]);
		OutputDebugString(L"\n");
		m_PathListCtr.InsertItem(nCount, &szName[i]);
		i += wcslen(&szName[i]);
		++nCount;
	}
	return nCount;
}

// 零度查杀（查杀按钮）
void Tab_LDCS::OnBnClickedOk()
{
	// 每次查杀把这ListCtr的控件插入初始化为0 insertItem(XX!, XX);
	CString str;
	UpdateData(TRUE);
	Tab_LDCS::m_MDEValues = this->m_MDEditValse;
	Tab_LDCS::nCounts = 0;
	Tab_LDCS::nCountList = 0;
	// 1. 初始化数据
	nK = nI = nCountFileNumber = 0;
	// 2. 文件遍历，对比MD5值，输出可疑文件信息(本地查杀)
	for (int i = 0; i < 5; i++)
	{
		str = m_PathListCtr.GetItemText(i, 0);
		if (str.GetLength() <= 0)
			break;
		strFindNamPathp[i] = m_PathListCtr.GetItemText(i, 0);
		++nCounts;
	}
	SetSaveSuspectedFileList();
	switch (Flag)
	{
	case 1:
	case 2:
	case 3:
		hThread = CreateThread(NULL, NULL, ThreadProcLDCS, (LPVOID)this, NULL, NULL);
	break;
	default:
		break;
	}

	// CDialogEx::OnOK();
}

// 测试
void Tab_LDCS::OnRadio2()
{

}
void Tab_LDCS::OnRadio3()
{

}

// 1. 全盘查杀
void Tab_LDCS::OnBnClickedRadio1()
{
	this->Flag = 1;
	m_PathListCtr.DeleteAllItems();
	m_ListMessageInfo.DeleteAllItems();
	SetStylePathList();
	nDriverCount = this->GetDriveStringName();
}

// 2. 快速查杀
void Tab_LDCS::OnBnClickedRadio2()
{
	// TODO:  在此添加控件通知处理程序代码
	this->Flag = 2;
	m_PathListCtr.DeleteAllItems();
	m_ListMessageInfo.DeleteAllItems();
	SetStylePathList();
	m_PathListCtr.InsertItem(0, L"C:\\项目演示\\");
}

// 3. 自定义查杀
void Tab_LDCS::OnBnClickedRadio3()
{
	// TODO:  在此添加控件通知处理程序代码
	this->Flag = 3;
	m_PathListCtr.DeleteAllItems();
	m_PathListCtr.InsertItem(0, L"请拖拽文件到列表框");
}

// 4. 响应拖拽消息（自定义）
void Tab_LDCS::OnDropFiles(HDROP hDropInfo)
{
	CString str;
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
		str = wcStr;
		// 5.更新到列表中
		if (3 == this->Flag)
		{
			m_PathListCtr.DeleteAllItems();
			m_PathListCtr.InsertItem(i, str);
		}
		else
			AfxMessageBox(L"非自定义杀毒不可响应拖拽");
	}
	// 6.释放内存
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}

// 5. 启动云查杀
void Tab_LDCS::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码

	// 1. 创建套接字
	if (!pSocket.Create())
	{
		AfxMessageBox(L"创建Socket失败");
	}
	// 2. 连接服务器
	if (!pSocket.Connect(L"127.0.0.1", 13245))
	{
		AfxMessageBox(L"连接服务器失败，重新新尝试");
		return;
	}
	ConnentFlag = TRUE;
	m_NetworkStatus = "云查杀已全面开启";
	UpdateData(FALSE);
}

// 6. 关闭云查杀
void Tab_LDCS::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	// 关闭套接字
	ConnentFlag = FALSE;
	pSocket.Close();
	m_NetworkStatus = "云查杀已关闭，建议开启云端查杀";
	UpdateData(FALSE);
}

// 7. 响应服务器发来的消息(全部消息类型)
void Tab_LDCS::OnReceives(WPARAM wParam, LPARAM lParam)
{
	// 7.1 强转结构体
	PSERVERSTRUCT InfoMeg = nullptr;
	InfoMeg = (PSERVERSTRUCT)lParam;
	switch (InfoMeg->eEnum)
	{
	// 7.1.1 上传病毒库响应
	case SockEnum::MD5MEEAGE:
	{
		if (InfoMeg->OpenCode)
			AfxMessageBox(L"病毒库（MD5）上传云端成功");
		else
			AfxMessageBox(L"病毒库（MD5）上传云端失败，请从新尝试");
	}
	break;
	// 7.2.2 查杀结果响应
	case SockEnum::MD5SELECT:
	{
		// 响应消息
		if (InfoMeg->OpenCode)
		{
			CString str = InfoMeg->u1.se.PathName;
			m_PathListCtr.InsertItem(nCountList, str);
			nCountList++;
		}
	}
	break;
	default:
		break;
	}
}

// 8. 上传云端 加载文件（MD5自定义文件特征）
void Tab_LDCS::OnBnClickedButton6()
{
	// 判断是否开启网络
	if (FALSE == ConnentFlag)
	{
		AfxMessageBox(L"请打开云查杀在点击重试");
		return;
	}
	// 数据包结构体
	SOCKETSEND SaveMd5values = { 0 };
	// 调用函数上传病毒库
	Md5PutValues();
	// 3. 循环向服务器发送数据包，更新病毒库（MD5）值
	for (int i = 0; i < nCOunt; ++i)
	{
		// 3.1 发送数据包到服务器
		SaveMd5values.hWnd = this->m_hWnd;
		SaveMd5values.eEnum = SockEnum::MD5MEEAGE;
		strcpy(SaveMd5values.u1.m_Md5.sMD5, SaveMd5[i]);
		SaveMd5values.u1.m_Md5.ulFileSize = strlen(SaveMd5[i]);
		// 3.2 有可能出现粘包问题(未解决)
		pSocket.Send((void *)&SaveMd5values, sizeof(_SOCKETSEND));
		Sleep(200);
	}
}

void Tab_LDCS::OnLvnItemchangedList5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

// 9. 右击弹出菜单
void Tab_LDCS::OnRclickList5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu PopMen;
	PopMen.LoadMenuW(IDR_MENU6);
	POINT point;
	GetCursorPos(&point);
	CMenu *p = PopMen.GetSubMenu(0);
	p->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

// 10. 属性
void Tab_LDCS::On40043()
{
	// TODO:  在此添加命令处理程序代码
	CString str;
	int index = m_PathListCtr.GetSelectionMark();
	str = m_PathListCtr.GetItemText(index,0);
	FileInfo fileName;
	fileName.cFilePath = str;
	fileName.DoModal();
}

// 11. 查找可疑文件(遍历查杀MD5，黑白名单)
void Tab_LDCS::FindFile(CString Path, CString MD5)
{
	CString strAt;
	char FilePath[MAX_PATH] = { 0 };
	WIN32_FIND_DATA FileData = { 0 };
	HANDLE hFile = FindFirstFile(Path + "\\*", &FileData);
	// 1. 判断
	if (hFile != INVALID_HANDLE_VALUE)
	{
		// 2. 递归循环
		do
		{
			CString FilePath = Path + FileData.cFileName;
			m_ListMessageInfo.InsertItem(nK, FilePath);
			GetDlgItem(IDC_VALUESTAT)->SetWindowTextW(FilePath);
			if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (CString(".") != FileData.cFileName && CString("..") != FileData.cFileName)
					FindFile(Path + CString("\\") + FileData.cFileName, MD5);
			}
			// 3. 不是文件就输出文件信息
			else
			{
				char str[MAX_PATH] = { 0 };
				WideCharToMultiByte(CP_ACP, NULL, FilePath, FilePath.GetLength(),
					str, MAX_PATH, NULL, NULL);
				CString mdValues = md5FileValue(str);
				// 4. 云联网查杀
				if (ConnentFlag)
					Md5Recver(FilePath, mdValues);
				else
				{
					// 4.2 比对MD5值
					if ((mdValues == MD5) && (mdValues.GetLength() > 0))
					{
						// 4.2.1 白名单 即使是MD5查杀文件 白名单仍可以免杀
						if (find(WhiteDialogBox::WhiteName.begin(), WhiteDialogBox::WhiteName.end(), FilePath) == WhiteDialogBox::WhiteName.end())
						{
							++nI;
							// 4.2.2 可疑文件个数查杀
							++nCountFileNumber;
							SaveSuspectedFile.push_back(Path + CString("\\") + FileData.cFileName);
							m_PathListCtr.InsertItem(nI, Path + CString("\\") + FileData.cFileName);
						}
					}
					// 4.3 黑明单 即使不是MD5查杀对象， 若发现该文件 立刻查杀
					if (find(BlacklistDialog::nBlackList.begin(), BlacklistDialog::nBlackList.end(), FilePath) != BlacklistDialog::nBlackList.end())
					{
						++nI;
						++nCountFileNumber;
						SaveSuspectedFile.push_back(Path + CString("\\") + FileData.cFileName);
						m_PathListCtr.InsertItem(nI, Path + CString("\\") + FileData.cFileName);
					}
					++nK;
				}
			}	
		} while (FindNextFile(hFile, &FileData));
	}
}

// 12. 停止查杀病毒
void Tab_LDCS::OnBnClickedButton3()
{
	// 关闭这个句柄线程
	TerminateThread(hThread, NULL);
	CloseHandle(hThread);
}

// 13. 恢复病毒查杀
void Tab_LDCS::OnBnClickedButton2()
{
	ResumeThread(hThread);
}

// 14. 暂停查杀病毒
void Tab_LDCS::OnBnClickedButton5()
{
	SuspendThread(hThread);
}

// 控件响应无用
void Tab_LDCS::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

// 15. 白名单
void Tab_LDCS::OnBnClickedButton7()
{
	WhiteDialogBox white;
	white.DoModal();
}

// 16. 黑名单
void Tab_LDCS::OnBnClickedButton8()
{
	BlacklistDialog black;
	black.DoModal();
}

// 17. 单选处理病毒
void Tab_LDCS::On40042()
{
	// TODO:  在此添加命令处理程序代码
	// 1. 获取文件路径
	int index = m_PathListCtr.GetSelectionMark();
	CString str = m_PathListCtr.GetItemText(index, 0);
	// 2. 打开进程并且处理修改PE头
	//DWORD dwCount = 1;
	char PEname[MAX_PATH] = { 0 };
	WideCharToMultiByte(CP_ACP, NULL, str, str.GetLength(),
		PEname, MAX_PATH, NULL, NULL);
	HANDLE hFile = CreateFileA(PEname, GENERIC_READ, FALSE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// 获取文件大小
	DWORD dwSize = GetFileSize(hFile, NULL);
	// 缓冲区
	char* lpBase;
	lpBase = new char[dwSize]{};
	// 读文件
	DWORD dwCount = 1;
	BOOL  bRet = ReadFile(hFile, lpBase, dwSize, &dwCount, NULL);
	if (bRet)
	{
		// 3. 隔离操作
		PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpBase;
		pDos->e_magic = 0x4D5A;
	}
	// 4. 写回到文件
	FlushFileBuffers(hFile);
	// 4. 移动文件到隔离文件夹
	CloseHandle(hFile);
	AfxMessageBox(L"隔离成功文件成功");
	MoveFile(str,  L"C:\\项目演示\\病毒隔离文件夹\\游戏.exe");
	delete[] lpBase;
}

void Tab_LDCS::OnStnClickedValuestat()
{
	// TODO:  在此添加控件通知处理程序代码
}

// 18. 云查杀MD上传病毒库
BOOL Tab_LDCS::Md5PutValues()
{
	// 临时接收MD5值
	// SaveMd5[MAX_PATH][36] = { 0 };
	// memcpy(SaveMd5, 0, sizeof(SaveMd5));
	// 1. 获取选中文件
	UpdateData(TRUE);
	// 2. 读文件病毒到堆或者容器
	char filePath[MAX_PATH] = { 0 };
	FILE *pFile = nullptr;
	WideCharToMultiByte(CP_ACP, NULL, m_Editvalues, m_Editvalues.GetLength(),
		filePath, MAX_PATH, NULL, NULL);
	// 2.1 打开文件
	pFile = fopen(filePath, "rb+");
	if (pFile)
	{
		// 2.2 读取文件内存保存在容器中
		while (!feof(pFile))
		{
			fread(SaveMd5[nCOunt], 32, 1, pFile);
			++nCOunt;
		}
		// SaveMd5Values.push_back();
	}
	return TRUE;
}

// 19. 云查杀响应服务器发来消息
BOOL Tab_LDCS::Md5Recver(CString PathName,CString md5valse)
{
	char MD5[36] = { 0 };
	// 1. 转换成char类型md5
	WideCharToMultiByte(CP_ACP, NULL, md5valse, md5valse.GetLength(), MD5, 36, NULL, NULL);
	// 2. 填充数据包
	_SOCKETSEND pSockMd5Msg = { 0 };
	pSockMd5Msg.eEnum = SockEnum::MD5SELECT;
	pSockMd5Msg.hWnd = this->m_hWnd;
	pSockMd5Msg.u1.m_Md5.ulFileSize = strlen(MD5);
	strcpy(pSockMd5Msg.u1.m_Md5.sMD5, MD5);
	WideCharToMultiByte(CP_ACP, NULL, PathName, PathName.GetLength(), pSockMd5Msg.PathName, sizeof(pSockMd5Msg.PathName), NULL, NULL);
	// 3. 发送服务器
	pSocket.Send((void *)&pSockMd5Msg, sizeof(_SOCKETSEND));
	Sleep(100);
	return TRUE;
}