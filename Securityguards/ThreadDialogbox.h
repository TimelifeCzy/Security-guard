#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include "TlHelp32.h"
// ThreadDialogbox 对话框

class ThreadDialogbox : public CDialogEx
{
	DECLARE_DYNAMIC(ThreadDialogbox)

public:
	ThreadDialogbox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ThreadDialogbox();

// 对话框数据
	enum { IDD = IDD_Thread_Info };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Listctr;
	virtual BOOL OnInitDialog();

// 公有接口
public:
	void GetThreadInfos(){ this->GetThreadInfo(); }
	// 保存父进程的ID
	DWORD m_ownperProcess = NULL;

// 私有方法
private:
	// 获取当前选中的List选中的Pid
	unsigned long GetThreadPid();
	// 获取按钮状态
	int value = 0;
	// 获取线程信息
	BOOL GetThreadInfo();
	// 显示获取信息
	void ShowThreadList(const THREADENTRY32 t_32, const int nSel);
	// 保存临时转换字符串
	TCHAR buf[MAX_PATH] = {};
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void HungThread();
	afx_msg void RestoretheThread();
	afx_msg void CloseThread();
//	afx_msg void GetHeapInfoss();
};
