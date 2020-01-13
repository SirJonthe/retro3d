/*#include <SDL/SDL.h>

#include "tiny_system.h"
#include "tiny_image.h"

#if defined(__linux) || defined(__linux__) || defined (__LINUX__) || defined (__gnu_linux__)
	#define XLIB_THREADS 1
#endif

#ifdef XLIB_THREADS
	#include "X11/Xlib.h"
#endif

using namespace tiny3d;

bool tiny3d::System::Init(tiny3d::UInt width, tiny3d::UInt height, const std::string &caption)
{
#ifdef XLIB_THREADS
	if (XInitThreads() != True) { return false; }
#endif
	if (SDL_Init(SDL_INIT_VIDEO) == -1) { return false; }
	if (SDL_SetVideoMode(int(width), int(height), 24, SDL_SWSURFACE) == nullptr) { return false; }
	SDL_WM_SetCaption(caption.c_str(), nullptr);
	return true;
}

void tiny3d::System::Close( void )
{
	SDL_FreeSurface(SDL_GetVideoSurface());
	SDL_Quit();
}

tiny3d::Real tiny3d::System::Time()
{
	Uint32 ticks = SDL_GetTicks();
	Real time = Real(float(ticks) / 1000.0f);
	return time;
}

void tiny3d::System::Video::Blit(const Image &src, const tiny3d::URect *dst_rect)
{
	if (SDL_GetVideoSurface() == nullptr) { return; }

	const UXInt x_start    = dst_rect == nullptr ? 0 : dst_rect->a.x;
	const UXInt y_start    = dst_rect == nullptr ? 0 : dst_rect->a.y;
	const UXInt x_end      = dst_rect == nullptr ? UXInt(System::Video::Width()) : dst_rect->b.x;
	const UXInt y_end      = dst_rect == nullptr ? UXInt(System::Video::Height()) : dst_rect->b.y;
	const UXInt vid_width  = UXInt(System::Video::Width());
	const UXInt vid_height = UXInt(System::Video::Height());
	const UXInt x_frac     = (UXInt(src.GetWidth()) << 16) / vid_width;
	const UXInt y_frac     = (UXInt(src.GetHeight()) << 16) / vid_height;

	Byte *pixel_row = reinterpret_cast<Byte*>(SDL_GetVideoSurface()->pixels) + x_start * SDL_GetVideoSurface()->format->BytesPerPixel + y_start * SDL_GetVideoSurface()->pitch;
	for (UXInt y = y_start; y < y_end; ++y) {
		unsigned char *pixels = pixel_row;
		for (UXInt x = x_start; x < x_end; ++x) {
			Color c = src.GetColor({UInt((x * x_frac) >> 16), UInt((y * y_frac) >> 16)});
			pixels[0] = c.b;
			pixels[1] = c.g;
			pixels[2] = c.r;
			pixels += SDL_GetVideoSurface()->format->BytesPerPixel;
		}
		pixel_row += SDL_GetVideoSurface()->pitch;
	}
}

void tiny3d::System::Video::Update(const tiny3d::URect *dst_rect)
{
	if (dst_rect == nullptr) {
		SDL_Flip(SDL_GetVideoSurface());
	} else {
		SDL_UpdateRect(SDL_GetVideoSurface(), Sint16(dst_rect->a.x), Sint16(dst_rect->a.y), Uint16(dst_rect->b.x - dst_rect->a.x), Uint16(dst_rect->b.y - dst_rect->a.y));
	}
}

tiny3d::UInt tiny3d::System::Video::Width( void )
{
	return tiny3d::UInt(SDL_GetVideoSurface()->w);
}

tiny3d::UInt tiny3d::System::Video::Height( void )
{
	return tiny3d::UInt(SDL_GetVideoSurface()->h);
}*/
