#ifndef NULL_VIDEO_DEVICE_H
#define NULL_VIDEO_DEVICE_H

#include "../frontend/retro_video_device.h"

namespace platform
{

class NullVideoDevice : public retro3d::VideoDevice
{
public:
	~NullVideoDevice( void ) {}

	bool Init( void ) override { return true; }
	bool CreateWindow(uint32_t, uint32_t, bool) override { return true; }
	void SetWindowCaption(const std::string&) override {}
	void DestroyWindow( void ) override {}
	virtual void FromImage(const tiny3d::Image&, const retro3d::URect&) override {}
	void Display(const retro3d::Rect&) override {}
	void Display( void ) override {}
	retro3d::Point GetWindowPosition( void ) const override { return retro3d::Point{ 0, 0 }; }
	uint32_t GetWindowWidth( void ) const override { return 0; }
	uint32_t GetWindowHeight( void ) const override { return 0; }
	std::string GetWindowCaption( void ) const override { return ""; }
};

}

#endif // NULL_VIDEO_DEVICE_H
