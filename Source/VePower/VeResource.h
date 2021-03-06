////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeResource.h
//  Created:     2015/08/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeResource : public VeRefObject
{
	VeNoCopy(VeResource);
	VeRTTIDecl(VeResource);
public:
	enum State
	{
		UNLOADED,
		LOADING,
		LOADED,
		INVALID
	};

	enum Error
	{
		ERR_FILE_NOT_FOUND,
		ERR_PARSE_FAILED,
		ERR_MAX
	};

	VeResource(const VeChar8* pcName) noexcept;

	virtual ~VeResource() noexcept;

	inline const VeChar8* GetName() const noexcept;

	inline const VeChar8* GetExt() const noexcept;

	inline const VeChar8* GetGroupName() const noexcept;

	inline const VeResourceGroupPtr& GetGroup() noexcept;

	inline State GetState() const noexcept;

	void SetGroup(const VeChar8* pcGroup) noexcept;

	void SetGroup(const VeResourceGroupPtr& spGroup) noexcept;

	void Hold() noexcept;

	void Free() noexcept;

	void Load() noexcept;

	void Unload() noexcept;

	virtual const VeChar8* GetTypeName() const noexcept = 0;

protected:
	void LoadAsync() noexcept;

	void UpdateState() noexcept;

	void Wait(VeResource* pkRes) noexcept;

	void WaitResLoaded(VeResource* pkRes) noexcept;

	void OnResLoaded() noexcept;

	void OnResUnloaded() noexcept;

	void OnResLoadFailed(Error eError) noexcept;

	virtual void LoadImpl(VeResourceManager::FileCachePtr spCache) noexcept;

	virtual void UnloadImpl() noexcept;

	VeRefNode<VeResource*> m_kNode;
	VeFixedString m_kName;
	VeResourceGroupPtr m_spGroup;
	State m_eState = UNLOADED;
	VeUInt32 m_u32WaitNumber = 0;
	VeList<VeResource*> m_kWaitList;
	VeUInt32 m_u32HoldCount = 0;
	VeUInt32 m_u32FreeTime = 0;

};

VeSmartPointer(VeResource);

#include "VeResource.inl"
