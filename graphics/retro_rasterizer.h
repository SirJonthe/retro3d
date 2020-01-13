#ifndef RETRO_RASTERIZER_H
#define RETRO_RASTERIZER_H

#include "../api/tiny3d/tiny3d.h"

namespace retro3d
{

template < typename attr_t >
struct ShaderInterface
{
	virtual tiny3d::Color operator()(tiny3d::UPoint screen_coord, tiny3d::Color in_color, const attr_t &attr) = 0;
};

template < typename attr_t, typename shader_t >
void Rasterize(tiny3d::Image &dst, tiny3d::Point a, tiny3d::Point b, tiny3d::Point c, const attr_t &aa, const attr_t &ba, const attr_t &ca, shader_t shader, bool no_overlap = true)
{
	struct Helper {
		static tiny3d::SXInt DetermineHalfSpace(tiny3d::Point a, tiny3d::Point b, tiny3d::Point point) {
			return tiny3d::SXInt(b.x - a.x) * tiny3d::SXInt(point.y - a.y) - tiny3d::SXInt(b.y - a.y) * tiny3d::SXInt(point.x - a.x);
		}
		static bool IsTopLeft(tiny3d::Point a, tiny3d::Point b) {
			return (a.x < b.x && b.y == a.y) || (a.y > b.y);
		}
	};

	// AABB Clipping
	tiny3d::SInt min_y = tiny3d::Max(tiny3d::Min(a.y, b.y, c.y), tiny3d::SInt(0));
	tiny3d::SInt max_y = tiny3d::Min(tiny3d::Max(a.y, b.y, c.y), tiny3d::SInt(dst.GetHeight() - 1));
	if (max_y - min_y <= 0) { return; }
	tiny3d::SInt min_x = tiny3d::Max(tiny3d::Min(a.x, b.x, c.x), tiny3d::SInt(0));
	tiny3d::SInt max_x = tiny3d::Min(tiny3d::Max(a.x, b.x, c.x), tiny3d::SInt(dst.GetWidth() - 1));
	if (max_x - min_x <= 0) { return; }

	// Triangle setup
	tiny3d::Point p    = { min_x, min_y };
	tiny3d::SXInt w0_y = Helper::DetermineHalfSpace(b, c, p);
	tiny3d::SXInt w1_y = Helper::DetermineHalfSpace(c, a, p);
	tiny3d::SXInt w2_y = Helper::DetermineHalfSpace(a, b, p);

	// Interpolation/triangle setup
	const tiny3d::SInt w2_x_inc = a.y - b.y;
	const tiny3d::SInt w2_y_inc = b.x - a.x;
	const tiny3d::SInt w0_x_inc = b.y - c.y;
	const tiny3d::SInt w0_y_inc = c.x - b.x;
	const tiny3d::SInt w1_x_inc = c.y - a.y;
	const tiny3d::SInt w1_y_inc = a.x - c.x;
	const float sum_inv_area_x2 = 1.0f / tiny3d::SInt(w0_y + w1_y + w2_y);
	float l0_y                  = tiny3d::SInt(w0_y) * sum_inv_area_x2;
	float l1_y                  = tiny3d::SInt(w1_y) * sum_inv_area_x2;
	float l2_y                  = tiny3d::SInt(w2_y) * sum_inv_area_x2;
	const float l0_x_inc        = w0_x_inc * sum_inv_area_x2;
	const float l1_x_inc        = w1_x_inc * sum_inv_area_x2;
	const float l2_x_inc        = w2_x_inc * sum_inv_area_x2;
	const float l0_y_inc        = w0_y_inc * sum_inv_area_x2;
	const float l1_y_inc        = w1_y_inc * sum_inv_area_x2;
	const float l2_y_inc        = w2_y_inc * sum_inv_area_x2;

	if (no_overlap == true) { // add offsets to coordinates to enforce fill convention
		w0_y += Helper::IsTopLeft(b, c) ? 0 : -1;
		w1_y += Helper::IsTopLeft(c, a) ? 0 : -1;
		w2_y += Helper::IsTopLeft(a, b) ? 0 : -1;
	}

	for (p.y = min_y; p.y <= max_y; ++p.y) {

		tiny3d::SInt w0 = tiny3d::SInt(w0_y);
		tiny3d::SInt w1 = tiny3d::SInt(w1_y);
		tiny3d::SInt w2 = tiny3d::SInt(w2_y);

		float l0 = l0_y;
		float l1 = l1_y;
		float l2 = l2_y;

		for (p.x = min_x; p.x <= max_x; ++p.x) {

			if ((w0 | w1 | w2) >= 0) {

				const tiny3d::UPoint q     = { tiny3d::UInt(p.x), tiny3d::UInt(p.y) };
				const tiny3d::Color  pixel = dst.GetColor(q);

				if (pixel.blend != tiny3d::Color::Transparent) { // use transparency bit as a 1-bit stencil
					dst.SetColor(q, shader(q, dst.GetColor(q), aa * l0 + ba * l1 + ca * l2));
				}
			}

			w0 += w0_x_inc;
			w1 += w1_x_inc;
			w2 += w2_x_inc;

			l0 += l0_x_inc;
			l1 += l1_x_inc;
			l2 += l2_x_inc;
		}

		w0_y += w0_y_inc;
		w1_y += w1_y_inc;
		w2_y += w2_y_inc;

		l0_y += l0_y_inc;
		l1_y += l1_y_inc;
		l2_y += l2_y_inc;
	}
}

}

#endif // RETRO_RASTERIZER_H
