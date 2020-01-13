#include "../common/MiniLib/MML/mmlMath.h"
#include "../common/MiniLib/MML/mmlVector.h"
#include "retro_texture.h"
#include "../serial/retro_import.h"

void retro3d::Texture::CreateErrorTexture( void )
{
	tiny3d::Image i;
	retro3d::CreateDefaultImage(i);
	FromImage(i);
}

void retro3d::Texture::BuildRefs( void )
{
	tiny3d::Texture *t = &m_mip_data[0];
	for (tiny3d::UInt i = 0; i < RETRO3D_MIP_COUNT; ++i) {
		m_mip_refs[i] = t;
		if (i < RETRO3D_MIP_COUNT - 1 && t[1].GetWidth() > 0) {
			++t;
		}
	}
}

retro3d::Texture::Texture( void )
{
	for (tiny3d::UInt i = 0; i < RETRO3D_MIP_COUNT; ++i) {
		m_mip_refs[i] = nullptr;
	}
}

retro3d::Texture::Texture(const retro3d::Texture &mips) : Texture()
{
	Copy(mips);
}

retro3d::Texture::Texture(const tiny3d::Image &img) : Texture()
{
	FromImage(img);
}

bool retro3d::Texture::FromImage(const tiny3d::Image &img)
{
	Destroy();

	tiny3d::Image a_img = img;
	tiny3d::Image b_img;
	tiny3d::Image *a = &a_img;
	tiny3d::Image *b = &b_img;

	for (tiny3d::UInt i = 0; i < RETRO3D_MIP_COUNT; ++i) {

		if (!m_mip_data[i].FromImage(*a)) {
			CreateErrorTexture();
			return false;
		}

		if (i < RETRO3D_MIP_COUNT - 1) {
			tiny3d::UInt w = a->GetWidth() >> 1;
			tiny3d::UInt h = a->GetHeight() >> 1;
			if (w < tiny3d::Texture::MinDimension() || h < tiny3d::Texture::MinDimension()) {
				break;
			}
			if (b->Create(w, h) == true) {
				for (tiny3d::UInt y = 0; y < h; ++y) {
					for (tiny3d::UInt x = 0; x < w; ++x) {
						tiny3d::Color c00 = a->GetColor({ x*2, y*2 });
						tiny3d::Color c10 = a->GetColor({ x*2 + 1, y*2 });
						tiny3d::Color c01 = a->GetColor({ x*2, y*2 + 1 });
						tiny3d::Color c11 = a->GetColor({ x*2 + 1, y*2 + 1 });
						mmlVector<4> v00 = mmlVector<4>( double(c00.r), double(c00.g), double(c00.b), double(c00.blend) );
						mmlVector<4> v10 = mmlVector<4>( double(c10.r), double(c10.g), double(c10.b), double(c10.blend) );
						mmlVector<4> v01 = mmlVector<4>( double(c01.r), double(c01.g), double(c01.b), double(c01.blend) );
						mmlVector<4> v11 = mmlVector<4>( double(c11.r), double(c11.g), double(c11.b), double(c11.blend) );
						mmlVector<4> vx = (v00 + v10 + v01 + v11) / 4.0f;
						b->SetColor({ x, y }, tiny3d::Color{ tiny3d::Byte(vx[0]), tiny3d::Byte(vx[1]), tiny3d::Byte(vx[2]), tiny3d::Byte(vx[3] >= 2.0f ? tiny3d::Color::Solid : tiny3d::Color::Transparent) });
					}
				}
			} else {
				CreateErrorTexture();
				return false;
			}

			mmlSwap(a, b);
		}
	}
	BuildRefs();
	return true;
}

void retro3d::Texture::Copy(const retro3d::Texture &mips)
{
	if (this == &mips) { return; }
	for (tiny3d::UInt i = 0; i < RETRO3D_MIP_COUNT; ++i) {
		m_mip_data[i].Copy(mips.m_mip_data[i]);
	}
	BuildRefs();
}

void retro3d::Texture::Destroy( void )
{
	for (tiny3d::UInt i = 0; i < RETRO3D_MIP_COUNT; ++i) {
		m_mip_data[i].Destroy();
		m_mip_refs[i] = nullptr;
	}
}

void retro3d::Texture::SetBlendMode1(tiny3d::Color::BlendMode blend_mode)
{
	for (tiny3d::UInt i = 0; i < RETRO3D_MIP_COUNT; ++i) {
		m_mip_data[i].SetBlendMode1(blend_mode);
	}
}

void retro3d::Texture::SetBlendMode2(tiny3d::Color::BlendMode blend_mode)
{
	for (tiny3d::UInt i = 0; i < RETRO3D_MIP_COUNT; ++i) {
		m_mip_data[i].SetBlendMode2(blend_mode);
	}
}

void retro3d::Texture::SetBlendModes(tiny3d::Color::BlendMode mode_1, tiny3d::Color::BlendMode mode_2)
{
	SetBlendMode1(mode_1);
	SetBlendMode2(mode_2);
}

void retro3d::Texture::ResetBlendMode1( void )
{
	SetBlendMode1(tiny3d::Color::Transparent);
}

void retro3d::Texture::ResetBlendMode2( void )
{
	SetBlendMode2(tiny3d::Color::Solid);
}

void retro3d::Texture::ResetBlendModes( void )
{
	ResetBlendMode1();
	ResetBlendMode2();
}

const tiny3d::Texture *retro3d::Texture::operator[](tiny3d::UInt level) const
{
	return m_mip_refs[mmlMin(level, tiny3d::UInt(RETRO3D_MIP_COUNT - 1))];
}

retro3d::Texture &retro3d::Texture::operator=(const retro3d::Texture &mips)
{
	Copy(mips);
	return *this;
}

const tiny3d::Texture *retro3d::Texture::GetHighestQuality( void ) const
{
	return m_mip_refs[0];
}

const tiny3d::Texture *retro3d::Texture::GetLowestQuality( void ) const
{
	return m_mip_refs[RETRO3D_MIP_COUNT - 1];
}
