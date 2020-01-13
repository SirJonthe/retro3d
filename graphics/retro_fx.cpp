#include "retro_fx.h"

void retro3d::FX_InitFire( void ) {}
void retro3d::FX_SpreadFire(const tiny3d::Color *fire_palette) {}

void retro3d::FX_Desaturate(tiny3d::Image &img, const tiny3d::URect *dst_rect)
{
	tiny3d::URect fs_rect = tiny3d::URect{ tiny3d::UPoint{ 0, 0 }, tiny3d::UPoint{ img.GetWidth(), img.GetHeight() } };
	tiny3d::URect rect = dst_rect != nullptr ? tiny3d::Clip(*dst_rect, fs_rect) : fs_rect;

	for (tiny3d::UInt y = rect.a.y; y < rect.b.y; ++y) {
		for (tiny3d::UInt x = rect.a.x; x < rect.b.x; ++x) {
			tiny3d::UPoint p = { x, y };
			tiny3d::Color  c = img.GetColor(p);
			tiny3d::Byte   i = tiny3d::Illum(c);
			c = { i, i, i, c.blend };
			img.SetColor(p, c);
		}
	}
}
