////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderBuffer.inl
//  Created:     2015/09/10 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeRenderBuffer::Type VeRenderBuffer::GetType() noexcept
{
	return m_eType;
}
//--------------------------------------------------------------------------
inline VeRenderBuffer::Useage VeRenderBuffer::GetUseage() noexcept
{
	return m_eUseage;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeRenderBuffer::GetSize() noexcept
{
	return m_u32Size;
}
//--------------------------------------------------------------------------
inline bool VeRenderBuffer::CanUse(Useage eUse) noexcept
{
	return VE_MASK_HAS_ALL(GetUseage(), eUse);
}
//--------------------------------------------------------------------------
