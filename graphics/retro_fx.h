#ifndef RETRO_FX_H
#define RETRO_FX_H

#include <cstdint>
#include "../api/tiny3d/tiny_structs.h"
#include "../api/tiny3d/tiny_image.h"

namespace retro3d
{

static constexpr uint32_t FX_FIRE_PALETTE_SIZE = 38;
static constexpr tiny3d::Color FX_FIRE_PALETTE[FX_FIRE_PALETTE_SIZE] = {
	tiny3d::Color{0x00,0x00,0x00,tiny3d::Color::Transparent},
	tiny3d::Color{0x07,0x07,0x07,tiny3d::Color::Transparent},
	tiny3d::Color{0x1F,0x07,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0x2F,0x0F,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0x47,0x0F,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0x57,0x17,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0x67,0x1F,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0x77,0x1F,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0x8F,0x27,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0x9F,0x2F,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0xAF,0x3F,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0xBF,0x47,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0xC7,0x47,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0xDF,0x4F,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0xDF,0x57,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0xDF,0x57,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0xD7,0x5F,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0xD7,0x5F,0x07,tiny3d::Color::Solid},
	tiny3d::Color{0xD7,0x67,0x0F,tiny3d::Color::Solid},
	tiny3d::Color{0xCF,0x6F,0x0F,tiny3d::Color::Solid},
	tiny3d::Color{0xCF,0x77,0x0F,tiny3d::Color::Solid},
	tiny3d::Color{0xCF,0x7F,0x0F,tiny3d::Color::Solid},
	tiny3d::Color{0xCF,0x87,0x17,tiny3d::Color::Solid},
	tiny3d::Color{0xC7,0x87,0x17,tiny3d::Color::Solid},
	tiny3d::Color{0xC7,0x8F,0x17,tiny3d::Color::Solid},
	tiny3d::Color{0xC7,0x97,0x1F,tiny3d::Color::Solid},
	tiny3d::Color{0xBF,0x9F,0x1F,tiny3d::Color::Solid},
	tiny3d::Color{0xBF,0x9F,0x1F,tiny3d::Color::Solid},
	tiny3d::Color{0xBF,0xA7,0x27,tiny3d::Color::Solid},
	tiny3d::Color{0xBF,0xA7,0x27,tiny3d::Color::Solid},
	tiny3d::Color{0xBF,0xAF,0x2F,tiny3d::Color::Solid},
	tiny3d::Color{0xB7,0xAF,0x2F,tiny3d::Color::Solid},
	tiny3d::Color{0xB7,0xB7,0x2F,tiny3d::Color::Solid},
	tiny3d::Color{0xB7,0xB7,0x37,tiny3d::Color::Solid},
	tiny3d::Color{0xCF,0xCF,0x6F,tiny3d::Color::Solid},
	tiny3d::Color{0xDF,0xDF,0x9F,tiny3d::Color::Solid},
	tiny3d::Color{0xEF,0xEF,0xC7,tiny3d::Color::Solid},
	tiny3d::Color{0xFF,0xFF,0xFF,tiny3d::Color::Solid}
};

void FX_InitFire();
void FX_SpreadFire(const tiny3d::Color *fire_palette = FX_FIRE_PALETTE);
void FX_Desaturate(tiny3d::Image &img, const tiny3d::URect *dst_rect = nullptr);

}

#endif // RETRO_FX_H
