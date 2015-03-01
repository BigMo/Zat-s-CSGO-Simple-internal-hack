#include "stdafx.h"
#include "Engine.h"

namespace Offsets
{
	DWORD
		LocalPlayer = 0x00, //0x00A6C90C,
		EntityList = 0x00, //0x04A0F014,
		EntityLoopDist = 0x10,
		Ent_Spotted = 0x00, //0x935,
		Ent_Team = 0x00, //0xF0,
		Ent_Health = 0xFC,
		Ent_GlowIndex = 0x1DB8,
		Ent_VecOrigin = 0x00,
		Loc_InCross = 0x00,
		Loc_VecPunchAng = 0x00,
		Engine_SetViewAngles = 0x00,
		Btn_Attack = 0x00, //0x02E80F48,
		GlowObjectBase = 0x00, //0x04B1FAC4,
		GlowObjectCount = 0x04;

	bool FindOffset(DWORD moduleBase, DWORD moduleSize, char* pattern, char* mask, DWORD addressOffset, char* offsetName, DWORD* offsetAddress, bool subtractBase = false)
	{
		DWORD address = Utils::FindPattern(pattern, mask, moduleBase, moduleBase + moduleSize, addressOffset);
		if (address)
		{
			*offsetAddress = *(DWORD*)address;
			if (subtractBase)
				*offsetAddress -= moduleBase;
			//printf("[Offsets] %s = %#X\n", offsetName, *offsetAddress);
			return true;
		}
		printf("[Offsets] Offset %s not found!\n", offsetName);
		return false;
	}

	bool FindLocalPlayer(DWORD clientDll, DWORD clientDllSize)
	{
		DWORD address = Utils::FindPattern("\x8D\x34\x85\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x8B\x41\x08\x8B\x48\x00", "xxx????xx????xxxxx?", clientDll, clientDll + clientDllSize, 0);
		if (address)
		{
			LocalPlayer = *(DWORD*)(address + 3) + *(char*)(address + 18) - clientDll;
			//printf("[Offsets] LocalPlayer = %#X\n", LocalPlayer);
			return true;
		}
		printf("[Offsets] Offset LocalPlayer not found!\n");
		return false;
	}

	bool FindEntityList(DWORD clientDll, DWORD clientDllSize)
	{
		DWORD address = Utils::FindPattern("\x05\x00\x00\x00\x00\xC1\xE9\x00\x39\x48\x04", "x????xx?xxx", clientDll, clientDll + clientDllSize, 0);
		if (address)
		{
			EntityList = *(DWORD*)(address + 1) + *(char*)(address + 7) - clientDll;
			//printf("[Offsets] EntityList = %#X\n", LocalPlayer);
			return true;
		}
		printf("[Offsets] Offset EntityList not found!\n");
		return false;
	}

	void GetOffsets(DWORD clientDll, DWORD clientDllSize, DWORD engineDll, DWORD engineDLlSize)
	{
		printf("[Offsets] Getting offsets...\n");
		bool offsetsComplete = true;
		if (!FindLocalPlayer(clientDll, clientDllSize))
			offsetsComplete = false;
		if (!FindEntityList(clientDll, clientDllSize))
			offsetsComplete = false;

		if (!FindOffset(clientDll, clientDllSize,
			"\x56\x57\x8B\xF9\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00\x8B\x0D", "xxxxxx????xx",
			6, "Loc_InCross", &Loc_InCross))
			offsetsComplete = false;
		if (!FindOffset(clientDll, clientDllSize,
			"\x89\x15\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\xF6\xC2\x03\x74\x03\x83\xCE\x04\xA8\x04\xBF", "xx????xx????xxxxxxxxxxx",
			2, "Btn_Attack", &Btn_Attack, true))
			offsetsComplete = false;
		if (!FindOffset(clientDll, clientDllSize,
			"\x8D\x8F\x00\x00\x00\x00\xA1\x00\x00\x00\x00\xC7\x04\x02\x00\x00\x00\x00\x89\x35\x00\x00\x00\x00\x8B\x51", "xx????x????xxx????xx????xx",
			7, "GlowObjectBase", &GlowObjectBase, true))
			offsetsComplete = false;
		if (!FindOffset(clientDll, clientDllSize,
			"\x8B\x47\x04\x8B\x4F\x08\x83\xB8\x00\x00\x00\x00\x00\x0F\x8E", "xxxxxxxx????xxx",
			8, "Ent_Health", &Ent_Health))
			offsetsComplete = false;
		if (!FindOffset(clientDll, clientDllSize,
			"\xCC\xCC\xCC\xCC\x8B\x89\x00\x00\x00\x00\xE9\x00\x00\x00\x00\xCC\xCC\xCC\xCC", "xxxxxx????x????xxxx",
			6, "Ent_Team", &Ent_Team))
			offsetsComplete = false;
		if (!FindOffset(clientDll, clientDllSize,
			"\xCC\xCC\xCC\xCC\x80\xB9\x00\x00\x00\x00\x00\x74\x12\x8B\x41\x08\x83\xC1\x08\x8B\x40\x24", "xxxxxx????xxxxxxxxxxxx",
			6, "Ent_Spotted", &Ent_Spotted))
			offsetsComplete = false;
		if (!FindOffset(clientDll, clientDllSize,
			"\xEB\x03\x0F\x57\xC0\x8B\xB7\x00\x00\x00\x00\xF3\x0F\x11\x45\xE8\xE8", "xxxxxxx????xxxxxx",
			7, "Ent_GlowIndex", &Ent_GlowIndex))
			offsetsComplete = false;
		printf("[Offsets] Scan completed\n");
		if (!offsetsComplete)
		{
			printf("[WARNING] Could not find one or more offsets! Further usage of this hack may crash the game!\n");
		}
	}
}

Engine::Engine()
{
	clientDll = (DWORD)GetModuleHandle("client.dll");
	engineDll = (DWORD)GetModuleHandle("engine.dll");
	clientDllSize = Utils::GetModuleSize("client.dll");
	engineDllSize = Utils::GetModuleSize("engine.dll");
	assert(clientDll);
	assert(engineDll);
	assert(clientDllSize);
	assert(engineDllSize);
	printf("[Engine] Intitialized, modules:\n");
	printf("> client.dll [at %#X, size %i bytes]\n", clientDll, clientDllSize);
	printf("> engine.dll [at %#X, size %i bytes]\n", engineDll, engineDllSize);
	GetOffsets();
}

Engine::~Engine()
{
}

void Engine::GetOffsets()
{
	Offsets::GetOffsets(clientDll, clientDllSize, engineDll, engineDllSize);
}

DWORD Engine::GetEntityList()
{
	//printf("[Engine] GetEntityList\n");
	return clientDll + Offsets::EntityList;
}
DWORD Engine::GetLocalPlayer()
{
	//printf("[Engine] GetLocalPlayer\n");
	return *(DWORD*)(clientDll + Offsets::LocalPlayer);
}
DWORD Engine::GetIncrossId()
{
	DWORD localPlayer = this->GetLocalPlayer();
	if (!localPlayer)
		return NULL;
	return *(DWORD*)(localPlayer + Offsets::Loc_InCross);
}
DWORD Engine::GetEntityById(DWORD _id)
{
	//printf("[Engine] GetEntityById\n");
	DWORD entityList = this->GetEntityList();
	//printf("[Engine] GetEntityById, entitylist %#X\n", entityList);
	if (!entityList)
		return NULL;
	return *(DWORD*)(entityList + Offsets::EntityLoopDist * _id);
}
BYTE Engine::GetEntitySpotted(DWORD _base)
{
	//printf("[Engine] GetEntitySpotted\n");
	return *(BYTE*)(_base + Offsets::Ent_Spotted);
}
void Engine::SetEntitySpotted(DWORD _base, BYTE _value)
{
	//printf("[Engine] SetEntitySpotted\n");
	*(BYTE*)(_base + Offsets::Ent_Spotted) = _value;
}
DWORD Engine::GetEntityHealth(DWORD _base)
{
	return *(DWORD*)(_base + Offsets::Ent_Health);
}
DWORD Engine::GetEntityTeam(DWORD _base)
{
	return *(DWORD*)(_base + Offsets::Ent_Team);
}
BYTE Engine::GetAttack()
{
	return *(BYTE*)(clientDll + Offsets::Btn_Attack);
}
void Engine::SetAttack(BYTE _value)
{
	*(BYTE*)(clientDll + Offsets::Btn_Attack) = _value;
}
DWORD Engine::GetGlowObjects()
{
	return *(DWORD*)(clientDll + Offsets::GlowObjectBase);
}
DWORD Engine::GetGlowObjectCount()
{
	return *(DWORD*)(clientDll + Offsets::GlowObjectBase + Offsets::GlowObjectCount);
}
bool Engine::IsEntityPlayer(DWORD _base)
{
	DWORD entity = NULL;
	for (int i = 0; i < 64; i++)
	{
		entity = this->GetEntityById(i);
		if (entity == _base)
			return true;
	}
	return false;
}
DWORD Engine::GetEntityGlowIndex(DWORD _base)
{
	return *(DWORD*)(_base + Offsets::Ent_GlowIndex);
}