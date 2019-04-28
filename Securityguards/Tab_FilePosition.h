#pragma once
#include "stdafx.h"
#include "resource.h"
// Tab_FilePosition 对话框

class Tab_FilePosition : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_FilePosition)

public:
	Tab_FilePosition(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_FilePosition();

// 对话框数据
	enum { IDD = IDD_POSIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_ImageBase;
	DWORD m_vritualAddress;
	DWORD m_offsetS;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_editupdate;
};
