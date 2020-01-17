#ifndef RETRO_VIDEO_DEVICE_H
#define RETRO_VIDEO_DEVICE_H

#include <string>
#include <cstdint>
#include "../common/retro_geom.h"
#include "retro_device.h"

namespace retro3d
{

// @info The video device only manages the window and retrieves (relatively) platform independent information about it.
class VideoDevice : public retro3d::Device
{
public:
	// @info The constructor should initialize the video subsystem.

	// @info The destructor should close the video subsystem.
	virtual ~VideoDevice( void );

	// @info Create a window with the properties.
	virtual bool CreateWindow(uint32_t width, uint32_t height, bool enable_fullscreen) = 0;

	// @info Sets the window caption.
	virtual void SetWindowCaption(const std::string &caption) = 0;

	// @info Destroy the window.
	virtual void DestroyWindow( void ) = 0;

	// @info Transfer pixel data (from one format) to the internal representation of the video device. Stretches based on the relationship between the source rectangle and destination rectangle.
	virtual void FromImage(void *src_pixels, const retro3d::URect &src_rect, const retro3d::URect &dst_rect) = 0;

	// @info Ultimately displays the viewable video surface with the results from
	virtual void Display(const retro3d::URect &rect) = 0;

	// @info Get window properties.
	virtual retro3d::Point GetWindowPosition( void ) const = 0;
	virtual uint32_t GetWindowWidth( void ) const = 0;
	virtual uint32_t GetWindowHeight( void ) const = 0;
	virtual std::string GetWindowCaption( void ) const = 0;
};

}

#endif // RETRO_VIDEO_DEVICE_H
