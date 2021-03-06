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
	mtlStringMap< retro3d::Schematic< type_t* > > m_schematics;

public:
	bool    AddSchematic(const std::string &name, retro3d::Schematic< type_t* > schematic);
	type_t *Assemble(const std::string &name) const;
};

}

template < typename type_t >
bool retro3d::Factory<type_t>::AddSchematic(const std::string &name, retro3d::Schematic< type_t* > schematic)
{
	const mtlChars chars = mtlChars::FromDynamic(name.c_str(), int(name.size()));
	retro3d::Schematic< type_t* > *entry = m_schematics.GetEntry(chars);

	RETRO3D_ASSERT(entry == nullptr);

	if (entry != nullptr) { return false; }

	entry = m_schematics.CreateEntry(chars);
	*entry = schematic;

	return true;
}

template < typename type_t >
type_t *retro3d::Factory<type_t>::Assemble(const std::string &name) const
{
	const mtlChars chars = mtlChars::FromDynamic(name.c_str(), int(name.size()));
	const retro3d::Schematic<type_t*> *entry = m_schematics.GetEntry(chars);
	return entry != nullptr ? (*entry)() : nullptr;
}

#endif // RETRO_FACTORY_H
