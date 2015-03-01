#pragma once
class Utils
{
public:
	static DWORD GetModuleBase(char*);
	static DWORD GetModuleSize(char*);
	static DWORD FindPattern(char*, char*, DWORD, DWORD, DWORD);
};