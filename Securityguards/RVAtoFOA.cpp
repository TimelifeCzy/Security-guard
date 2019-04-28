#include "RVAtoFOA.h"

char* RVAtoFOA::g_NamelpBase = 0;

RVAtoFOA::RVAtoFOA()
{

}

RVAtoFOA::~RVAtoFOA()
{

}

// RVA --> offset
DWORD RVAtoFOA::RVAofFOAs(const DWORD dwRVA)
{
	// 1.获取DOS头
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)g_NamelpBase;
	// 2.获取PE头
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + g_NamelpBase);
	// 3.获取段头
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	// 4.获取区段数量
	DWORD dwCount = pNt->FileHeader.NumberOfSections;
	// 5.循环判断区段在那个区域
	for (DWORD i = 0; i < dwCount; ++i)
	{
		// 6.在该区段
		if ((dwRVA >= pSection->VirtualAddress) && (dwRVA < (pSection->VirtualAddress + pSection->SizeOfRawData))){
			// 7.返回 RVA（转） - RVA（区）+ offset(区) 
			return dwRVA - pSection->VirtualAddress + pSection->PointerToRawData;
		}
		++pSection;
	}
	return 0;
}

_SAVERVAOFOFFSET RVAtoFOA::GetRvaofFOA(const DWORD dwRVA)
{
	_SAVERVAOFOFFSET sa = { 0 };
	// 1.获取DOS头
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)g_NamelpBase;
	// 2.获取PE头
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + g_NamelpBase);
	// 3.获取段头
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	// 4.获取区段数量
	DWORD dwCount = pNt->FileHeader.NumberOfSections;
	// 5.循环判断区段在那个区域
	for (DWORD i = 0; i < dwCount; ++i)
	{
		// 6.在该区段
		if ((dwRVA >= pSection->VirtualAddress) && (dwRVA < (pSection->VirtualAddress + pSection->SizeOfRawData))){
			// 7.返回 RVA（转） - RVA（区）+ offset(区) 
			sa.Rva = pSection->VirtualAddress;
			sa.offset =  pSection->PointerToRawData;
			return sa;
		}
		++pSection;
	}
	return sa;
}