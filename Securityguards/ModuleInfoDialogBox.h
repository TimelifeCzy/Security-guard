#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"

// ModuleInfoDialogBox 对话框

class ModuleInfoDialogBox : public CDialogEx
{
	DECLARE_DYNAMIC(ModuleInfoDialogBox)

public:
	ModuleInfoDialogBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ModuleInfoDialogBox();
	// 保存父类ID
	DWORD m_ownerPid = 0;
	// 获取进程模块
	void GetProcessModule(const DWORD idProcess);

// 对话框数据
	enum { IDD = IDD_MODULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtr;
	virtual BOOL OnInitDialog();
};
