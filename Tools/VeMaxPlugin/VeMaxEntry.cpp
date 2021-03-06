////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   Entry.cpp
//  Created:     2015/09/08 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//--------------------------------------------------------------------------
HINSTANCE g_hInstance = nullptr;
//--------------------------------------------------------------------------
VeMaxEntry::VeMaxEntry() noexcept
{

}
//--------------------------------------------------------------------------
VeMaxEntry::~VeMaxEntry() noexcept
{
	VE_ASSERT(m_kClassList.empty());
}
//--------------------------------------------------------------------------
void VeMaxEntry::Init() noexcept
{
	m_kClassList.push_back(VeExportClassDesc::Get());
}
//--------------------------------------------------------------------------
void VeMaxEntry::Term() noexcept
{
	m_kClassList.clear();
}
//--------------------------------------------------------------------------
VeInt32 VeMaxEntry::GetClassNum() noexcept
{
	return (VeInt32)m_kClassList.size();
}
//--------------------------------------------------------------------------
ClassDesc* VeMaxEntry::GetClass(VeInt32 i32Index) noexcept
{
	if (i32Index >= 0 && i32Index < m_kClassList.size())
	{
		return m_kClassList[i32Index];
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeFixedString ToStr(const TCHAR* pcStr) noexcept
{
	VeChar8* pcBuffer = nullptr;
	VeInt32 i32Size;
#	ifdef _UNICODE
	i32Size = WideCharToMultiByte(CP_UTF8, 0, pcStr, -1, 0, 0, 0, FALSE);
	if (i32Size > 0)
	{
		pcBuffer = VeStackAlloc(VeChar8, i32Size + 1);
		WideCharToMultiByte(CP_UTF8, 0, pcStr,
			-1, pcBuffer, i32Size, 0, FALSE);
	}
#	else
	i32Size = (VeInt32)VeStrlen(pcStr);
	if (i32Size > 0)
	{
		pcBuffer = VeStackAlloc(VeChar8, i32Size + 1);
		VeStrcpy(pcBuffer, i32Size + 1, pcStr);
	}
#	endif
	VeFixedString kRes;
	if (pcBuffer)
	{
		kRes = pcBuffer;
		VeStackFree(pcBuffer);
		pcBuffer = nullptr;
	}
	return kRes;
}
//--------------------------------------------------------------------------
VeFixedString ToStrReplaceIegality(const TCHAR* pcStr) noexcept
{
	VeChar8* pcBuffer = nullptr;
	VeInt32 i32Size;
#	ifdef _UNICODE
	i32Size = WideCharToMultiByte(CP_UTF8, 0, pcStr, -1, 0, 0, 0, FALSE);
	if (i32Size > 0)
	{
		pcBuffer = VeStackAlloc(VeChar8, i32Size + 1);
		WideCharToMultiByte(CP_UTF8, 0, pcStr,
			-1, pcBuffer, i32Size, 0, FALSE);
	}
#	else
	i32Size = (VeInt32)VeStrlen(pcStr);
	if (i32Size > 0)
	{
		pcBuffer = VeStackAlloc(VeChar8, i32Size + 1);
		VeStrcpy(pcBuffer, i32Size + 1, pcStr);
	}
#	endif
	VeFixedString kRes;
	if (pcBuffer)
	{
		{
			VeChar8* pcTemp = pcBuffer;
			while (*pcTemp)
			{
				VeChar8 c = *pcTemp;
				if (c == ' ' || c == '/' || c == '\\' || c == ':' || c == '#' || c == '.')
				{
					*pcTemp = '_';
				}
				++pcTemp;
			}
		}
		kRes = pcBuffer;
		VeStackFree(pcBuffer);
		pcBuffer = nullptr;
	}
	return kRes;
}
//--------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		g_hInstance = hinstDLL;
		DisableThreadLibraryCalls(g_hInstance);
	}
	return TRUE;
}
//--------------------------------------------------------------------------
VE_MAX_EXPORT const TCHAR* LibDescription()
{
	return VE_MAX_LIB_DESC;
}
//--------------------------------------------------------------------------
VE_MAX_EXPORT int LibNumberClasses()
{
	return ve_max_ent.GetClassNum();
}
//--------------------------------------------------------------------------
VE_MAX_EXPORT ClassDesc* LibClassDesc(int i)
{
	return ve_max_ent.GetClass(i);
}
//--------------------------------------------------------------------------
VE_MAX_EXPORT ULONG LibVersion()
{
	return VERSION_3DSMAX;
}
//--------------------------------------------------------------------------
VE_MAX_EXPORT int LibInitialize(void)
{
	VeStringTable::Create();
	VeSystem::Create(VeSystem::TYPE_DEFAULT, "com.VenusIE.VeMaxPlugin");
	VeEngine::Create();
	VeMaxEntry::Create();
	VeMaxSceneLoader::Create();
	ve_sys.Init();
	ve_engine.Init();
	ve_max_ent.Init();
	return TRUE;
}
//--------------------------------------------------------------------------
VE_MAX_EXPORT int LibShutdown(void)
{
	ve_max_ent.Term();
	ve_engine.Term();
	ve_sys.Term();
	VeMaxSceneLoader::Destory();
	VeMaxEntry::Destory();
	VeEngine::Destory();
	VeSystem::Destory();
	VeStringTable::Destory();
#	ifdef VE_MEM_DEBUG
	_VeMemoryExit();
#	endif
	return TRUE;
}
//--------------------------------------------------------------------------
