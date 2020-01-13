#ifndef RETRO_SDL_INPUT_H
#define RETRO_SDL_INPUT_H

#include "../frontend/retro_input_device.h"

namespace platform
{

class SDLInputDevice : public retro3d::InputDevice
{
private:
	uint64_t m_last_update;
	int32_t  m_mouse_correction_x, m_mouse_correction_y;
	bool     m_mouse_locked;
	bool     m_mouse_visible;
	bool     m_user_quit;

private:
	void UpdateState(retro3d::InputDevice::Input &internal_state, float api_state, double delta_time);

public:
	SDLInputDevice( void );
	~SDLInputDevice( void ) override;

	bool     Init( void ) override;

	bool     UserQuit( void ) const override;
	uint64_t GetRealTimeMS( void ) const override;
	uint64_t GetProgramTimeMS( void ) const override;
	void     Sleep(uint64_t time_ms) const override;
	void     ToggleMouseCursorLock( void ) override;
	bool     IsMouseCursorLocked( void ) const override;
	void     ToggleMouseCursorVisibility( void ) override;
	bool     IsMouseCursorVisible( void ) const override;
	void     Update( void ) override;
};

}

#endif // RETRO_SDL_INPUT_H
