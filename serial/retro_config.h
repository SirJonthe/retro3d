#ifndef RETRO_CONFIG_H
#define RETRO_CONFIG_H

#include "../common/MiniLib/MTL/mtlParser.h"
#include "../common/MiniLib/MTL/mtlStringMap.h"
#include "retro_serialize.h"

namespace retro3d
{

template < typename type_t >
struct ParseValue
{
	type_t value;
	bool   success;
	operator bool( void ) const { return success; }
};

template < typename type_t >
ParseValue<type_t> FromString(const mtlChars &str)
{
	ParseValue<type_t> o;
	o.success = o.value.Deserialize(str);
	return o;
}

template <>
ParseValue<mtlChars> FromString<mtlChars>(const mtlChars &str)
{
	ParseValue<mtlChars> o;
	o.success = true;
	o.value = str;
	return o;
}

template <>
ParseValue<int> FromString<int>(const mtlChars &str)
{
	ParseValue<int> o;
	o.success = str.ToInt(o.value);
	return o;
}

template <>
ParseValue<float> FromString<float>(const mtlChars &str)
{
	ParseValue<float> o;
	o.success = str.ToFloat(o.value);
	return o;
}

template <>
ParseValue<bool> FromString<bool>(const mtlChars &str)
{
	ParseValue<bool> o;
	o.success = str.ToBool(o.value);
	return o;
}

class Configs
{
private:
	class Config
	{
	private:
		mtlStringMap<mtlChars> m_map;

	public:
		template < typename type_t > ParseValue<type_t> Get(const mtlChars &name) const;
		void Set(const mtlChars &name, const mtlChars &value);
	};

private:
	mtlString            m_buffer;
	mtlStringMap<Config> m_map;

public:
	template < typename type_t > ParseValue<type_t> Get(const mtlChars &name) const;
	void Set(mtlChars name, mtlChars value);
	bool Read(const mtlChars &filename);
	bool Read(const mtlChars &filename, const mtlChars &section);
	bool Write(const mtlChars &filename);
	void Clear( void );
};

template < typename type_t >
ParseValue<type_t> retro3d::Configs::Config::Get(const mtlChars &name) const
{
	auto *e = m_map.GetEntry(name);
	return e != nullptr ?
		FromString<type_t>(*e) :
		ParseValue<type_t>{ type_t(), false };
}

template < typename type_t >
ParseValue<type_t> retro3d::Configs::Get(const mtlChars &name) const
{
	mtlSyntaxParser p;
	p.SetBuffer(name);
	mtlArray<mtlChars> m;
	switch (p.Match("%s.%S %| %S", m)) {
	case 0: {
		m[0] = m[0].GetTrimmed();
		m[1] = m[1].GetTrimmed();
		auto *e = m_map.GetEntry(m[0].GetSize() > 0 ? m[0] : "global");
		return e != nullptr ? e->Get<type_t>(m[1]) : ParseValue<type_t>{ type_t(), false };
	}
	case 1: {
		m[0] = m[0].GetTrimmed();
		auto *e = m_map.GetEntry("global");
		return e != nullptr ? e->Get<type_t>(m[0]) : ParseValue<type_t>{ type_t(), false };
	}
	}
	return ParseValue<type_t>{ type_t(), false };
}

}

#endif // RETRO_CONFIG_H
