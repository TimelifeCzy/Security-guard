// FileInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "FileInfo.h"
#include "afxdialogex.h"
#include "Psapi.h"
#include "MD5.h"

// FileInfo 对话框

CImageList* FileInfo::Icon = NULL;

IMPLEMENT_DYNAMIC(FileInfo, CDialogEx)

FileInfo::FileInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(FileInfo::IDD, pParent)
	, m_seFileType(_T(""))
	, m_seFileWZ(_T(""))
	, m_seFileSizeof(_T(""))
	, m_seFileCreate(_T(""))
	, m_seFileModify(_T(""))
	, m_seFileAccess(_T(""))
	, m_seEditFileName(_T(""))
	, m_seEditMD5(_T(""))
{

}

FileInfo::~FileInfo()
{
}

void FileInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEFILETYPE, m_seFileType);
	DDX_Text(pDX, IDC_SEFILEWZ, m_seFileWZ);
	DDX_Text(pDX, IDC_SEFILESIZEOF, m_seFileSizeof);
	DDX_Text(pDX, IDC_SEFILECREATE, m_seFileCreate);
	DDX_Text(pDX, IDC_SEFILEMODIFY, m_seFileModify);
	DDX_Text(pDX, IDC_SEACCESSTIME, m_seFileAccess);
	DDX_Text(pDX, IDC_SEEDITNAME, m_seEditFileName);
	DDX_Text(pDX, IDC_EDIT2, m_seEditMD5);
}


BEGIN_MESSAGE_MAP(FileInfo, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// FileInfo 消息处理程序


BOOL FileInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 获取文件信息
	GetFileInfo(cFilePath);
	//m_bush.CreateSolidBrush(RGB(255, 255, 255));
	return TRUE;
}

// 获取文件详细信息
void FileInfo::GetFileInfo(CString str)
{
	MD5VAL md5;
	// 获取文件路径
	TCHAR Path[MAX_PATH] = {};
	SYSTEMTIME System = { 0 };
	// 用于保存临时字符串的缓冲区
	TCHAR TempBuffer[MAX_PATH] = { 0 };
	// VS_FIXEDFILEINFO 
	WIN32_FIND_DATA stFileData = { 0 };
	HANDLE hFile = FindFirstFile(str, &stFileData);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	// 1. 添加文件名
	m_seEditFileName = stFileData.cFileName;
	// 2. 添加创建时间
	FileTimeToSystemTime(&stFileData.ftCreationTime, &System);
	_stprintf(TempBuffer, TEXT("%d/%d/%d %d:%d:%d"), System.wYear,
		System.wMonth, System.wDay, System.wHour, System.wMinute, System.wSecond);
	m_seFileCreate = TempBuffer;
	// 3. 添加文件修改时间
	FileTimeToSystemTime(&stFileData.ftLastWriteTime, &System);
	_stprintf(TempBuffer, TEXT("%d/%d/%d %d:%d:%d"), System.wYear,
		System.wMonth, System.wDay, System.wHour, System.wMinute, System.wSecond);
	m_seFileModify = TempBuffer;
	// 4. 添加最后访问
	FileTimeToSystemTime(&stFileData.ftLastAccessTime, &System);
	_stprintf(TempBuffer, TEXT("%d/%d/%d %d:%d:%d"), System.wYear,
		System.wMonth, System.wDay, System.wHour, System.wMinute, System.wSecond);
	m_seFileAccess = TempBuffer;
	TempBuffer[0] = 0;
	// 5. 判断是不是目录  
	if (stFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		_tcscat(TempBuffer, TEXT("目录 "));
	else
		_tcscat(TempBuffer, TEXT("文件 "));
	m_seFileType = TempBuffer;
	// 6. 显示当前文件的大小
	TempBuffer[0] = 0;
	if (stFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		_tcscat(TempBuffer, TEXT("-"));
	else
	{
		if (stFileData.nFileSizeLow > 1073741824)
			_stprintf(TempBuffer, TEXT("%.2lfGB"), stFileData.nFileSizeLow / 1024.0 / 1024.0 / 1024.0);
		else if (stFileData.nFileSizeLow > 1048576)
			_stprintf(TempBuffer, TEXT("%.2lfMB"), stFileData.nFileSizeLow / 1024.0 / 1024.0);
		else
			_stprintf(TempBuffer, TEXT("%.2lfKB"), stFileData.nFileSizeLow / 1024.0 / 1024.0);
	}
	m_seFileSizeof = TempBuffer;
	// 7. 属性
	// 判断是不是隐藏的
	if (stFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		_tcscat(TempBuffer, TEXT("隐藏 "));
	// 判断是不是只读的
	if (stFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		_tcscat(TempBuffer, TEXT("只读"));
	m_seFileWZ = TempBuffer;
	GetModuleFileNameEx(hFile, NULL, Path, MAX_PATH);
	// MD5计算
	char FileName[MAX_PATH] = { 0 };
	char *p = FileName;
	memset(FileName, 0, sizeof(FileName));
	// strcpy(FileName, str.GetBuffer(str.GetLength()));
	// sprintf 不可行
	// p = (LPSTR)(LPCSTR)str;
	// 转成宽字符
	sprintf(FileName, "%ws", str);
	m_seEditMD5 = md5FileValue(FileName);
	// 获取信息
	SHFILEINFOW shfileinfo;
	SHGetFileInfo(Path, 0, &shfileinfo, sizeof(SHFILEINFOW), SHGFI_ICON);
	//	Icon->Add(shfileinfo.hIcon);
	//	m_ICon.SetIcon((HBITMAP)Icon->);
	// 更新变量
	UpdateData(FALSE);
}

HBRUSH FileInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return  m_bush;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
