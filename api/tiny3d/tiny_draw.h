#ifndef TINY_DRAW_H
#define TINY_DRAW_H

#include "tiny_math.h"
#include "tiny_image.h"
#include "tiny_texture.h"
#include "tiny_structs.h"

// @data TINY3D_CHAR_WIDTH
// @info The width in pixels of a character in the built-in system font.
#define TINY3D_CHAR_WIDTH  6

// @data TINY3D_CHAR_HEIGHT
// @info The height in pixels of a character in the built-in system font.
#define TINY3D_CHAR_HEIGHT 6

// @data TINY3D_CHAR_FIRST
// @info The first character in the character table in the built-in system font.
#define TINY3D_CHAR_FIRST  '!'

// @data TINY3D_CHAR_LAST
// @info The last character in the character table in the built-in system font.
#define TINY3D_CHAR_LAST   '~'

namespace tiny3d
{
// @algo DrawPoint
// @info Draws a single pixel point on the destination buffer.
// @in
//   zread -> The depth buffer used to determine visibility. NULL to disable depth read.
//   a -> The vertex to render.
//   tex -> The texture to use for rendering. NULL for untextured.
//   dst_rect -> The mask rectangle. Discards rendering outside of the given bounds. NULL for full screen.
// @inout
//   dst -> The destination color buffer to draw a point to.
//   zwrite -> The depth buffer to store depth information in. NULL to disable depth write.
void DrawPoint(tiny3d::Image &dst, const tiny3d::Array<float> *zread, tiny3d::Array<float> *zwrite, const tiny3d::Vertex &a, const tiny3d::Texture *tex, const tiny3d::URect *dst_rect = nullptr);


// @algo DrawLine
// @info Draws a single pixel width line on the destination buffer.
// @in
//   zread -> The depth buffer used to determine visibility. NULL to disable depth read.
//   a, b -> The vertices defining the line segment to render.
//   tex -> The texture to use for rendering. NULL for untextured.
//   dst_rect -> The mask rectangle. Discards rendering outside of the given bounds. NULL for full screen.
// @inout
//   dst -> The destination color buffer to draw a point to.
//   zwrite -> The depth buffer to store depth information in. NULL to disable depth write.
void DrawLine(tiny3d::Image &dst, const tiny3d::Array<float> *zread, tiny3d::Array<float> *zwrite, const tiny3d::Vertex &a, const tiny3d::Vertex &b, const tiny3d::Texture *tex, const tiny3d::URect *dst_rect = nullptr);

// @algo DrawTriangle
// @info Draws a triangle on the destination buffer.
// @in
//   zread -> The depth buffer used to determine visibility. NULL to disable depth read.
//   a, b, c -> The vertices defining the triangle to render.
//   tex -> The texture to use for rendering. NULL for untextured.
//   dst_rect -> The mask rectangle. Discards rendering outside of the given bounds. NULL for full screen.
// @inout
//   dst -> The destination color buffer to draw a point to.
//   zwrite -> The depth buffer to store depth information in. NULL to disable depth write.
void DrawTriangle(tiny3d::Image &dst, const tiny3d::Array<float> *zread, tiny3d::Array<float> *zwrite, const tiny3d::Vertex &a, const tiny3d::Vertex &b, const tiny3d::Vertex &c, const tiny3d::Texture *tex, const tiny3d::URect *dst_rect = nullptr);
void DrawTriangle_Fast(tiny3d::Image &dst, const tiny3d::Array<float> *zread, tiny3d::Array<float> *zwrite, const tiny3d::Vertex &a, const tiny3d::Vertex &b, const tiny3d::Vertex &c, const tiny3d::Texture *tex, const tiny3d::URect *dst_rect = nullptr);

// @algo DrawTriangle
// @info Draws a lightmap shaded triangle to the destination buffer.
// @in
//   zread -> The depth buffer used to determine visibility. NULL to disable depth read.
//   a, b, c -> The vertices defining the triangle to render.
//   tex -> The texture to use for rendering. NULL for untextured.
//   lightmap -> The non-optional light map used for shading the triangle.
//   dst_rect -> The mask rectangle. Discards rendering outside of the given bounds. NULL for full screen.
// @inout
//   dst -> The destination color buffer to draw a point to.
//   zwrite -> The depth buffer to store depth information in. NULL to disable depth write.
void DrawTriangle(tiny3d::Image &dst, const tiny3d::Array<float> *zread, tiny3d::Array<float> *zwrite, const tiny3d::LVertex &a, const tiny3d::LVertex &b, const tiny3d::LVertex &c, const tiny3d::Texture *tex, const tiny3d::Texture &lightmap, const tiny3d::URect *dst_rect = nullptr);

// @algo DrawChars
// @info Draws a series of characters to the destination buffer using the built-in system font.
// @in
//   p -> The origin of render (top-left corner of text).
//   x_margin -> The left margin used when resetting caret on new line.
//   ch -> The series of characgters to render.
//   ch_num -> The number of characters to render.
//   color -> The color of the text.
//   scale -> The integer scale of the text.
//   dst_rect -> The mask rectangle. Discards rendering outside of the given bounds. NULL for full screen.
// @inout
//   dst -> The destination color buffer to draw a point to.
tiny3d::Point DrawChars(tiny3d::Image &dst, tiny3d::Point p, tiny3d::SInt x_margin, const char *ch, tiny3d::UInt ch_num, tiny3d::Color color, tiny3d::UInt scale, const tiny3d::URect *dst_rect = nullptr);

	
//void DrawRegion(tiny3d::Image &dst, tiny3d::Rect dst_rect, const tiny3d::Image &src, tiny3d::Rect src_rect);

}

#endif // TINY_DRAW_H
