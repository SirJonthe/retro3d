#ifndef TINY_TEXTURE_H
#define TINY_TEXTURE_H

#include "tiny_system.h"
#include "tiny_structs.h"

namespace tiny3d
{

	// @data Texture
	// @info Contains compressed color data (average 3 bits per element) in a spatial format that is especially suited for pseudo-linear access in non-axis aligned access patterns.
class Texture
{
private:
	struct CCCBlock
	{
		tiny3d::UHInt color_idx; // bit field
		tiny3d::UHInt colors[2];
	};

	struct Index
	{
		tiny3d::UInt block;
		tiny3d::UInt bit;
	};

private:
	CCCBlock                 *m_texels; // compressed
	tiny3d::UInt              m_dimension;
	tiny3d::UInt              m_dim_mask;
	tiny3d::UInt              m_dim_shift;
	tiny3d::Real              m_fix_dim;
	tiny3d::UInt              m_blocks;
	tiny3d::UInt              m_block_mask;
	tiny3d::UInt              m_block_shift;
	tiny3d::Real              m_fix_blocks;
	tiny3d::Color::BlendMode  m_blend_modes[2];

private:
	bool            SetDimension(tiny3d::UInt dimension);
	tiny3d::UInt    GetMortonIndex(tiny3d::UPoint p) const;
	tiny3d::UPoint  GetXY(tiny3d::Vector2 uv) const;
	tiny3d::UPoint  GetXY(tiny3d::UPoint p) const;
	Index           GetIndex(tiny3d::Vector2 uv) const;
	Index           GetIndex(tiny3d::UPoint p) const;
	tiny3d::UHInt   EncodeTexel(tiny3d::Color color) const;
	tiny3d::Color   DecodeTexel(tiny3d::UHInt texel) const;
	tiny3d::Color   GetColor(tiny3d::Texture::Index i) const;

public:
	 Texture( void );
	 explicit Texture(tiny3d::UInt dimension);
	 explicit Texture(const tiny3d::Image &img);
	 Texture(const tiny3d::Texture &t);
	~Texture( void );

	// @algo Create
	// @info Creates a new texture surface with the speficied dimensions.
	// @note Maximum dimensions are defined by MaxDImension and minimum dimensions are specified in MinDimensions. Only supports width equal to height, and a power of 2 dimension.
	// @in dimensions -> The unsigned dimension of the new texture surface.
	// @out TRUE on success.
	bool Create(tiny3d::UInt dimension);
	
	// @algo Destroy
	// @info Releases the texture resources.
	void Destroy( void );
	
	// @algo Copy
	// @info Copies an texture surface.
	// @in t -> The surface to copy.
	void Copy(const tiny3d::Texture &t);
	
	// @algo ToImage
	// @info Copies contents of texture to a newly created image.
	// @inout image -> The resulting image.
	// @out TRUE on success (this should always return TRUE, unless Image specifications change).
	bool ToImage(tiny3d::Image &image) const;
	
	// @algo FromImage
	// @info Converts an image to a texture. Has same constraints as Create.
	// @note This is a lossy compression algorithm.
	// @in image -> The image to convert.
	// @out TRUE on success.
	bool FromImage(const tiny3d::Image &image);

	// @algo GetWidth
	// @out The width in pixels of the image.
	tiny3d::UInt             GetWidth( void ) const;
	
	// @algo GetHeight
	// @out The height in pixels of the image.
	tiny3d::UInt             GetHeight( void ) const;
	
	// @algo GetColor
	// @info Gets a color based on normalized texture coordinates (0-1). Coordinates less than 0 or greater than 1 wraps around to 0-1 range.
	// @in uv -> The normalized texture coordinates (0-1).
	// @out The color.
	tiny3d::Color            GetColor(tiny3d::Vector2 uv) const;
	
	// @algo GetColor
	// @info Decodes a color at a given coordinate into a 32-bit value.
	// @note Needs to decode a 16-bit color to a 32-bit color. May be slow.
	// @in p -> The coordinate of the color to get.
	// @out The color.
	tiny3d::Color            GetColor(tiny3d::UPoint p) const;
	
	// @algo GetBlendMode1
	// @out The primary blend mode.
	tiny3d::Color::BlendMode GetBlendMode1( void ) const;
	
	// @algo GetBlendMode2
	// @out The secondary blend mode.
	tiny3d::Color::BlendMode GetBlendMode2( void ) const;
	
	// @algo SetBlendMode1
	// @in blend_mode -> The new primary blend mode.
	void                     SetBlendMode1(tiny3d::Color::BlendMode blend_mode);
	
	// @algo SetBlendMode2
	// @in blend_mode -> The new secondary blend mode.
	void                     SetBlendMode2(tiny3d::Color::BlendMode blend_mode);
	
	// @algo ProjectUV
	// @info Projects normalized UV texture coordinates (0-1) to absolute texture coordinates.
	// @in uv -> The normalized UV texture coordinates (0-1). Coordinates less than 0 or greater than 1 wraps around to 0-1 range.
	// @out The projected absolute texture coordinates.
	tiny3d::Vector2          ProjectUV(tiny3d::Vector2 uv) const;

	// @algo =
	// @info Copies a texture.
	// @in i -> The texture to be copied.
	// @out The texture (self).
	tiny3d::Texture &operator=(const tiny3d::Texture &r);

	// @algo MinDimension
	// @out The minimum supported image size in one dimension.
	static constexpr tiny3d::UInt MinDimension( void ) { return 0x4; }
	
	// @algo MaxDimension
	// @out The maximum supported image size in one dimension.
	static constexpr tiny3d::UInt MaxDimension( void ) { return 0x100; }
};

}

#endif // TINY_TEXTURE_H
