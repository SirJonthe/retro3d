#ifndef RETRO_DEVICE_H
#define RETRO_DEVICE_H

#include "../retro3d.h"

namespace retro3d
{

/// @brief The base device node on which all devices are based. Devices represent an abstraction of some API for input and/or output such as graphics, sound, and e.g. keyboards, mice, controllers.
class device : public retro3d::core::node
{};

}

#endif