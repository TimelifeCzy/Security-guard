#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxwin.h"

// FileInfo 对话框

class FileInfo : public CDialogEx
{
	DECLARE_DYNAMIC(FileInfo)

public:
	FileInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FileInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_seFileType;
	CString m_seFileWZ;
	CString m_seFileSizeof;
	CString m_seFileCreate;
	CString m_seFileModify;
	CString m_seFileAccess;
	CString m_seEditFileName;
	CString m_seEditMD5;
	virtual BOOL OnInitDialog();
	// 保存文件路径(名字)
	CString cFilePath;
	static CImageList* Icon;
	CBrush m_bush;
private:
	// 获取文件路径
	void GetFileInfo(CString str);
public:
	CStatic m_ICon;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
