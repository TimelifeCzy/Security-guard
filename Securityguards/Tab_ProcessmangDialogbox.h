#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "TlHelp32.h"
#include "Psapi.h"
// Tab_ProcessmangDialogbox 对话框

// ListNameArrNumber
const int g_MaxName = 6;

class Tab_ProcessmangDialogbox : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_ProcessmangDialogbox)

public:
	Tab_ProcessmangDialogbox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_ProcessmangDialogbox();

// 对话框数据
	enum { IDD = IDD_Process_management };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

// 接口
public:
	void GetProcessInfos(){ this->GetProcessInfo(); }

// 私有方法
private:
	// 1. 获取进程信息
	BOOL GetProcessInfo();
	// 2. 设置进程信息
	void SetProcessInfo(const LPPROCESSENTRY32W P_32, const int count, const TCHAR* buf);
	// 3. ListNameArr
	static const TCHAR m_ListNameArr[g_MaxName][g_MaxName];
	TCHAR buf[MAX_PATH] = { };

// App方法
private:
	// 获取当前窗口进程数据
	void GetNewWindowProcessData();
	// 获取窗口信息的回调函数
	static BOOL CALLBACK EnumWinProc(HWND hwnd, LPARAM lParam);
	// 封装
	void ShowWindowApp();
	static int count;
	static TCHAR buff[MAX_PATH];
	static TCHAR buff1[MAX_PATH];
	static CImageList* Icon;


public:
	/*设置菜单属性*/
	CMenu men;
	POINT point;
	CMenu *p;
	/*-----------*/
	CListCtrl m_ListCtr;
	afx_msg void OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void CloseProcess();
	afx_msg void Filelocation();
	afx_msg void GetThreadInfo();
	afx_msg void GetHeapInfoss();
	CListCtrl m_ListCtrApp;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void GetModule();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void AppList();
	afx_msg void OnAttres();
};
