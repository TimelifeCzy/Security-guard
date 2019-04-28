#include <afxwin.h>
#include <windows.h>

/*
	Class Name :		ProgramStatic
	The Functionality:	用户调用主窗口
*/

class ProgramStatic : public CWinApp
{
public:
	ProgramStatic();
	virtual ~ProgramStatic() override;
private:
	virtual BOOL InitInstance() override;
};
