#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"

// Tab_Dlltables 对话框

class Tab_Dlltables : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_Dlltables)

public:
	Tab_Dlltables(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_Dlltables();

// 对话框数据
	enum { IDD = IDD_DLLTABLES };
public:
	// 保存文件加载(VA)
	char* m_lpBase = 0;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtr;
	virtual BOOL OnInitDialog();

public:
	// 获取显示窗口
	void GetdllLoadofShow();
};
