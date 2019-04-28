// MasterTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "MasterTabCtrl.h"
#include <list>

// MasterTabCtrl

IMPLEMENT_DYNAMIC(MasterTabCtrl, CTabCtrl)

MasterTabCtrl::MasterTabCtrl()
{

}

MasterTabCtrl::~MasterTabCtrl()
{
}


BEGIN_MESSAGE_MAP(MasterTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &MasterTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()


// MasterTabCtrl 消息处理程序

BOOL MasterTabCtrl::InitData(DWORD dwCount, ...)
{
	va_list va;
	va_start(va, dwCount);
	for (DWORD i = 0; i < dwCount; ++i)
	{
		TCHAR* pStrTabName = va_arg(va, TCHAR *);
		InsertItem(i,  pStrTabName);
	}
	va_end(va);
	m_dwCount = dwCount;
	return TRUE;
}

BOOL MasterTabCtrl::AddData(DWORD dwCount, ...)
{
	va_list va_prt;
	va_start(va_prt, dwCount);
	for (DWORD i = 0; i < dwCount; ++i)
	{
		UINT uId = va_arg(va_prt, UINT);
		m_pDlg[i] = va_arg(va_prt, CDialogEx*);
		m_pDlg[i]->Create(uId, this);
		SetSelAndShow(i);
	}
	va_end(va_prt);
	SetCurSel(0);
	m_pDlg[0]->ShowWindow(SW_SHOW);
	return TRUE;
}

BOOL MasterTabCtrl::SetSelAndShow(int nSel)
{
	CRect rc;
	GetClientRect(rc);
	rc.DeflateRect(1, 27, 3, 3);
	m_pDlg[nSel]->MoveWindow(rc);
	return TRUE;
}

void MasterTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	DWORD dwSel = this->GetCurSel();
	for (DWORD i = 0; i < m_dwCount; ++i)
	{
		if (dwSel == i)
			m_pDlg[i]->ShowWindow(SW_SHOW);
		else
			m_pDlg[i]->ShowWindow(SW_HIDE);
	}
	*pResult = 0;
}
