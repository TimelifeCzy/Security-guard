#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"

// Tab_resourceTables 对话框

class Tab_resourceTables : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_resourceTables)

public:
	Tab_resourceTables(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_resourceTables();

// 对话框数据
	enum { IDD = IDD_RESOURCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	// 保存lpBase;
	char* m_lpBase = 0;
	virtual BOOL OnInitDialog();
	// 获取显示资源表
	void ShowResourInfo();
	CTreeCtrl m_TreeFile;
};
