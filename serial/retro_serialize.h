#ifndef RETRO_SERIALIZE_H
#define RETRO_SERIALIZE_H

#include "../common/MiniLib/MTL/mtlType.h"
#include "../common/MiniLib/MTL/mtlPath.h"
#include "retro_reader.h"

namespace retro3d
{

class Serializeable : public mtlBase
{
public:
	Serializeable( void );
	Serializeable(const Serializeable&);
	Serializeable(Serializeable&&);

	Serializeable &operator=(const Serializeable&);

	virtual bool Deserialize(retro3d::Reader &reader);
	virtual bool Serialize(const mtlPath &file_name) const;
};

}

#endif // RETRO_SERIALIZE_H
