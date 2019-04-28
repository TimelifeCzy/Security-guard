#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxwin.h"

// Tab_PEfileanalDialogbox 对话框

class Tab_PEfileanalDialogbox : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_PEfileanalDialogbox)

public:
	Tab_PEfileanalDialogbox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_PEfileanalDialogbox();

// 对话框数据
	enum { IDD = IDD_PE_Fille_analysis };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_entryPoint;
	CString m_baseAddres;
	CString m_codeStart;
	CString m_fileSize;
	CStatic m_dataStart;
	CStatic m_block;
	CStatic m_childSystem;
	CStatic m_flag;
	CString m_fileAli;
	CStatic m_rvaName;
	CString m_messageFlag;
	CString m_fileTime;
	DWORD	e_addrssbase;
// 公用接口
public:
	// 判定是否PE文件
	BOOL IsPEfile(char* lpBase);
	// 显示PE头部信息
	void ShowHeaderInfo(char* lpBase);
	// 文件加载接口
	void FileLoadMemorys(char* lpFilePtah){ this->FileLoadMemory(lpFilePtah); }

// 私有方法
private:
	// 加载文件到内存
	char* FileLoadMemory(char* lpFilePath);
	// 临时保存数据区
	char* m_lpBase = 0;

public:
	CString m_textPath;
	// 响应拖拽信息
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_blocks;
	CString m_childsystems;
	CString m_dataStarts;
	CString m_rvaNames;
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton18();
};
