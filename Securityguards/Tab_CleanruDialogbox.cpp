// Tab_CleanruDialogbox.cpp : 实现文件
//

#include "stdafx.h"
#include "Tab_CleanruDialogbox.h"
#include "afxdialogex.h"
#include <algorithm>

// Tab_CleanruDialogbox 对话框

// VS清理后缀
CString Tab_CleanruDialogbox::ExtendString = { _T(".obj\\.tlog\\.lastbuildstate\\.idb\\.pdb\\.pch\\.res\\.ilk\\.exe\\.sdf\\.ipch\\.log\\.db\\.suo\\") };

// 自定义消息后缀
CString Tab_CleanruDialogbox::UserExtendString = {};

// 暂定 全局唯一标识
int Tab_CleanruDialogbox::FileNumbersFlag = 0;

IMPLEMENT_DYNAMIC(Tab_CleanruDialogbox, CDialogEx)

Tab_CleanruDialogbox::Tab_CleanruDialogbox(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_CleanruDialogbox::IDD, pParent)
	, m_EditText(_T(""))
	, m_cleaFilenumber(_T(""))
	, m_cleaFileSizeof(_T(""))
	, m_cleaDeleFileCounts(_T(""))
	, m_SuffixFileNameValues(_T(""))
{

}

Tab_CleanruDialogbox::~Tab_CleanruDialogbox()
{
}

void Tab_CleanruDialogbox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_TypeSelect);
	DDX_Text(pDX, IDC_EDIT1, m_EditText);
	DDX_Control(pDX, IDC_EDIT1, m_EditTexts);
	DDX_Control(pDX, IDC_BUTTON1, m_GetEditText);
	DDX_Text(pDX, IDC_CLEAFILENUMBER, m_cleaFilenumber);
	DDX_Text(pDX, IDC_CLEAFILESIZEOF, m_cleaFileSizeof);
	DDX_Control(pDX, IDC_CLEADELETEFILECOUNT, m_cleaDeleFileCount);
	DDX_Text(pDX, IDC_CLEADELETEFILECOUNT, m_cleaDeleFileCounts);
	DDX_Control(pDX, IDC_LIST2, m_cleaDeleteInfoList);
	DDX_Control(pDX, IDC_LIST1, m_cleaFileInfoList);
	DDX_Control(pDX, IDC_EDIT2, m_SuffixFileName);
	DDX_Control(pDX, IDC_BUTTON2, m_Suffix);
	DDX_Text(pDX, IDC_EDIT2, m_SuffixFileNameValues);
}

BEGIN_MESSAGE_MAP(Tab_CleanruDialogbox, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Tab_CleanruDialogbox::OnCbnSelchangeCombo1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &Tab_CleanruDialogbox::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Tab_CleanruDialogbox::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &Tab_CleanruDialogbox::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &Tab_CleanruDialogbox::OnBnClickedButton2)
END_MESSAGE_MAP()

// Tab_CleanruDialogbox 消息处理程序

BOOL Tab_CleanruDialogbox::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_TypeSelect.AddString(L"VS清理");
	m_TypeSelect.AddString(L"自定义清理");
	m_TypeSelect.AddString(L"浏览器清理");
	m_TypeSelect.AddString(L"系统垃圾清理");
	m_TypeSelect.SetCurSel(2);
	this->Flag = 3;
	CRect rc;
	DWORD dwOldStyle = 0;
	m_cleaFileInfoList.GetClientRect(rc);
	int nWidth = rc.Width();
	dwOldStyle = m_cleaFileInfoList.GetExtendedStyle();
	m_cleaFileInfoList.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cleaDeleteInfoList.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cleaFileInfoList.InsertColumn(0, L"File  Info", LVCFMT_CENTER, nWidth);
	m_cleaDeleteInfoList.InsertColumn(0, L"Delete  FileInfo", LVCFMT_CENTER, nWidth);
	// 初始化两个累计大小
	CountSize.QuadPart = 0;
	DestSize.QuadPart = 0;
	return TRUE;
}

// 响应下拉选择
void Tab_CleanruDialogbox::OnCbnSelchangeCombo1()
{
	m_cleaDeleteInfoList.DeleteAllItems();
	m_cleaFileInfoList.DeleteAllItems();
	/*只对控件控制 及 标志位修改*/
	CString str;
	int nIndex = m_TypeSelect.GetCurSel();
	m_TypeSelect.GetLBText(nIndex, str);
	if ("VS清理" == str)
	{	
		this->Flag = 1;
		m_SuffixFileName.ShowWindow(SW_SHOW);
		m_Suffix.ShowWindow(SW_SHOW);
		m_EditTexts.ShowWindow(SW_HIDE);
		m_SuffixFileName.SetWindowTextW(ExtendString);
	}
	else if ("自定义清理" == str)
	{
		this->Flag = 2;
		m_SuffixFileName.ShowWindow(SW_SHOW);
		m_Suffix.ShowWindow(SW_SHOW);
		m_EditTexts.ShowWindow(SW_SHOW);
		m_GetEditText.ShowWindow(SW_SHOW);
		m_SuffixFileName.SetWindowTextW(UserExtendString);
	}
	else if ("浏览器清理" == str)
	{
		this->Flag = 3;
		m_SuffixFileName.ShowWindow(SW_SHOW);
		m_Suffix.ShowWindow(SW_HIDE);
		m_EditTexts.ShowWindow(SW_HIDE);
	}
	else if ("系统垃圾清理" == str)
	{
		this->Flag = 4;
		m_SuffixFileName.ShowWindow(SW_SHOW);
		m_Suffix.ShowWindow(SW_HIDE);
		m_EditTexts.ShowWindow(SW_HIDE);
	}
	else
		AfxMessageBox(L"么有获取到正确信息！╱╲！");

}

// 响应文件拖拽
void Tab_CleanruDialogbox::OnDropFiles(HDROP hDropInfo)
{
	PathString.clear();
	CString str;
	// 1. 获取文件拖拽数目 如果第二个参数为-1则返回一个计数文件删除
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);
	// 2. 循环获取的每一个文件
	for (int i = 0; i < DropCount; ++i)
	{
		WCHAR wcStr[MAX_PATH] = { 0 };
		// 3. 获取拖拽第i个文件名
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);
		if (find(PathString.begin(), PathString.end(), wcStr) == PathString.end())
		{
			if (this->Flag <= 2)
			{
				// 5. 添加到保存到路径的容器和列表中
				PathString.push_back(wcStr);
				str = "文件路径：";
				str += wcStr;
				m_cleaFileInfoList.InsertItem(i, str);
			}
			else
				AfxMessageBox(L"系统清理或浏览器清理不可更改清理路径");
		}
	}
	// 6. 释放内存
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}

// 获取待删除文件的列表
void Tab_CleanruDialogbox::GetDeleteFileList(vector<CString> PathStrings)
{
	DeletString.clear();
	FileCount = DestCount = 0;
	DestSize.QuadPart = DestSize.HighPart = 0;
	// 1. 获取删除路径下所有文件
	for (auto Path : PathStrings)
		FindFile(Path);
	// 2. 遍历获取文件大小
	CString DeleteSizeofs;
	// 3. 文件个数   可删除文件个数
	m_cleaFilenumber.Format(L"%-3d 个", FileCount);
	m_cleaFileSizeof.Format(L"%.2lf MB", (CountSize.QuadPart / 1024.0 / 1024.0));
	m_cleaDeleFileCounts.Format(L"%-3d 个", DestCount);
	// 4. 弹出可删除文件的大小
	DeleteSizeofs.Format(L"可删除文件大小： %.2lf MB", DestSize.QuadPart / 1024.0 / 1024.0);
	UpdateData(FALSE);
	AfxMessageBox(DeleteSizeofs);
}

// 查找路径及删除后缀名
void Tab_CleanruDialogbox::FindFile(CString Path)
{
	int i = 0;
	WIN32_FIND_DATA FileData = { 0 };
	HANDLE hFile = FindFirstFile(Path + "\\*", &FileData);
	// 1. 判断
	if (hFile != INVALID_HANDLE_VALUE)
	{
		// 2. 递归循环
		do
		{
			if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (CString(".") != FileData.cFileName && CString("..") != FileData.cFileName)
					FindFile(Path + CString("\\") + FileData.cFileName);
			}
			// 3. 不是文件就输出文件信息
			else
			{
				// 文件个数+1  及 文件大小
				++FileCount;
				CountSize.QuadPart += FileData.nFileSizeLow;
				// 4. 判断后缀名
				if ((2 >= this->Flag) && (UserExtendString.GetLength() > 0) )
				{
					// 4.1 VS后缀清理 及 自定义清理
					if (-1 != ExtendString.Find(PathFindExtension(FileData.cFileName)) && (1 == this->Flag) )
					{
						++DestCount;
						DestSize.QuadPart += FileData.nFileSizeLow;
						DeletString.push_back(Path + CString("\\") + FileData.cFileName);
						m_cleaDeleteInfoList.InsertItem(i, Path + CString("\\") + FileData.cFileName);
					}
					// 4.2 VS后缀清理 及 自定义清理
					if (-1 != UserExtendString.Find(PathFindExtension(FileData.cFileName)) && (2 == this->Flag) )
					{
						++DestCount;
						DestSize.QuadPart += FileData.nFileSizeLow;
						DeletString.push_back(Path + CString("\\") + FileData.cFileName);
						m_cleaDeleteInfoList.InsertItem(i, Path + CString("\\") + FileData.cFileName);
					}
				}
				// 5. 清理是固定目录如系统清理 浏览器清理 不区分后缀
				else
				{
					++DestCount;
					DestSize.QuadPart += FileData.nFileSizeLow;
					DeletString.push_back(Path + CString("\\") + FileData.cFileName);
					m_cleaDeleteInfoList.InsertItem(i, Path + CString("\\") + FileData.cFileName);
				}
			}
			++i;
		} while (FindNextFile(hFile, &FileData));
	}
}

// 删除文件按钮
void Tab_CleanruDialogbox::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str = "确认清理路径为 ‘ ";
	for (SIZE_T i = 0; i < PathString.size(); ++i)
	{
		str += ' ';
		str += PathString[i].GetString();
	}
	str += " ’ 下的文件????";
	if (IDOK == ::MessageBox(NULL, str, L"删除确认", MB_ICONQUESTION | MB_OKCANCEL))
	{
		// 开始删除文件
		DeleteCount = 0;
		for (auto File : DeletString)
		{
			if (DeleteFile(File))
				++DeleteCount;
		}
		CString Msg;
		Msg.Format(L"需删除文件 %d 个\n已删除文件 %d 个",
			DestCount, DeleteCount);
		MessageBox(Msg);
	}
	return;
	// CDialogEx::OnOK();
}

// 获取当前类型及准备删除环境
void Tab_CleanruDialogbox::OnBnClickedButton1()
{
	// 清除不掉信息？
	m_cleaDeleteInfoList.DeleteAllItems();
	// 1. 获取当前选中的文本信息
	CString str;
	int nIndex = m_TypeSelect.GetCurSel();
	m_TypeSelect.GetLBText(nIndex, str);
	// 2. 响应相对的事件
	if ("VS清理" == str)
	{
		if (0 == PathString.size())
		{
			AfxMessageBox(L"请先拖拽准备的删除文件夹, 再次获取");
			return;
		}
		// 2.1 传递拖拽文件路径
		GetDeleteFileList(PathString);
	}
	else if ("自定义清理" == str)
	{
		UpdateData(TRUE);
		if (m_EditText.GetLength() > 0)
			PathString.push_back(str);
		else
		{
			AfxMessageBox(L"输入\\拖拽 删除文件或目录, 再次获取");
			return;
		}
		GetDeleteFileList(PathString);
	}
	else if ("浏览器清理" == str)
	{
		// 固定路径
		this->strPath = "c:\\users\\administrator\\appdata\\roaming\\360se6\\User Data\\Default";
		PathString.push_back(strPath);
		GetDeleteFileList(PathString);
	}
	else if ("系统垃圾清理" == str)
	{
		// 固定路径
		this->strPath = "C:\\Windows\\Temp";
		PathString.push_back(strPath);
		GetDeleteFileList(PathString);
	}
	else
		AfxMessageBox(L"么有获取到正确信息！╱╲！");
}

// 更新VS及自定义后缀名称
void Tab_CleanruDialogbox::OnBnClickedButton3()
{
	UpdateData(TRUE);
	// 完成VS后缀更新
	if (1 == Flag)
	{
		ExtendString = m_SuffixFileNameValues;
		return;
	}
	if (2 == Flag)
	{
		UserExtendString = m_SuffixFileNameValues;
		return;
	}
}

void Tab_CleanruDialogbox::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
}
