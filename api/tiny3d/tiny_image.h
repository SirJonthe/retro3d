#ifndef TINY_IMAGE_H
#define TINY_IMAGE_H

#include "tiny_system.h"
#include "tiny_math.h"
#include "tiny_structs.h"

namespace tiny3d
{

// @data Image
// @info Contains pixel information. 16 bits per pixel (5 bits per RGB channel + 1 bit for stencil).
class Image
{
private:
	tiny3d::UHInt *m_pixels;
	tiny3d::UInt   m_width;
	tiny3d::UInt   m_height;

private:
	tiny3d::UHInt EncodePixel(Color color) const;
	tiny3d::Color DecodePixel(UHInt pixel) const;

public:
	 Image( void );
	 explicit Image(tiny3d::UInt dimension);
	 Image(tiny3d::UInt width, tiny3d::UInt height);
	 Image(const tiny3d::Image &i);
	~Image( void );

	// @algo Create
	// @info Creates a new image surface with the speficied dimensions.
	// @note Maximum dimensions are defined by MaxDImension.
	// @in width, height -> The unsigned dimension of the new image surface.
	// @out TRUE on success.
	bool Create(tiny3d::UInt width, tiny3d::UInt height);
	
	// @algo Create
	// @info Creates a new image surface with the speficied dimensions.
	// @note Maximum dimensions are defined by MaxDImension.
	// @in dimensions -> The unsigned dimension of the new image surface.
	// @out TRUE on success.
	bool Create(tiny3d::UInt dimensions);
	
	// @algo Destroy
	// @info Releases the image resources.
	void Destroy( void );
	
	// @algo Copy
	// @info Copies an image surface.
	// @in i -> The surface to copy.
	void Copy(const tiny3d::Image &i);
	
	// @algo Fill
	// @info Fills a rectangle with a given color.
	// @in
	//   rect -> The rectangle to fill.
	//   color -> The color to use.
	void Fill(tiny3d::URect rect, tiny3d::Color color);
	
	// @algo Fill
	// @info Fills the entire image with a given color.
	// @in color -> The color to use.
	void Fill(tiny3d::Color color);

	// @algo ClearStencil
	// @info Sets the stencil bit in the specified rectangle.
	// @in
	//   rect -> The rectangle to clear the stencil in.
	//   stencil -> The state of the stencil to set.
	void                     ClearStencil(tiny3d::URect rect, tiny3d::Color::BlendMode stencil = tiny3d::Color::Solid);
	
	// @algo GetStencil
	// @info Retrieves a stencil bit.
	// @in p -> The coordinate of the stencil bit to fetch.
	// @out The requested stencil bit.
	tiny3d::Color::BlendMode GetStencil(tiny3d::UPoint p) const;
	
	// @algo SetStencil
	// @info Sets a stencil bit.
	// @in
	//   p -> The coordinate of the stencil to set.
	//   stencil -> The state to set the stencil.
	void                     SetStencil(tiny3d::UPoint p, tiny3d::Color::BlendMode stencil);

	// @algo GetWidth
	// @out The width in pixels of the image.
	tiny3d::UInt  GetWidth( void )  const;
	
	// @algo GetHeight
	// @out The height in pixels of the image.
	tiny3d::UInt  GetHeight( void ) const;
	
	// @algo GetColor
	// @info Decodes a color at a given coordinate into a 32-bit value.
	// @note Needs to decode a 16-bit color to a 32-bit color. May be slow.
	// @in p -> The coordinate of the color to get.
	// @out The color.
	tiny3d::Color GetColor(tiny3d::UPoint p) const;
	
	// @algo SetColor
	// @info Sets the color of a pixel at a given coordinate.
	// @note Needs to encode the input 32-bit color to a output 16-bit color. May be slow.
	// @ in
	//   p -> The coordinate of the color to set.
	//   color -> The color to set.
	void          SetColor(tiny3d::UPoint p, tiny3d::Color color);

	// @algo SetColorKey
	// @info Sets the color matching the key to Transparent.
	// @in key -> The color to make transparent.
	void SetColorKey(tiny3d::Color key);
	
	// @algo FlipX
	// @info Mirror the image in the X axis.
	void FlipX( void );
	
	// @algo FlipY
	// @info Mirror the image in the Y axis.
	void FlipY( void );

	// @algo =
	// @info Copies an image.
	// @in i -> The image to be copied.
	// @out The image (self).
	tiny3d::Image &operator=(const tiny3d::Image &i);

	// @algo MaxDimension
	// @out The maximum supported image size in one dimension.
	static constexpr tiny3d::UInt MaxDimension( void ) { return 0x400; }
};

}

#endif // TINY_IMAGE_H
