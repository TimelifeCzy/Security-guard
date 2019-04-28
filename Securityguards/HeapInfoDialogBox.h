#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"

// HeapInfoDialogBox 对话框

class HeapInfoDialogBox : public CDialogEx
{
	DECLARE_DYNAMIC(HeapInfoDialogBox)

public:
	HeapInfoDialogBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~HeapInfoDialogBox();

// 对话框数据
	enum { IDD = IDD_HeapInfo };
	// 接收父类数据ID
	DWORD m_ownerPid = 0;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtr;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);

public:
	// 获取堆信息数据
	BOOL GetHeapInfo(DWORD pid);
	// 保存临时字符串
	TCHAR buf[MAX_PATH] = {};

	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
