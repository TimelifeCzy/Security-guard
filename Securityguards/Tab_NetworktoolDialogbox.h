#pragma once
#include "stdafx.h"
#include "resource.h"

// Tab_NetworktoolDialogbox 对话框

class Tab_NetworktoolDialogbox : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_NetworktoolDialogbox)

public:
	Tab_NetworktoolDialogbox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_NetworktoolDialogbox();

// 对话框数据
	enum { IDD = IDD_Network_tool };



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	DWORD m_SPid = 0, m_TPid =0;
public:
	afx_msg void OnBnClickedButton4();
};
