#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"

// Tab_RELOCATION 对话框

class Tab_RELOCATION : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_RELOCATION)

public:
	Tab_RELOCATION(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_RELOCATION();
	char* m_lpBaseReLoc;

// 对话框数据
	enum { IDD = IDD_RELOCATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedRelist1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_ReListCtr;
	CListCtrl m_ReListCtrs;
	// 获取区段名称及区段的基址
	DWORD GetSection(PIMAGE_NT_HEADERS const pNt, DWORD const dwRVA);
	// 保存区段名称
	CString strName;
private:
	// 区段属性解析 analysis
	void SectionAnal();
	// 块项目属性解析
	void BloackAnal();
};
