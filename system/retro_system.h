#ifndef RETRO_SYSTEM_H
#define RETRO_SYSTEM_H

#include "../retro3d.h"

namespace retro3d
{

/// @brief The base system on which all systems are based.
/// @tparam component_t The type of component that the system should respond to. This system gets the opportunity to be notified of its creation and destruction.
template < typename component_t >
class system : public retro3d::core::node
{};

}

#endif
