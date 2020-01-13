#ifndef RETRO3D_TRANSFORM_COMPONENT_H
#define RETRO3D_TRANSFORM_COMPONENT_H

#include "../../common/MiniLib/MTL/mtlPointer.h"
#include "../../common/retro_transform.h"
#include "../../physics/retro_collider.h"
#include "../retro_component.h"

namespace retro3d
{

retro_component(TransformComponent)
{
private:
	retro3d::SharedTransform m_transform;
	retro3d::Transform       m_old_transform; // or should we store next_transform?

protected:
	void OnUpdate( void );

public:
	TransformComponent( void );

	const retro3d::SharedTransform GetTransform( void ) const;
	retro3d::SharedTransform       GetTransform( void );

	mmlVector<3> GetLastMove( void ) const;
	retro3d::Transform GetLastTransform( void ) const;
};

}

#endif // RETRO3D_TRANSFORM_COMPONENT_H
