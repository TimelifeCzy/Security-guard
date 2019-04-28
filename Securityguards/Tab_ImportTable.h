#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"

// Tab_ImportTable 对话框

class Tab_ImportTable : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_ImportTable)

public:
	Tab_ImportTable(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_ImportTable();

// 对话框数据
	enum { IDD = IDD_IMPORTTABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ImportListCtr;
	CListCtrl m_ImportListCtr1;
	virtual BOOL OnInitDialog();
	char* m_ImlpBase;
private:
	// 显示导入表
	void ShowImportTable();
	// 切换API函数
	void APIshow();
	// 导入头
	PIMAGE_IMPORT_DESCRIPTOR pImport = { 0 };
public:
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickList3(NMHDR *pNMHDR, LRESULT *pResult);
};
