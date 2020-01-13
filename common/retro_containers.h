#ifndef RETRO_CONTAINERS_H
#define RETRO_CONTAINERS_H

#include "../common/MiniLib/MTL/mtlArray.h"
#include "retro_assets.h"

namespace retro3d
{

template < typename type_t >
class Array : public retro3d::Asset< retro3d::Array<type_t> >, public mtlArray<type_t>
{};

}

#endif // RETRO_CONTAINERS_H
