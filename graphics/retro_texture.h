#ifndef RETRO_TEXTURE_H
#define RETRO_TEXTURE_H

#include "../api/tiny3d/tiny_texture.h"
#include "../api/tiny3d/tiny_image.h"
#include "../common/retro_assets.h"

// 256x256
// 64x64
// 32x32
// 16x16
// 8x8
// 4x4
#define RETRO3D_MIP_COUNT 6

namespace retro3d
{

class Texture : public retro3d::Asset<Texture>
{
private:
	tiny3d::Texture  m_mip_data[RETRO3D_MIP_COUNT];
	tiny3d::Texture *m_mip_refs[RETRO3D_MIP_COUNT]; // TODO: Decouple from Tiny3d (for compatibility with other API:s) - This must instead be an index array

private:
	void Delete( void );
	void CreateErrorTexture( void );
	void BuildRefs( void );

public:
			  Texture( void );
			  Texture(const retro3d::Texture &mips);
	explicit  Texture(const tiny3d::Image &img);

	bool FromImage(const tiny3d::Image &img);
	void Copy(const Texture &mips);
	void Destroy( void );
	void SetBlendMode1(tiny3d::Color::BlendMode blend_mode);
	void SetBlendMode2(tiny3d::Color::BlendMode blend_mode);
	void SetBlendModes(tiny3d::Color::BlendMode mode_1, tiny3d::Color::BlendMode mode_2);
	void ResetBlendMode1( void );
	void ResetBlendMode2( void );
	void ResetBlendModes( void );

	const tiny3d::Texture *operator[](tiny3d::UInt level) const;
	retro3d::Texture      &operator=(const Texture &mips);

	const tiny3d::Texture *GetHighestQuality( void ) const;
	const tiny3d::Texture *GetLowestQuality( void ) const;

	static constexpr uint32_t MaxDimension( void ) { return tiny3d::Texture::MaxDimension(); }
	static constexpr uint32_t MinDimension( void ) { return tiny3d::Texture::MinDimension(); }
};

}

#endif // RETRO_TEXTURE_H
