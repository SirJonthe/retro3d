#include "retro_serialize.h"

retro3d::Serializeable::Serializeable( void ) : mtlBase(this)
{}

retro3d::Serializeable::Serializeable(const Serializeable&) : mtlBase(this)
{}

retro3d::Serializeable::Serializeable(Serializeable&&) : mtlBase(this)
{}

retro3d::Serializeable &retro3d::Serializeable::operator=(const retro3d::Serializeable&)
{
	return *this;
}

bool retro3d::Serializeable::Deserialize(retro3d::Reader&)
{
	return false;
}

bool retro3d::Serializeable::Serialize(const mtlPath&) const
{
	return false;
}
