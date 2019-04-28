#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include "list"

using std::list;

// Tab_MemoryDialogbox 对话框

class Tab_MemoryDialogbox : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_MemoryDialogbox)

public:
	Tab_MemoryDialogbox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Tab_MemoryDialogbox();

// 对话框数据
	enum { IDD = IDD_Memory_optimization };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 获取CPU的占用率
	void GetCpuUsage();

private:
	// 权限
	enum MEMORYSTATE {
		e_stat_free = MEM_FREE,
		e_stat_reserve = MEM_RESERVE,
		e_stat_commit = MEM_COMMIT
	};
	// 三种内存分页类型
	enum MEMORYTYPE {
		e_type_image = MEM_IMAGE,
		e_type_mapped = MEM_MAPPED,
		e_type_private = MEM_PRIVATE,
	};
	// 结构体
	typedef struct VMINFO {
		DWORD		address;
		DWORD		size;
		MEMORYSTATE state;
	}VMINFO;
	// 保存List结构体
	list<VMINFO> vmlist;

private:
	// 获取虚拟内存的信息
	void GetMemoryInfo();
	// 堆内存遍历
	void queryViryualMemoryStatue(HANDLE hProccess);
	// 时间转换
	double FILETIMEDouble(const _FILETIME & filetime);
public:
	afx_msg void OnBnClickedButton8();
	CListCtrl m_ListCtr;
	CString m_Cpusyl;
	CString m_MemoryBFB;
	CString m_Pymemory;
	CString m_Pagesize;
	CString m_Memorysize;
	CString m_Kymemorysize;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
