#include "tiny_texture.h"
#include "tiny_image.h"

using namespace tiny3d;

static constexpr tiny3d::UInt CCC_DIM         =  4;
static constexpr tiny3d::UInt CCC_DIM_MASK    =  3;
static constexpr tiny3d::UInt CCC_DIM_SHIFT   =  2;
static constexpr tiny3d::UInt CCC_COUNT       = 16;
static constexpr tiny3d::UInt CCC_COUNT_MASK  = 15;
static constexpr tiny3d::UInt CCC_COUNT_SHIFT =  4;

bool tiny3d::Texture::SetDimension(tiny3d::UInt dimension)
{
	const bool is_valid = dimension >= MinDimension() && dimension <= MaxDimension() && tiny3d::IsPow2(dimension);
	if (is_valid) {
		m_dimension   = dimension;
		m_dim_mask    = dimension - 1;
		m_dim_shift   = UHInt(tiny3d::Log2(UInt(dimension)));
		m_fix_dim     = Real(SInt(dimension));
		m_blocks      = dimension / MinDimension();
		m_block_mask  = m_blocks - 1;
		m_block_shift = UHInt(tiny3d::Log2(UInt(m_blocks)));
		m_fix_blocks  = Real(SInt(m_blocks));
	} else {
		m_dimension   = 0;
		m_dim_mask    = 0;
		m_dim_shift   = 0;
		m_fix_dim     = Real(0);
		m_blocks      = 0;
		m_block_mask  = 0;
		m_block_shift = 0;
		m_fix_blocks  = Real(0);
	}
	return is_valid;
}

tiny3d::UInt tiny3d::Texture::GetMortonIndex(tiny3d::UPoint p) const
{
	p.x = (p.x | (p.x << 8)) & 0x00FF00FF;
	p.x = (p.x | (p.x << 4)) & 0x0F0F0F0F;
	p.x = (p.x | (p.x << 2)) & 0x33333333;
	p.x = (p.x | (p.x << 1)) & 0x55555555;

	p.y = (p.y | (p.y << 8)) & 0x00FF00FF;
	p.y = (p.y | (p.y << 4)) & 0x0F0F0F0F;
	p.y = (p.y | (p.y << 2)) & 0x33333333;
	p.y = (p.y | (p.y << 1)) & 0x55555555;

	return p.x | (p.y << 1);
}

tiny3d::UPoint tiny3d::Texture::GetXY(tiny3d::Vector2 uv) const
{
	UPoint p;
	p.x = UInt(SInt(uv.x * m_fix_dim)) & m_dim_mask;
	p.y = UInt(SInt(uv.y * m_fix_dim)) & m_dim_mask;
	return p;
}

tiny3d::UPoint tiny3d::Texture::GetXY(tiny3d::UPoint p) const
{
	p.x &= m_dim_mask;
	p.y &= m_dim_mask;
	return p;
}

UPoint BlockXY(UPoint p)
{
	return UPoint{ p.x / CCC_DIM, p.y / CCC_DIM };
}

UInt BitI(UPoint p)
{
	return (p.x % CCC_DIM) + ((p.y % CCC_DIM) * CCC_DIM);
}

tiny3d::Texture::Index tiny3d::Texture::GetIndex(tiny3d::Vector2 uv) const
{
	UPoint p = GetXY(uv);
	return Index{ GetMortonIndex(BlockXY(p)), BitI(p) };
}

tiny3d::Texture::Index tiny3d::Texture::GetIndex(tiny3d::UPoint p) const
{
	p = GetXY(p);
	return Index{ GetMortonIndex(BlockXY(p)), BitI(p) };
}

tiny3d::UHInt tiny3d::Texture::EncodeTexel(tiny3d::Color color) const
{
	// bits = M BBBBB GGGGG RRRRR
	constexpr UInt FIX_SCALAR = (32 << 8) / 255;
	const UInt r = (UInt(color.r) * FIX_SCALAR) >> 8;
	const UInt g = (UInt(color.g) * FIX_SCALAR) >> 8;
	const UInt b = (UInt(color.b) * FIX_SCALAR) >> 8;
	const UInt stencil = UInt(color.blend & 1) << 15;
	const UHInt pixel = UHInt(stencil) | UHInt(b << 10) | UHInt(g << 5) | UHInt(r);
	return pixel;
}

tiny3d::Color tiny3d::Texture::DecodeTexel(tiny3d::UHInt texel) const
{
	// bits = M BBBBB GGGGG RRRRR
	constexpr UInt FIX_SCALAR = (256 << 8) / 31;
	const UInt t32 = UInt(texel);
	const UInt r = ((t32 & 0x001F) * FIX_SCALAR) >> 8;
	const UInt g = (((t32 & 0x03E0) * FIX_SCALAR) >> 13);
	const UInt b = (((t32 & 0x7C00) * FIX_SCALAR) >> 18);
	Color color;
	color.r = Byte(r);
	color.g = Byte(g);
	color.b = Byte(b);
	color.blend = (texel & 0x8000) ? m_blend_modes[1] : m_blend_modes[0];
	return color;
}

tiny3d::Color tiny3d::Texture::GetColor(tiny3d::Texture::Index i) const
{
	TINY3D_ASSERT(i.block < m_blocks * m_blocks);
	const CCCBlock &b = m_texels[i.block];
	return DecodeTexel(b.colors[ReadBit(b.color_idx, i.bit)]);
}

tiny3d::Texture::Texture( void ) : m_texels(nullptr), m_dimension(0), m_dim_mask(0), m_dim_shift(0), m_fix_dim(0), m_blocks(0), m_block_mask(0), m_block_shift(0), m_fix_blocks(0), m_blend_modes{ Color::Transparent, Color::Solid }
{}

tiny3d::Texture::Texture(tiny3d::UInt dimension) : Texture()
{
	Create(dimension);
}

tiny3d::Texture::Texture(const tiny3d::Image &img) : Texture()
{
	FromImage(img);
}

tiny3d::Texture::Texture(const tiny3d::Texture &t) : Texture()
{
	Copy(t);
}

tiny3d::Texture::~Texture( void )
{
	delete [] m_texels;
}

bool tiny3d::Texture::Create(tiny3d::UInt dimension)
{
	if (dimension != m_dimension) {
		delete m_texels;
		if (SetDimension(dimension)) {
			m_texels = new CCCBlock[m_blocks * m_blocks];
		} else {
			m_texels = nullptr;
			return false;
		}
	}
	return true;
}

void tiny3d::Texture::Destroy( void )
{
	delete [] m_texels;
	m_texels = nullptr;
	SetDimension(0);
}

void tiny3d::Texture::Copy(const tiny3d::Texture &t)
{
	if (this == &t) { return; }
	Create(t.m_dimension);
	m_blend_modes[0] = t.m_blend_modes[0];
	m_blend_modes[1] = t.m_blend_modes[1];
	UInt size = m_blocks * m_blocks;
	for (UInt i = 0; i < size; ++i) {
		m_texels[i] = t.m_texels[i];
	}
}

bool tiny3d::Texture::ToImage(tiny3d::Image &image) const
{
	UInt dim = GetWidth();
	if (!image.Create(dim, dim)) { return false; }
	for (UInt y = 0; y < dim; ++y) {
		for (UInt x = 0; x < dim; ++x) {
			image.SetColor({ x, y }, GetColor(UPoint{ x, y }));
		}
	}
	return true;
}

struct IColor { UInt r, g, b; float blend; };
IColor operator/(IColor l, UInt r) { return IColor{ l.r / r, l.g / r, l.b / r, l.blend / r }; }
IColor operator+(IColor l, tiny3d::Color r) { return IColor{ l.r + r.r, l.g + r.g, l.b + r.b, l.blend + float(r.blend & 1) }; }
tiny3d::Color ToColor(IColor c) { return Color{ Byte(c.r), Byte(c.g), Byte(c.b), Byte(c.blend + 0.5f) }; }

tiny3d::UInt AverageLuminance(const tiny3d::Image &img, tiny3d::UPoint p)
{
	UInt avg_lum = 0;
	for (UInt sy = 0; sy < CCC_DIM; ++sy) {
		for (UInt sx = 0; sx < CCC_DIM; ++sx) {
			avg_lum += Illum(img.GetColor(UPoint{ p.x + sx, p.y + sy }));
		}
	}
	return avg_lum / CCC_COUNT;
}

tiny3d::UHInt LookupBits(const tiny3d::Image &img, tiny3d::UPoint p, tiny3d::UInt avg_lum)
{
	UHInt lookup = 0;
	for (UInt sy = 0; sy < CCC_DIM; ++sy) {
		for (UInt sx = 0; sx < CCC_DIM; ++sx) {
			Byte lum = Illum(img.GetColor(UPoint{ p.x + sx, p.y + sy }));
			UInt i = sx + sy * CCC_DIM;
			if (lum <= avg_lum) {
				ClearBit(lookup, i);
			} else {
				SetBit(lookup, i);
			}
		}
	}
	return lookup;
}

tiny3d::Color AverageColor0(const tiny3d::Image &img, tiny3d::UPoint p, tiny3d::UInt lookup)
{
	UInt num = 0;
	IColor avg_col = { 0, 0, 0, 0 };
	for (UInt sy = 0; sy < CCC_DIM; ++sy) {
		for (UInt sx = 0; sx < CCC_DIM; ++sx) {
			if (!TestBit(lookup, sx + sy * CCC_DIM)) {
				++num;
				avg_col = avg_col + img.GetColor(UPoint{ p.x + sx, p.y + sy });
			}
		}
	}
	avg_col = avg_col / (num > 0 ? num : 1);
	return ToColor(avg_col);
}

tiny3d::Color AverageColor1(const tiny3d::Image &img, tiny3d::UPoint p, tiny3d::UInt lookup)
{
	UInt num = 0;
	IColor avg_col = { 0, 0, 0, 0 };
	for (UInt sy = 0; sy < CCC_DIM; ++sy) {
		for (UInt sx = 0; sx < CCC_DIM; ++sx) {
			if (TestBit(lookup, sx + sy * CCC_DIM)) {
				++num;
				avg_col = avg_col + img.GetColor(UPoint{ p.x + sx, p.y + sy });
			}
		}
	}
	avg_col = avg_col / (num > 0 ? num : 1);
	return ToColor(avg_col);
}

bool tiny3d::Texture::FromImage(const tiny3d::Image &image)
{
	if (image.GetWidth() != image.GetHeight() || !Create(image.GetWidth())) { return false; }

	UInt dim = GetWidth();
	for (UInt y = 0; y < dim; y += CCC_DIM) {
		for (UInt x = 0; x < dim; x += CCC_DIM) {

			UPoint p = { x, y };

			// calculate the average luminance value per 4x4 block
			UInt avg_lum = AverageLuminance(image, p);

			// determine what color lookup table points to based on if luminance of current color is more or less than average luminance
			CCCBlock &block = m_texels[GetIndex(p).block];
			block.color_idx = LookupBits(image, p, avg_lum);

			// determine representative color by averaging the colors on either side of the average luminance value
			block.colors[0] = EncodeTexel(AverageColor0(image, p, block.color_idx));
			block.colors[1] = EncodeTexel(AverageColor1(image, p, block.color_idx));
		}
	}

	return true;
}

tiny3d::UInt tiny3d::Texture::GetWidth( void ) const
{
	return m_dimension;
}

tiny3d::UInt tiny3d::Texture::GetHeight( void ) const
{
	return m_dimension;
}

tiny3d::Color tiny3d::Texture::GetColor(tiny3d::Vector2 uv) const
{
	Index i = GetIndex(uv);
	return GetColor(i);
}

tiny3d::Color tiny3d::Texture::GetColor(tiny3d::UPoint p) const
{
	Index i = GetIndex(p);
	return GetColor(i);
}

tiny3d::Color::BlendMode tiny3d::Texture::GetBlendMode1( void ) const
{
	return m_blend_modes[0];
}

tiny3d::Color::BlendMode tiny3d::Texture::GetBlendMode2( void ) const
{
	return m_blend_modes[1];
}

void tiny3d::Texture::SetBlendMode1(tiny3d::Color::BlendMode blend_mode)
{
	m_blend_modes[0] = blend_mode;
}

void tiny3d::Texture::SetBlendMode2(tiny3d::Color::BlendMode blend_mode)
{
	m_blend_modes[1] = blend_mode;
}

tiny3d::Vector2 tiny3d::Texture::ProjectUV(tiny3d::Vector2 uv) const
{
	return Vector2{ uv.x * m_fix_dim, uv.y * m_fix_dim };
}

tiny3d::Texture &tiny3d::Texture::operator=(const tiny3d::Texture &t)
{
	Copy(t);
	return *this;
}
