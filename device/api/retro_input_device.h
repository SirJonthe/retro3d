
#ifndef RETRO_INPUT_DEVICE_H
#define RETRO_INPUT_DEVICE_H

#include "../retro_device.h"

namespace retro3d
{

/// @brief The base class for the abstraction of input method API:s. Engine and game code calls functions defined in this class while an implementation of this class makes the API calls to the API of choise.
class input_device : public device
{};

}

#endif
