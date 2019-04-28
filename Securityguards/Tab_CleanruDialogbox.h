#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
// Tab_CleanruDialogbox 对话框

using namespace std;

class Tab_CleanruDialogbox : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_CleanruDialogbox)

public:
	Tab_CleanruDialogbox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_CleanruDialogbox();

// 对话框数据
	enum { IDD = IDD_Clean_rubbish };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_TypeSelect;
	afx_msg void OnCbnSelchangeCombo1();
	CString m_EditText;
	CEdit m_EditTexts;
	CButton m_GetEditText;
	afx_msg void OnDropFiles(HDROP hDropInfo);
private:
	static int FileNumbersFlag;		// 记录当前文件路径一共占用几行
	int Flag = 0;					// 区别清理后缀	
	vector<CString> PathString;		// 保存路径的容器
	static CString ExtendString;	// VS清理后缀路径
	static CString UserExtendString;// 自定义清理后缀路径
	vector<CString> DeletString;	// 要删除的文件
	INT FileCount, DestCount;		// 总文件个数
	LARGE_INTEGER CountSize;		// 文件总大小
	INT DeleteCount;				// 已删除的文件个数
	LARGE_INTEGER DestSize;			// 可删除文件的大小
	TCHAR buf[MAX_PATH] = { };		// 保存自定义文件信息的名字
	CString strPath;				// 保存浏览器及系统垃圾清理路径
	void GetDeleteFileList(vector<CString> PathStrings);	// 获取待删除列表
	void FindFile(CString Path);	// 查找遍历文件
public:
	CString m_cleaFilenumber;
	CString m_cleaFileSizeof;
	CStatic m_cleaDeleFileCount;
	CString m_cleaDeleFileCounts;
	afx_msg void OnBnClickedOk();
	CListCtrl m_cleaDeleteInfoList;
	CListCtrl m_cleaFileInfoList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CEdit m_SuffixFileName;
	afx_msg void OnBnClickedButton2();
	CButton m_Suffix;
	CString m_SuffixFileNameValues;
};
