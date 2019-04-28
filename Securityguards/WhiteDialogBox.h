#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include <vector>

using std::vector;

// WhiteDialogBox 对话框

class WhiteDialogBox : public CDialogEx
{
	DECLARE_DYNAMIC(WhiteDialogBox)

public:
	WhiteDialogBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WhiteDialogBox();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 白名单容器
	static vector<CString> WhiteName;
	// 初始化封装
	void UpdateListName();
	CString m_WhiteEdit;
	CListCtrl m_ListArr;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
