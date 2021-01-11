#include "retro_config.h"

void retro3d::Configs::Config::Set(const mtlChars &name, const mtlChars &value)
{
	auto *e = m_map.GetEntry(name);
	if (e == nullptr) {
		e = m_map.CreateEntry(name);
	}
	*e = value;
}

void retro3d::Configs::Set(mtlChars name, mtlChars value)
{
	mtlSyntaxParser p;
	p.SetBuffer(name);
	mtlArray<mtlChars> m;

	mtlChars subname;

	switch (p.Match("%s.%S %| %S", m)) {
	case 0:
		name = m[0].GetTrimmed();
		name = name.GetSize() > 0 ? name : "global";
		subname = m[1].GetTrimmed();
		break;
	case 1:
		name = "global";
		subname = m[0].GetTrimmed();
		break;
	}

	auto *e = m_map.GetEntry(name);
	if (e == nullptr) {
		e = m_map.CreateEntry(name);
	}
	e->Set(subname, value);
}

bool retro3d::Configs::Read(const mtlChars &filename)
{
	if (!mtlBufferFile(filename, m_buffer)) { return false; }
	mtlSyntaxParser p;
	p.SetBuffer(m_buffer);
	mtlArray<mtlChars> m;
	while (!p.IsEnd()) {
		switch (p.Match("%S=\"%s\" %| %S=%w %| %s", m)) {
		case 0:
		case 1:
			Set(m[0], m[1]);
			break;
		default:
			break;
		}
	}
	return true;
}

bool retro3d::Configs::Read(const mtlChars &filename, const mtlChars &section)
{
	// Refresh the contents of a Config
	return false;
}

bool retro3d::Configs::Write(const mtlChars &filename)
{
	return false;
}

void retro3d::Configs::Clear( void )
{
	m_map.RemoveAll();
	m_buffer.Free();
}
