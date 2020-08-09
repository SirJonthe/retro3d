#include <chrono>
#include "sdl_input_device.h"
#include "../retro3d.h"

#ifdef RETRO3D_USE_SDL1
	#include <SDL/SDL.h>
	#define SDL_STR "SDL1"
#elif defined(RETRO3D_USE_SDL2)
	#include <SDL2/SDL.h>
	#define SDL_STR "SDL2"
#endif

#ifdef RETRO3D_USE_SDL1
SDLKey ToSDLKeycode(uint64_t code)
{
	switch (code) {
	case retro3d::Keyboard::A: return SDLK_a;
	case retro3d::Keyboard::B: return SDLK_b;
	case retro3d::Keyboard::C: return SDLK_c;
	case retro3d::Keyboard::D: return SDLK_d;
	case retro3d::Keyboard::E: return SDLK_e;
	case retro3d::Keyboard::F: return SDLK_f;
	case retro3d::Keyboard::G: return SDLK_g;
	case retro3d::Keyboard::H: return SDLK_h;
	case retro3d::Keyboard::I: return SDLK_i;
	case retro3d::Keyboard::J: return SDLK_j;
	case retro3d::Keyboard::K: return SDLK_k;
	case retro3d::Keyboard::L: return SDLK_l;
	case retro3d::Keyboard::M: return SDLK_m;
	case retro3d::Keyboard::N: return SDLK_n;
	case retro3d::Keyboard::O: return SDLK_o;
	case retro3d::Keyboard::P: return SDLK_p;
	case retro3d::Keyboard::Q: return SDLK_q;
	case retro3d::Keyboard::R: return SDLK_r;
	case retro3d::Keyboard::S: return SDLK_s;
	case retro3d::Keyboard::T: return SDLK_t;
	case retro3d::Keyboard::U: return SDLK_u;
	case retro3d::Keyboard::V: return SDLK_v;
	case retro3d::Keyboard::W: return SDLK_w;
	case retro3d::Keyboard::X: return SDLK_x;
	case retro3d::Keyboard::Y: return SDLK_y;
	case retro3d::Keyboard::Z: return SDLK_z;

	case retro3d::Keyboard::KP0: return SDLK_0;
	case retro3d::Keyboard::KP1: return SDLK_1;
	case retro3d::Keyboard::KP2: return SDLK_2;
	case retro3d::Keyboard::KP3: return SDLK_3;
	case retro3d::Keyboard::KP4: return SDLK_4;
	case retro3d::Keyboard::KP5: return SDLK_5;
	case retro3d::Keyboard::KP6: return SDLK_6;
	case retro3d::Keyboard::KP7: return SDLK_7;
	case retro3d::Keyboard::KP8: return SDLK_8;
	case retro3d::Keyboard::KP9: return SDLK_9;

	case retro3d::Keyboard::NP0: return SDLK_KP0;
	case retro3d::Keyboard::NP1: return SDLK_KP1;
	case retro3d::Keyboard::NP2: return SDLK_KP2;
	case retro3d::Keyboard::NP3: return SDLK_KP3;
	case retro3d::Keyboard::NP4: return SDLK_KP4;
	case retro3d::Keyboard::NP5: return SDLK_KP5;
	case retro3d::Keyboard::NP6: return SDLK_KP6;
	case retro3d::Keyboard::NP7: return SDLK_KP7;
	case retro3d::Keyboard::NP8: return SDLK_KP8;
	case retro3d::Keyboard::NP9: return SDLK_KP9;

	case retro3d::Keyboard::NPPlus: return SDLK_KP_PLUS;
	case retro3d::Keyboard::NPMinus: return SDLK_KP_MINUS;
	case retro3d::Keyboard::NPMul: return SDLK_KP_MULTIPLY;
	case retro3d::Keyboard::NPDiv: return SDLK_KP_DIVIDE;
	case retro3d::Keyboard::NPEnter: return SDLK_KP_ENTER;

	case retro3d::Keyboard::F1: return SDLK_F1;
	case retro3d::Keyboard::F2: return SDLK_F2;
	case retro3d::Keyboard::F3: return SDLK_F3;
	case retro3d::Keyboard::F4: return SDLK_F4;
	case retro3d::Keyboard::F5: return SDLK_F5;
	case retro3d::Keyboard::F6: return SDLK_F6;
	case retro3d::Keyboard::F7: return SDLK_F7;
	case retro3d::Keyboard::F8: return SDLK_F8;
	case retro3d::Keyboard::F9: return SDLK_F9;
	case retro3d::Keyboard::F10: return SDLK_F10;
	case retro3d::Keyboard::F11: return SDLK_F11;
	case retro3d::Keyboard::F12: return SDLK_F12;

	case retro3d::Keyboard::Space: return SDLK_SPACE;
	case retro3d::Keyboard::Enter: return SDLK_RETURN;
	case retro3d::Keyboard::Escape: return SDLK_ESCAPE;
	case retro3d::Keyboard::Backspace: return SDLK_BACKSPACE;
	case retro3d::Keyboard::RShift: return SDLK_RSHIFT;
	case retro3d::Keyboard::LShift: return SDLK_LSHIFT;
	case retro3d::Keyboard::RControl: return SDLK_RCTRL;
	case retro3d::Keyboard::LControl: return SDLK_LCTRL;
	case retro3d::Keyboard::RAlt: return SDLK_RALT;
	case retro3d::Keyboard::LAlt: return SDLK_LALT;

	case retro3d::Keyboard::Up: return SDLK_UP;
	case retro3d::Keyboard::Down: return SDLK_DOWN;
	case retro3d::Keyboard::Left: return SDLK_LEFT;
	case retro3d::Keyboard::Right: return SDLK_RIGHT;

	case retro3d::Keyboard::Tab: return SDLK_TAB;

	default: break;
	}
	return SDLK_UNKNOWN;
}
#elif defined(RETRO3D_USE_SDL2)
SDL_Scancode ToSDLKeycode(uint64_t code)
{
	switch (code) {
	case retro3d::Keyboard::A: return SDL_SCANCODE_A;
	case retro3d::Keyboard::B: return SDL_SCANCODE_B;
	case retro3d::Keyboard::C: return SDL_SCANCODE_C;
	case retro3d::Keyboard::D: return SDL_SCANCODE_D;
	case retro3d::Keyboard::E: return SDL_SCANCODE_E;
	case retro3d::Keyboard::F: return SDL_SCANCODE_F;
	case retro3d::Keyboard::G: return SDL_SCANCODE_G;
	case retro3d::Keyboard::H: return SDL_SCANCODE_H;
	case retro3d::Keyboard::I: return SDL_SCANCODE_I;
	case retro3d::Keyboard::J: return SDL_SCANCODE_J;
	case retro3d::Keyboard::K: return SDL_SCANCODE_K;
	case retro3d::Keyboard::L: return SDL_SCANCODE_L;
	case retro3d::Keyboard::M: return SDL_SCANCODE_M;
	case retro3d::Keyboard::N: return SDL_SCANCODE_N;
	case retro3d::Keyboard::O: return SDL_SCANCODE_O;
	case retro3d::Keyboard::P: return SDL_SCANCODE_P;
	case retro3d::Keyboard::Q: return SDL_SCANCODE_Q;
	case retro3d::Keyboard::R: return SDL_SCANCODE_R;
	case retro3d::Keyboard::S: return SDL_SCANCODE_S;
	case retro3d::Keyboard::T: return SDL_SCANCODE_T;
	case retro3d::Keyboard::U: return SDL_SCANCODE_U;
	case retro3d::Keyboard::V: return SDL_SCANCODE_V;
	case retro3d::Keyboard::W: return SDL_SCANCODE_W;
	case retro3d::Keyboard::X: return SDL_SCANCODE_X;
	case retro3d::Keyboard::Y: return SDL_SCANCODE_Y;
	case retro3d::Keyboard::Z: return SDL_SCANCODE_Z;

	case retro3d::Keyboard::KP0: return SDL_SCANCODE_0;
	case retro3d::Keyboard::KP1: return SDL_SCANCODE_1;
	case retro3d::Keyboard::KP2: return SDL_SCANCODE_2;
	case retro3d::Keyboard::KP3: return SDL_SCANCODE_3;
	case retro3d::Keyboard::KP4: return SDL_SCANCODE_4;
	case retro3d::Keyboard::KP5: return SDL_SCANCODE_5;
	case retro3d::Keyboard::KP6: return SDL_SCANCODE_6;
	case retro3d::Keyboard::KP7: return SDL_SCANCODE_7;
	case retro3d::Keyboard::KP8: return SDL_SCANCODE_8;
	case retro3d::Keyboard::KP9: return SDL_SCANCODE_9;

	case retro3d::Keyboard::NP0: return SDL_SCANCODE_KP_0;
	case retro3d::Keyboard::NP1: return SDL_SCANCODE_KP_1;
	case retro3d::Keyboard::NP2: return SDL_SCANCODE_KP_2;
	case retro3d::Keyboard::NP3: return SDL_SCANCODE_KP_3;
	case retro3d::Keyboard::NP4: return SDL_SCANCODE_KP_4;
	case retro3d::Keyboard::NP5: return SDL_SCANCODE_KP_5;
	case retro3d::Keyboard::NP6: return SDL_SCANCODE_KP_6;
	case retro3d::Keyboard::NP7: return SDL_SCANCODE_KP_7;
	case retro3d::Keyboard::NP8: return SDL_SCANCODE_KP_8;
	case retro3d::Keyboard::NP9: return SDL_SCANCODE_KP_9;

	case retro3d::Keyboard::NPPlus: return SDL_SCANCODE_KP_PLUS;
	case retro3d::Keyboard::NPMinus: return SDL_SCANCODE_KP_MINUS;
	case retro3d::Keyboard::NPMul: return SDL_SCANCODE_KP_MULTIPLY;
	case retro3d::Keyboard::NPDiv: return SDL_SCANCODE_KP_DIVIDE;
	case retro3d::Keyboard::NPEnter: return SDL_SCANCODE_KP_ENTER;

	case retro3d::Keyboard::F1: return SDL_SCANCODE_F1;
	case retro3d::Keyboard::F2: return SDL_SCANCODE_F2;
	case retro3d::Keyboard::F3: return SDL_SCANCODE_F3;
	case retro3d::Keyboard::F4: return SDL_SCANCODE_F4;
	case retro3d::Keyboard::F5: return SDL_SCANCODE_F5;
	case retro3d::Keyboard::F6: return SDL_SCANCODE_F6;
	case retro3d::Keyboard::F7: return SDL_SCANCODE_F7;
	case retro3d::Keyboard::F8: return SDL_SCANCODE_F8;
	case retro3d::Keyboard::F9: return SDL_SCANCODE_F9;
	case retro3d::Keyboard::F10: return SDL_SCANCODE_F10;
	case retro3d::Keyboard::F11: return SDL_SCANCODE_F11;
	case retro3d::Keyboard::F12: return SDL_SCANCODE_F12;

	case retro3d::Keyboard::Space: return SDL_SCANCODE_SPACE;
	case retro3d::Keyboard::Enter: return SDL_SCANCODE_RETURN;
	case retro3d::Keyboard::Escape: return SDL_SCANCODE_ESCAPE;
	case retro3d::Keyboard::Backspace: return SDL_SCANCODE_BACKSPACE;
	case retro3d::Keyboard::RShift: return SDL_SCANCODE_RSHIFT;
	case retro3d::Keyboard::LShift: return SDL_SCANCODE_LSHIFT;
	case retro3d::Keyboard::RControl: return SDL_SCANCODE_RCTRL;
	case retro3d::Keyboard::LControl: return SDL_SCANCODE_LCTRL;
	case retro3d::Keyboard::RAlt: return SDL_SCANCODE_RALT;
	case retro3d::Keyboard::LAlt: return SDL_SCANCODE_LALT;

	case retro3d::Keyboard::Up: return SDL_SCANCODE_UP;
	case retro3d::Keyboard::Down: return SDL_SCANCODE_DOWN;
	case retro3d::Keyboard::Left: return SDL_SCANCODE_LEFT;
	case retro3d::Keyboard::Right: return SDL_SCANCODE_RIGHT;

	case retro3d::Keyboard::Tab: return SDL_SCANCODE_TAB;

	default: break;
	}
	return SDL_SCANCODE_UNKNOWN;
}
#endif

void platform::SDLInputDevice::UpdateState(retro3d::InputDevice::Input &internal_state, float api_state, double delta_time)
{
	float prev_state = internal_state.activation;
	internal_state.activation = api_state;
	internal_state.delta      = internal_state.activation - prev_state;
	internal_state.time       = ((prev_state != 0.0f) == (internal_state.activation != 0.0f)) ? internal_state.time + delta_time : 0.0;
	internal_state.awoken     = (prev_state == 0.0f && internal_state.activation != 0.0f);
}

platform::SDLInputDevice::SDLInputDevice( void ) : retro3d::InputDevice(), m_last_update(0), m_mouse_correction_x(0), m_mouse_correction_y(0), m_mouse_locked(false), m_mouse_visible(true), m_user_quit(false)
{
	CreateStateTable(retro3d::INPUT_COUNT + 1);
}

platform::SDLInputDevice::~SDLInputDevice( void )
{
	std::cout << "[SDLInputDevice::dtor (" << SDL_STR << ")] Quitting subsystem SDL_JOYSTICK" << std::endl;
	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

bool platform::SDLInputDevice::Init( void )
{
	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0) {
		std::cout << "[SDLInputDevice::Init (" << SDL_STR << ")] " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

bool platform::SDLInputDevice::UserQuit( void ) const
{
	return m_user_quit;
}

uint64_t platform::SDLInputDevice::GetRealTimeMS( void ) const
{
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	return uint64_t(ms.count());
}

uint64_t platform::SDLInputDevice::GetProgramTimeMS( void ) const
{
	return uint64_t(SDL_GetTicks());
}

void platform::SDLInputDevice::Sleep(retro3d::Time time) const
{
	SDL_Delay(Uint32(time.GetTotalMS()));
}

void platform::SDLInputDevice::ToggleMouseCursorLock( void )
{
	m_mouse_locked = !m_mouse_locked;
}

bool platform::SDLInputDevice::IsMouseCursorLocked( void ) const
{
	return m_mouse_locked;
}

void platform::SDLInputDevice::ToggleMouseCursorVisibility( void )
{
	m_mouse_visible = !m_mouse_visible;
	SDL_ShowCursor(m_mouse_visible ? 1 : 0);
}

bool platform::SDLInputDevice::IsMouseCursorVisible( void ) const
{
	return m_mouse_visible;
}

void platform::SDLInputDevice::Update( void )
{
	uint64_t time = GetProgramTimeMS();
	uint64_t time_diff = time - m_last_update;
	double delta_time = double(time_diff) / 1000.0;

	SDL_PumpEvents();
#ifdef RETRO3D_USE_SDL1
	Uint8 *keys = SDL_GetKeyState(nullptr);
#elif defined(RETRO3D_USE_SDL2)
	const Uint8 *keys = SDL_GetKeyboardState(nullptr);
#endif

	for (uint64_t i = retro3d::Keyboard::FIRST; i < retro3d::Keyboard::LAST; ++i) {
		UpdateState(m_state[i], float(keys[ToSDLKeycode(uint64_t(i))]), delta_time);
	}

	int x, y;
	Uint8 mouse_state = SDL_GetMouseState(&x, &y);

	if (m_mouse_locked == true) {
		const Uint16 w2 = Uint16(GetEngine()->GetVideo()->GetWindowWidth() / 2);
		const Uint16 h2 = Uint16(GetEngine()->GetVideo()->GetWindowHeight() / 2);
#ifdef RETRO3D_USE_SDL1
		SDL_WarpMouse(w2, h2);
#elif defined(RETRO3D_USE_SDL2)
		retro3d::Point window_position = GetEngine()->GetVideo()->GetWindowPosition();
		SDL_WarpMouseGlobal(window_position.x + int32_t(w2), window_position.y + int32_t(h2));
#endif
		m_state[retro3d::Mouse::MoveX].activation -= float(w2 - x);
		m_state[retro3d::Mouse::MoveY].activation -= float(h2 - y);
	}

	UpdateState(m_state[retro3d::Mouse::Right], (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) > 0 ? 1.0f : 0.0f, delta_time);
	UpdateState(m_state[retro3d::Mouse::Mid], (mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) > 0 ? 1.0f : 0.0f, delta_time);
	UpdateState(m_state[retro3d::Mouse::Left], (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) > 0 ? 1.0f : 0.0f, delta_time);
	UpdateState(m_state[retro3d::Mouse::MoveX], float(x), delta_time);
	UpdateState(m_state[retro3d::Mouse::MoveY], float(y), delta_time);

	if (m_mouse_locked == true) {
		m_state[retro3d::Mouse::MoveX].delta = -m_state[retro3d::Mouse::MoveX].delta;
		m_state[retro3d::Mouse::MoveY].delta = -m_state[retro3d::Mouse::MoveY].delta;
	}

	m_last_update = time;
}
