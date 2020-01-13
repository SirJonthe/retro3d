#ifndef RETRO_TRANSFORM_SYSTEM_H
#define RETRO_TRANSFORM_SYSTEM_H

#include "../retro_system.h"
#include "../components/retro_transform_component.h"

namespace retro3d
{

retro_system(TransformSystem, retro3d::TransformComponent)
{
public:
	TransformSystem( void );
};

}

#endif // RETRO_TRANSFORM_SYSTEM_H
