////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeParser.h
//  Created:     2015/09/11 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

VE_POWER_API void VeAddParserDestructor(std::function<void()> funcDest) noexcept;

template<class _Ty>
class VeEnumParser : public VeSingleton<VeEnumParser<_Ty>>
{
public:
	static_assert(std::is_enum<_Ty>::value, "_Ty has to be a enum");

	VeEnumParser(std::initializer_list < std::pair <
		_Ty, const VeChar8* >> kList) noexcept
	{
		for (auto pair : kList)
		{
			m_kValueMap[pair.first] = pair.second;
			m_kNameMap[pair.second] = pair.first;
		}
		VeAddParserDestructor([]() noexcept
		{
			VeEnumParser<_Ty>::Destory();
		});
	}

	~VeEnumParser() noexcept
	{
		m_kNameMap.clear();
		m_kValueMap.clear();
	}
	
	_Ty ValueToEnum(VeUInt32 u32Value, _Ty eDefault) noexcept
	{
		auto it = m_kValueMap.find(_Ty(u32Value));
		if (it != m_kValueMap.end())
		{
			return it->first;
		}
		return eDefault;
	}

	_Ty ParseEnum(const VeChar8* pcStr, _Ty eDefault) noexcept
	{
		auto it = m_kNameMap.find(pcStr);
		if (it != m_kNameMap.end())
		{
			return it->second;
		}
		return eDefault;
	}

	_Ty ParseFlags(const VeChar8* pcStr, _Ty eStart) noexcept
	{
		VeUInt32 u32Flags = eStart;
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeStrcpy(acBuffer, pcStr);
		VeChar8* pcContext;
		VeChar8* pcTemp = VeStrtok(acBuffer, "|", &pcContext);
		while (pcTemp)
		{
			pcTemp = VeTrim(pcTemp);
			auto it = m_kNameMap.find(pcTemp);
			if (it != m_kNameMap.end())
			{
				u32Flags |= it->second;
			}
			pcTemp = VeStrtok<VeChar8>(nullptr, "|", &pcContext);
		}
		return (_Ty)u32Flags;
	}

	VeFixedString EnumToStr(_Ty eFlags) noexcept
	{
		auto it = m_kValueMap.find(eFlags);
		if (it != m_kValueMap.end())
		{
			return it->second;
		}
		return "";
	}

	VeFixedString FlagsToStr(_Ty eFlags) noexcept
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeChar8* pcTemp = nullptr;
		for (auto val : m_kValueMap)
		{
			if (VE_MASK_HAS_ALL(eFlags, val.first))
			{
				if (pcTemp)
				{
					pcTemp += VeSprintf(pcTemp, VE_MAX_PATH_LEN - (pcTemp - acBuffer),
						"|%s", (const VeChar8*)val.second);
				}
				else
				{
					pcTemp = VeSprintf(acBuffer, (const VeChar8*)val.second);
				}				
			}
		}
		return acBuffer;
	}

protected:
	VeMap<_Ty, VeFixedString> m_kValueMap;
	VeStringMap<_Ty> m_kNameMap;

};

#define VE_ENUM(ty,...) VE_NEW VeEnumParser<ty>(__VA_ARGS__)

namespace venus
{
	template <class _Ty>
	struct ValueParser;
}

class VE_POWER_API VeParser : public VeSingleton<VeParser>
{
	VeNoCopy(VeParser);
public:
	enum TokenType
	{
		TYPE_NUMBER,
		TYPE_STRING,
		TYPE_MAX
	};

	struct TokenStr
	{
		const VeChar8* m_pcStr;
		VeSizeT m_stLen;
	};

	struct Token
	{
		TokenType m_eType;
		union
		{
			VeFloat64 m_f64Number;
			TokenStr m_kString;
		};
	};

	VeParser() noexcept;

	virtual ~VeParser() noexcept;

	template<class _Ty>
	_Ty Parse(const VeChar8* pcStr, _Ty eDefault) noexcept
	{
		return venus::ValueParser<_Ty>::Parse(pcStr, eDefault);
	}

	template<class _Ty>
	_Ty ValueToEnum(VeUInt32 u32Value, _Ty eDefault) noexcept
	{
		return VeEnumParser<_Ty>::GetSingleton().ValueToEnum(u32Value, eDefault);
	}

	template<class _Ty>
	_Ty ParseEnum(const VeChar8* pcStr, _Ty eDefault) noexcept
	{
		return VeEnumParser<_Ty>::GetSingleton().ParseEnum(pcStr, eDefault);
	}

	template<class _Ty>
	_Ty ParseFlags(const VeChar8* pcStr, _Ty eStart) noexcept
	{
		return VeEnumParser<_Ty>::GetSingleton().ParseFlags(pcStr, eStart);
	}

	template<class _Ty>
	VeFixedString EnumToStr(_Ty eFlags) noexcept
	{
		return VeEnumParser<_Ty>::GetSingleton().EnumToStr(eFlags);
	}

	template<class _Ty>
	VeFixedString FlagsToStr(_Ty eFlags) noexcept
	{
		return VeEnumParser<_Ty>::GetSingleton().FlagsToStr(eFlags);
	}

	inline VeFloat64 GetValue(const VeChar8* pcName) noexcept;
	
	inline void SetValue(const VeChar8* pcName, VeFloat64 f64Value) noexcept;

	void AddDestructor(std::function<void()> funcDest) noexcept;

	VeFloat64 CalculateExpression(const VeChar8* pcExpr, VeFloat64 f64Default) noexcept;

	VeFloat64 CalculateExpression(const VeStringMap<VeFloat64>& kMap,
		const VeChar8* pcExpr, VeFloat64 f64Default) noexcept;

protected:
	VeVector<std::function<void()>> m_kDestructList;
	VeStringMap<VeInt32> m_kOpMap;
	VeStringMap<VeFloat64> m_kValueMap;
	

};

#define ve_parser VeParser::GetSingleton()

namespace venus
{
	template <class _Ty>
	struct EnumParser
	{
		static_assert(std::is_enum<_Ty>::value, "_Ty has to be a enum");

		static _Ty Parse(const VeChar8* str, _Ty def) noexcept
		{
			return ve_parser.ParseEnum(str, def);
		}
	};

	template <class _Ty>
	struct NumberParser
	{
		static_assert(std::is_arithmetic<_Ty>::value, "_Ty has to be a number");

		static _Ty Parse(const VeChar8* str, _Ty def) noexcept
		{
			return (_Ty)ve_parser.CalculateExpression(str, (VeFloat64)def);
		}
	};

	template <class _Ty>
	struct NormalParser
	{
		static _Ty Parse(const VeChar8* str, _Ty def) noexcept
		{
			return _Ty::Parse(str, def);
		}
	};

	template <class _Ty>
	struct ValueParser : std::conditional <std::is_enum<_Ty>::value, EnumParser<_Ty>,
		typename std::conditional < std::is_arithmetic<_Ty>::value, NumberParser<_Ty>,
		NormalParser<_Ty> >::type > ::type {};
}

#include "VeParser.inl"
