#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include "vector"
// BlacklistDialog 对话框

using std::vector;

class BlacklistDialog : public CDialogEx
{
	DECLARE_DYNAMIC(BlacklistDialog)

public:
	BlacklistDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BlacklistDialog();

// 对话框数据
	enum { IDD = IDD_BLACKLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 保存黑名单路径
	static vector<CString> nBlackList;
	// 封装UpdateList
	void UpdateList();
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CListCtrl m_blacklsit;
	CString m_BlackList;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
