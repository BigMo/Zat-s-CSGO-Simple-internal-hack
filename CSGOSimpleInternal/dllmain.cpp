// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

//Variables
HANDLE threadLoop;

//Methods
void SetupConsole()
{
	//Allocate a console and make sure I can write to it
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	freopen("CONOUT$", "wb", stderr);
	freopen("CONIN$", "rb", stdin);
	SetConsoleTitle("CSGOSimpleInternal");
}

void Shoot(int duration)
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(duration / 2);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(duration / 2);
}

DWORD WINAPI Loop(PVOID pThreadParameter)
{
	printf("> Waiting for CSGO to load modules...\n");
	while (!GetModuleHandle("client.dll") || !GetModuleHandle("engine.dll"))
		Sleep(100);

	printf("> Modules loaded!\n");
	Engine lEngine;
	DWORD localPlayer = NULL;
	GlowObjectDefinition *glowObjects = NULL;
	while (threadLoop && !GetAsyncKeyState(VK_F5))
	{
		assert(threadLoop);
		Sleep(16);
		localPlayer = lEngine.GetLocalPlayer();
		if (!localPlayer) //LocalPlayer not initialized yet
		{
			continue;
		}
		//Set spotted
		for (int i = 0; i < 32; i++)
		{
			DWORD base = lEngine.GetEntityById(i);
			if (base)
				lEngine.SetEntitySpotted(base, 1);
		}

		//Triggerbot
		//DWORD inCrossId = lEngine.GetIncrossId();
		//printf("(InCrossId %i) ", inCrossId);
		//if (inCrossId > 0 && inCrossId < 65)
		//{
		//	DWORD enemy = lEngine.GetEntityById(inCrossId - 1);
		//	printf("(Enemy %#X) ", enemy);
		//	if (enemy)
		//	{
		//		if (lEngine.GetEntityTeam(enemy) != lEngine.GetEntityTeam(localPlayer))
		//		{
		//			if (lEngine.GetAttack() == 4 && GetAsyncKeyState(VK_LBUTTON) != 1) //Attack isn't currently down
		//			{
		//				Shoot(10);
		//			}
		//		}
		//	}
		//}

		//Glow
		DWORD glowBase = lEngine.GetGlowObjects();
		if (glowBase)
		{
			glowObjects = (GlowObjectDefinition*)glowBase;
			for (int i = 0; i < 64; i++)
			{
				DWORD player = lEngine.GetEntityById(i);
				if (!player)
					continue;
				DWORD glowIdx = lEngine.GetEntityGlowIndex(player);
				if (glowIdx >= 0 && glowIdx < lEngine.GetGlowObjectCount())
				{
					glowObjects[glowIdx].a = 0.9f;
					glowObjects[glowIdx].r = 0.0f;
					glowObjects[glowIdx].g = 0.0f;
					glowObjects[glowIdx].b = 0.0f;
					if (lEngine.GetEntityTeam(player) == 2) //T
					{
						glowObjects[glowIdx].r = 0.9f;
					}
					else //CT
					{
						glowObjects[glowIdx].b = 0.9f;
					}
					glowObjects[glowIdx].m_bRenderWhenOccluded = true;
					glowObjects[glowIdx].m_bRenderWhenUnoccluded = true;
				}
			}
		}
	}
	glowObjects = NULL;
	printf("> Exiting thread\n");
	FreeConsole();
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SetupConsole();
		threadLoop = CreateThread(0, 0, Loop, 0, 0, NULL);
		printf("[>]==-- Simple CSGO internal hack by Zat.\n");
		printf("(Press F5 to terminate this hack)\n");
		if (threadLoop)
			printf("> Thread started\n");
		else
			printf("> Thread could not be started\n");
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if (threadLoop)
			CloseHandle(threadLoop);
		break;
	}
	return TRUE;
}

