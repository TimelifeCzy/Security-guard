#include "ProgramStart.h"
#include "MasterWindows.h"

ProgramStatic g_WinApp;		// 建立应用程序

ProgramStatic::ProgramStatic()
{

}

ProgramStatic::~ProgramStatic()
{

}

BOOL ProgramStatic::InitInstance()
{
	// MasterWindows* obj_master = NULL;
	MasterWindows obj_master;
	m_pMainWnd = &obj_master;
	obj_master.DoModal();
	return TRUE;
}

