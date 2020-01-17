#include "sdl_video_device.h"

#ifdef RETRO3D_USE_SDL1
	#include <SDL/SDL.h>
#elif defined(RETRO3D_USE_SDL2)
	#include <SDL2/SDL.h>
#endif

#ifdef RETRO3D_USE_SDL2
	static SDL_Renderer *Renderer = nullptr;
	static SDL_Window   *Window   = nullptr;
	static SDL_Surface  *Surface  = nullptr;
#endif

platform::SDLVideoDevice::SDLVideoDevice( void )
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		std::cout << "[SDLVideoDevice::ctor] " << SDL_GetError() << std::endl;
	}
}

platform::SDLVideoDevice::~SDLVideoDevice( void )
{
#ifdef RETRO3D_USE_SDL1
	SDL_FreeSurface(SDL_GetVideoSurface());
#elif defined(RETRO3D_USE_SDL2)
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
#endif
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool platform::SDLVideoDevice::CreateWindow(uint32_t width, uint32_t height, bool enable_fullscreen)
{
	DestroyWindow();

#ifdef RETRO3D_USE_SDL1
	SDL_SetVideoMode(width, height, 24, SDL_SWSURFACE|(enable_fullscreen ? SDL_FULLSCREEN : 0));
	if (SDL_GetVideoSurface() == nullptr) {
		std::cout << "[SDLVideoDevice::CreateWindow (SDL1)] " << SDL_GetError() << std::endl;
	}
	return SDL_GetVideoSurface() != nullptr;
#elif defined(RETRO3D_USE_SDL2)
	Window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_SWSURFACE|(enable_fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
	if (Window == nullptr) {
		std::cout << "[SDLVideoDevice::CreateWindow (SDL2)] " << SDL_GetError() << std::endl;
		return false;
	}
	Surface = SDL_GetWindowSurface(Window);
	if (Surface == nullptr) {
		SDL_DestroyWindow(Window);
		Window = nullptr;
		std::cout << "[SDLVideoDevice::CreateWindow (SDL2)] " << SDL_GetError() << std::endl;
		return false;
	}
	Renderer = SDL_CreateSoftwareRenderer(Surface);
	if (Renderer == nullptr) {
		SDL_DestroyWindow(Window);
		Window = nullptr;
		std::cout << "[SDLVideoDevice::CreateWindow (SDL2)] " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
#endif
}

void platform::SDLVideoDevice::SetWindowCaption(const std::string &caption)
{
#ifdef RETRO3D_USE_SDL1
	SDL_WM_SetCaption(caption.c_str(), nullptr);
#elif defined(RETRO3D_USE_SDL2)
	SDL_SetWindowTitle(Window, caption.c_str());
#endif
}

void platform::SDLVideoDevice::DestroyWindow( void )
{
#ifdef RETRO3D_USE_SDL1
	SDL_FreeSurface(SDL_GetVideoSurface());
#elif defined(RETRO3D_USE_SDL2)
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	Renderer = nullptr;
	Window = nullptr;
	Surface = nullptr;
#endif
}

void platform::SDLVideoDevice::FromImage(void *src_pixels, const retro3d::URect &src_rect, const retro3d::URect &dst_rect)
{
	// TODO: From a surface that the renderer used to the surface the video uses
}

retro3d::Point platform::SDLVideoDevice::GetWindowPosition( void ) const
{
	retro3d::Point p = { 0, 0 };
#ifdef RETRO3D_USE_SDL1
	// NOTE: There is no way to get window position in SDL1.2, so return 0,0.
#elif defined(RETRO3D_USE_SDL2)
	SDL_GetWindowPosition(Window, &p.x, &p.y);
#endif
	return p;
}

void platform::SDLVideoDevice::Display(const retro3d::URect &rect)
{
	SDL_Rect r;
	r.x = rect.a.x;
	r.y = rect.a.y;
	r.w = rect.b.x - rect.a.x;
	r.h = rect.b.y - rect.a.y;

#ifdef RETRO3D_USE_SDL1
	SDL_UpdateRect(SDL_GetVideoSurface(), r.x, r.y, r.w, r.h);
#elif defined(RETRO3D_USE_SDL2)
	SDL_UpdateWindowSurfaceRects(Window, &r, 1);
#endif
}

uint32_t platform::SDLVideoDevice::GetWindowWidth( void ) const
{
#ifdef RETRO3D_USE_SDL1
	return uint32_t(SDL_GetVideoSurface()->w);
#elif defined(RETRO3D_USE_SDL2)
	return uint32_t(Surface->w);
#endif
}

uint32_t platform::SDLVideoDevice::GetWindowHeight( void ) const
{
#ifdef RETRO3D_USE_SDL1
	return uint32_t(SDL_GetVideoSurface()->h);
#elif defined(RETRO3D_USE_SDL2)
	return uint32_t(Surface->h);
#endif
}

std::string platform::SDLVideoDevice::GetWindowCaption( void ) const
{
#ifdef RETRO3D_USE_SDL1
	char *caption;
	SDL_WM_GetCaption(&caption, nullptr);
	return std::string(caption);
#elif defined(RETRO3D_USE_SDL2)
	return std::string(SDL_GetWindowTitle(Window));
#endif
}
