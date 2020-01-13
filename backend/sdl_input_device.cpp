#include <chrono>
#include <SDL/SDL.h>
#include "sdl_input_device.h"

uint64_t ToR3DKeycode(SDLKey sdlk)
{
	switch (sdlk) {
	case SDLK_a: return retro3d::Keyboard::A;
	case SDLK_b: return retro3d::Keyboard::B;
	case SDLK_c: return retro3d::Keyboard::C;
	case SDLK_d: return retro3d::Keyboard::D;
	case SDLK_e: return retro3d::Keyboard::E;
	case SDLK_f: return retro3d::Keyboard::F;
	case SDLK_g: return retro3d::Keyboard::G;
	case SDLK_h: return retro3d::Keyboard::H;
	case SDLK_i: return retro3d::Keyboard::I;
	case SDLK_j: return retro3d::Keyboard::J;
	case SDLK_k: return retro3d::Keyboard::K;
	case SDLK_l: return retro3d::Keyboard::L;
	case SDLK_m: return retro3d::Keyboard::M;
	case SDLK_n: return retro3d::Keyboard::N;
	case SDLK_o: return retro3d::Keyboard::O;
	case SDLK_p: return retro3d::Keyboard::P;
	case SDLK_q: return retro3d::Keyboard::Q;
	case SDLK_r: return retro3d::Keyboard::R;
	case SDLK_s: return retro3d::Keyboard::S;
	case SDLK_t: return retro3d::Keyboard::T;
	case SDLK_u: return retro3d::Keyboard::U;
	case SDLK_v: return retro3d::Keyboard::V;
	case SDLK_w: return retro3d::Keyboard::W;
	case SDLK_x: return retro3d::Keyboard::X;
	case SDLK_y: return retro3d::Keyboard::Y;
	case SDLK_z: return retro3d::Keyboard::Z;

	case SDLK_0: return retro3d::Keyboard::KP0;
	case SDLK_1: return retro3d::Keyboard::KP1;
	case SDLK_2: return retro3d::Keyboard::KP2;
	case SDLK_3: return retro3d::Keyboard::KP3;
	case SDLK_4: return retro3d::Keyboard::KP4;
	case SDLK_5: return retro3d::Keyboard::KP5;
	case SDLK_6: return retro3d::Keyboard::KP6;
	case SDLK_7: return retro3d::Keyboard::KP7;
	case SDLK_8: return retro3d::Keyboard::KP8;
	case SDLK_9: return retro3d::Keyboard::KP9;

	case SDLK_KP0: return retro3d::Keyboard::NP0;
	case SDLK_KP1: return retro3d::Keyboard::NP1;
	case SDLK_KP2: return retro3d::Keyboard::NP2;
	case SDLK_KP3: return retro3d::Keyboard::NP3;
	case SDLK_KP4: return retro3d::Keyboard::NP4;
	case SDLK_KP5: return retro3d::Keyboard::NP5;
	case SDLK_KP6: return retro3d::Keyboard::NP6;
	case SDLK_KP7: return retro3d::Keyboard::NP7;
	case SDLK_KP8: return retro3d::Keyboard::NP8;
	case SDLK_KP9: return retro3d::Keyboard::NP9;
	case SDLK_KP_PLUS: return retro3d::Keyboard::NPPlus;
	case SDLK_KP_MINUS: return retro3d::Keyboard::NPMinus;
	case SDLK_KP_MULTIPLY: return retro3d::Keyboard::NPMul;
	case SDLK_KP_DIVIDE: return retro3d::Keyboard::NPDiv;
	case SDLK_KP_ENTER: return retro3d::Keyboard::NPEnter;

	case SDLK_F1: return retro3d::Keyboard::F1;
	case SDLK_F2: return retro3d::Keyboard::F2;
	case SDLK_F3: return retro3d::Keyboard::F3;
	case SDLK_F4: return retro3d::Keyboard::F4;
	case SDLK_F5: return retro3d::Keyboard::F5;
	case SDLK_F6: return retro3d::Keyboard::F6;
	case SDLK_F7: return retro3d::Keyboard::F7;
	case SDLK_F8: return retro3d::Keyboard::F8;
	case SDLK_F9: return retro3d::Keyboard::F9;
	case SDLK_F10: return retro3d::Keyboard::F10;
	case SDLK_F11: return retro3d::Keyboard::F11;
	case SDLK_F12: return retro3d::Keyboard::F12;

	case SDLK_SPACE: return retro3d::Keyboard::Space;
	case SDLK_RETURN: return retro3d::Keyboard::Enter;
	case SDLK_ESCAPE: return retro3d::Keyboard::Escape;
	case SDLK_BACKSPACE: return retro3d::Keyboard::Backspace;
	case SDLK_RSHIFT: return retro3d::Keyboard::RShift;
	case SDLK_LSHIFT: return retro3d::Keyboard::LShift;
	case SDLK_RCTRL: return retro3d::Keyboard::RControl;
	case SDLK_LCTRL: return retro3d::Keyboard::LControl;
	case SDLK_RALT: return retro3d::Keyboard::RAlt;
	case SDLK_LALT: return retro3d::Keyboard::LAlt;

	case SDLK_UP: return retro3d::Keyboard::Up;
	case SDLK_DOWN: return retro3d::Keyboard::Down;
	case SDLK_LEFT: return retro3d::Keyboard::Left;
	case SDLK_RIGHT: return retro3d::Keyboard::Right;

	case SDLK_TAB: return retro3d::Keyboard::Tab;

	default: break;
	}
	return retro3d::INPUT_COUNT;
}

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
	return SDLK_LAST;
}

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
	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

bool platform::SDLInputDevice::Init( void )
{
	return SDL_InitSubSystem(SDL_INIT_JOYSTICK) == 0;
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

void platform::SDLInputDevice::Sleep(uint64_t time_ms) const
{
	SDL_Delay(Uint32(time_ms));
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
	Uint8 *keys = SDL_GetKeyState(nullptr);

	for (uint64_t i = retro3d::Keyboard::FIRST; i < retro3d::Keyboard::LAST; ++i) {
		UpdateState(m_state[i], float(keys[ToSDLKeycode(uint64_t(i))]), delta_time);
	}

	int x, y;
	Uint8 mouse_state = SDL_GetMouseState(&x, &y);

	if (m_mouse_locked == true) {
		const Uint16 w2 = Uint16(SDL_GetVideoSurface()->w / 2);
		const Uint16 h2 = Uint16(SDL_GetVideoSurface()->h / 2);
		SDL_WarpMouse(w2, h2);
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
