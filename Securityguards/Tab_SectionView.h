#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"

// Tab_SectionView 对话框

class Tab_SectionView : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_SectionView)

public:
	Tab_SectionView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_SectionView();

// 对话框数据
	enum { IDD = IDD_SECTIONVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtr;
	virtual BOOL OnInitDialog();
	static const TCHAR strName[6][20];
	void ShowDateInfo();
	// 传递加载接口
	void SetlpBase(char* lpbase){ this->m_lpBase = lpbase; }
private:
	char* m_lpBase;										// 保存加载地址
	TCHAR buf[MAX_PATH] = {};							// 保存临时数据
};
