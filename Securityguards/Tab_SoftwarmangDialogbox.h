#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include <vector>

using std::vector;

// Tab_SoftwarmangDialogbox 对话框

class Tab_SoftwarmangDialogbox : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_SoftwarmangDialogbox)

public:
	Tab_SoftwarmangDialogbox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_SoftwarmangDialogbox();

	// 对话框数据
	enum { IDD = IDD_Softwar_management };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ServicesoList;
	CListCtrl m_ServerInfo;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);

	// 公有接口
public:
	void pGetofShowAllSofwInfo() { this->GetofShowAllSofwInfo(); };
	//void pGetofShowZJSofwInfo() { this->GetofShowZJSofwInfo(); };
	//void pGetofShowBCYSofwInfo() { this->GetofShowBCYSofwInfo(); };
	void pGetofShowSystemService() { this->GetofShowSystemService(); };
	// 1. 自定义结构体
	typedef struct _SOFTINFO
	{
		WCHAR szSoftName[50];				// 软件名称 
		WCHAR szSoftVer[50];				// 软件版本号
		WCHAR szSoftDate[20];				// 软件安装日期
		WCHAR szSoftSize[MAX_PATH];			// 软件大小
		WCHAR strSoftInsPath[MAX_PATH];		// 软件安装路径
		WCHAR strSoftUniPath[MAX_PATH];		// 软件卸载路径
		WCHAR strSoftVenRel[50];			// 软件发布厂商
		WCHAR strSoftIco[MAX_PATH];			// 软件图标路径
	}SOFTINFO, *PSOFTINFO;

	// 私有方法
private:
	// 1. 获取显示全部软件 
	BOOL GetofShowAllSofwInfo();
	//// 2. 获取显示最近安装软件
	//BOOL GetofShowZJSofwInfo();
	//// 3. 获取显示不常用软件
	//BOOL GetofShowBCYSofwInfo();
	// 4. 获取显示系统服务
	BOOL GetofShowSystemService();
	// 保存软件相关信息
	vector<SOFTINFO> m_vecSofInfo;
	// 主键
	HKEY RootKey = HKEY_LOCAL_MACHINE;
	// 子键
	LPCTSTR lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	// 接收即将打开的键的句柄
	HKEY hkResult = 0;
	// List风格封装（系统）
	void ListStyleFun();
	// List风格封装（软件）
	void ListStyleFuns();
public:
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void StartService();
	afx_msg void StopService();
	afx_msg void SofwUninstall();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};