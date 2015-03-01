#include "stdafx.h"
#include "Utils.h"


DWORD Utils::GetModuleBase(char* moduleName)
{
	HANDLE hSnap;
	MODULEENTRY32 xModule;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(GetCurrentProcess()));
	xModule.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(hSnap, &xModule)) {
		while (Module32Next(hSnap, &xModule)) {
			if (!strncmp((char*)xModule.szModule, moduleName, 8)) {
				CloseHandle(hSnap);
				return *(DWORD*)xModule.modBaseAddr;
			}
		}
	}
	CloseHandle(hSnap);
	return 0;
}
DWORD Utils::GetModuleSize(char* moduleName)
{
	HANDLE hSnap;
	MODULEENTRY32 xModule;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(GetCurrentProcess()));
	xModule.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(hSnap, &xModule)) {
		while (Module32Next(hSnap, &xModule)) {
			if (!strncmp((char*)xModule.szModule, moduleName, 8)) {
				CloseHandle(hSnap);
				return (DWORD)xModule.modBaseSize;
			}
		}
	}
	CloseHandle(hSnap);
	return 0;
}

/*
	Scans memory from 'start' to 'end' for the given pattern and mask.
	Returns the address of the where the pattern was found plus the given offset,
	returns 0 if the pattern was not found.
*/
DWORD Utils::FindPattern(char* pattern, char* mask, DWORD start, DWORD end, DWORD offset)
{
	int patternLength = strlen(mask);
	bool found = false;

	//For each byte from start to end
	for (DWORD i = start; i < end - patternLength; i++)
	{
		found = true;
		//For each byte in the pattern
		for (int idx = 0; idx < patternLength; idx++)
		{
			if (mask[idx] == 'x' && pattern[idx] != *(char*)(i + idx))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return i + offset;
		}
	}
	return NULL;
}