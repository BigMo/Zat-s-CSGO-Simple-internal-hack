#include "stdafx.h"

struct GlowObjectDefinition
{
	DWORD pEntity;
	float r;
	float g;
	float b;
	float a;
	BYTE pad0[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
	BYTE pad1[10];
};