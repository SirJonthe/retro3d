#include "retro_dynref.h"

retro3d::IDynRef::IDynRef( void ) : m_self_ref(mtlShared< retro3d::IDynRef*>::Create())
{
	*m_self_ref.GetShared() = this;
}

retro3d::IDynRef::~IDynRef( void )
{
	*m_self_ref.GetShared() = nullptr;
}
