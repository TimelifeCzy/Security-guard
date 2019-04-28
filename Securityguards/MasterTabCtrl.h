#pragma once


// MasterTabCtrl

class MasterTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(MasterTabCtrl)

public:
	MasterTabCtrl();
	virtual ~MasterTabCtrl();

// 公开接口
public:
	// 初始化Tab
	BOOL InitData(DWORD dwCount, ...);
	// 增加窗口
	BOOL AddData(DWORD dwCount, ...);
	// 设置
	BOOL SetSelAndShow(int nSel);
// 私有接口
private:
	// 参数
	CDialogEx* m_pDlg[10];
	DWORD m_dwCount = 0;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


