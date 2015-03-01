#pragma once

class Engine
{
public:
	Engine();
	~Engine();
	DWORD GetEntityList();
	DWORD GetLocalPlayer();
	DWORD GetEntityById(DWORD);
	BYTE GetEntitySpotted(DWORD);
	void SetEntitySpotted(DWORD, BYTE);
	DWORD GetEntityHealth(DWORD);
	DWORD GetEntityTeam(DWORD);
	DWORD GetEntityGlowIndex(DWORD);
	DWORD GetIncrossId();
	BYTE GetAttack();
	void SetAttack(BYTE);
	DWORD GetGlowObjects();
	DWORD GetGlowObjectCount();
	bool IsEntityPlayer(DWORD);
private:
	DWORD clientDll;
	DWORD engineDll;
	DWORD clientDllSize;
	DWORD engineDllSize;
	void GetOffsets();
};

