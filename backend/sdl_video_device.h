#ifndef SDL_VIDEO_DEVICE_H
#define SDL_VIDEO_DEVICE_H

#include "../frontend/retro_video_device.h"

namespace platform
{

class SDLVideoDevice : public retro3d::VideoDevice
{
public:
	~SDLVideoDevice( void );

	bool Init( void ) override;
	bool CreateWindow(uint32_t width, uint32_t height, bool enable_fullscreen) override;
	void SetWindowCaption(const std::string &caption) override;
	void DestroyWindow( void ) override;
	void FromImage(const tiny3d::Image &src, const retro3d::URect &dst_rect) override;
	void Display(const retro3d::Rect &rect) override;
	void Display( void ) override;
	retro3d::Point GetWindowPosition( void ) const override;
	uint32_t GetWindowWidth( void ) const override;
	uint32_t GetWindowHeight( void ) const override;
	std::string GetWindowCaption( void ) const override;
};

}

#endif // SDL_VIDEO_DEVICE_H
