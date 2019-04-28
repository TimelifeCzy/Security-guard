#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "MFCsockclass.h"
#include "afxeditbrowsectrl.h"
#include "vector"

using std::vector;

// Tab_LDCS 对话框

class Tab_LDCS : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_LDCS)

public:
	Tab_LDCS(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_LDCS();

// 对话框数据
	enum { IDD = IDD_TABLDCS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_PathListCtr;
	CListCtrl m_ListMessageInfo;
	afx_msg void OnBnClickedOk();
	int m_buttQP;
	CButton m_ButtCX;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
public:
	// 响应服务器发来的消息
	void OnReceives(WPARAM wParam, LPARAM lParam);
	// 遍历文件查杀
	void FindFile(CString Path, CString MD5);
	static CString strFindNamPathp[5];
	// 标记什么样查杀 防止恶意文件拖拽
	static int Flag;
	static int nCounts;
	int GetDriveStringName();
	// 临时接收MD5值
	char SaveMd5[MAX_PATH][36] = {};
	// 上传病毒库
	BOOL Md5PutValues();
	char IsEm = 0;				// 变量（遍历）
	int nCOunt = 0;
	// 云查杀返回对比
	BOOL Md5Recver(CString PathName, CString md5valse);
	static int nCountList;		// 控件插入全局变量


private:
	// 保存MD5样本容器
	vector<CString> SaveMd5Values;
	// 风格属性
	CRect rc;
	DWORD dwOldStyle;
	// 记录可疑文件个数(及循环插入List等数据行号)
	int nI = 0, nK = 0, nCountFileNumber = 0;
	// 获取盘符个数
	int nDriverCount = 0;
	// 保存可疑文件
	vector<CString> SaveSuspectedFile;
	// Socket对象
	MFCsockclass pSocket;
	// 设置风格查杀路径
	void SetStylePathList();
	// 设置可以文件风格
	void SetSaveSuspectedFileList();
	// 本地查杀区分标志
	BOOL ConnentFlag = FALSE;
	// 回调函数句柄
	HANDLE hThread;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	CString m_NetworkStatus;
	CMFCEditBrowseCtrl m_CMFeditBriwse;
	afx_msg void OnBnClickedButton6();
	CString m_Editvalues;
	afx_msg void OnLvnItemchangedList5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickList5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On40043();
	CString m_MDEditValse;
	static CString m_MDEValues;
	// CProgressCtrl m_ProgressBar;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnEnChangeMfceditbrowse1();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void On40042();
	afx_msg void OnStnClickedValuestat();
};
