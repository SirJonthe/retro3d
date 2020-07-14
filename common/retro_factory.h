#ifndef RETRO_FACTORY_H
#define RETRO_FACTORY_H

#include <string>
#include "MiniLib/MTL/mtlStringMap.h"
#include "retro_defs.h"
#include "retro_assert.h"

namespace retro3d
{

template < typename type_t >
class Factory
{
private:
	mtlStringMap< retro3d::Schematic< type_t > > m_schematics;

public:
	bool AddSchematic(const std::string &name, retro3d::Schematic< type_t > schematic);
	bool Assemble(const std::string &name, type_t *assembly) const;
};

}

template < typename type_t >
bool retro3d::Factory<type_t>::AddSchematic(const std::string &name, retro3d::Schematic< type_t > schematic)
{
	const mtlChars chars = mtlChars::FromDynamic(name.c_str(), int(name.size()));
	retro3d::Schematic< type_t > *entry = m_schematics.GetEntry(chars);

	RETRO3D_ASSERT(entry == nullptr);

	if (entry != nullptr) { return false; }

	entry = m_schematics.CreateEntry(chars);
	*entry = schematic;

	return true;
}

template < typename type_t >
bool retro3d::Factory<type_t>::Assemble(const std::string &name, type_t *assembly) const
{
	const mtlChars chars = mtlChars::FromDynamic(name.c_str(), int(name.size()));
	type_t *entry = m_schematics.GetEntry(name);
	if (entry == nullptr) { return false; }
	(*entry)(assembly);
	return true;
}

#endif // RETRO_FACTORY_H
