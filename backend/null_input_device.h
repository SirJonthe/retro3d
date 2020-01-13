#ifndef RETRO_NULL_INPUT_H
#define RETRO_NULL_INPUT_H

#include "../frontend/retro_input_device.h"

namespace platform
{

class NullInputDevice : public retro3d::InputDevice
{
public:
	bool     Init( void ) override { return true; }

	bool     UserQuit( void ) const override { return false; }
	uint64_t GetRealTimeMS( void ) const override { return 0; }
	uint64_t GetProgramTimeMS( void ) const override { return 0; }
	void     Sleep(uint64_t) const override { return; }
	void     ToggleMouseCursorLock( void ) override { return; }
	bool     IsMouseCursorLocked( void ) const override { return false; }
	void     ToggleMouseCursorVisibility( void ) override { return; }
	bool     IsMouseCursorVisible( void ) const override { return false; }
	void     Update( void ) override { return; }
};

}

#endif // RETRO_NULL_INPUT_H
