#ifndef TINY_STRUCTS_H
#define TINY_STRUCTS_H

#include "tiny_system.h"
#include "tiny_math.h"

namespace tiny3d
{

// @data Point
// @info Contains coordinates for a point in signed space.
struct Point
{
	SInt x, y;
};

// @data UPoint
// @info Contains coordinates for a point in unsigned space.
struct UPoint
{
	UInt x, y;
};

// @data Rect
// @info Contains minimum (a) and maximum (b) bounds for a rectangle in signed space.
struct Rect
{
	Point a, b;
};

// @algo Clip
// @info Gets the overlapping rectangle between two rectangles (if any).
// @in a, b -> Input rectangles.
// @out Overlapping rectangle (zero area if none).
Rect Clip(Rect a, Rect b);

// @data URect
// @info Contains minimum (a) and maximum (b) bounds for a rectangle in unsigned space.
struct URect
{
	UPoint a, b;
};

// @algo Clip
// @info Gets the overlapping rectangle between two rectangles (if any).
// @in a, b -> Input rectangles.
// @out Overlapping rectangle (zero area if none).
URect Clip(URect a, URect b);

	
// @data Color
// @info Contains 32-bit color. RGB and blend mode.
struct Color
{
	// @data BlendMode
	// @info Possible values for blend mode.
	enum BlendMode
	{
		Transparent,     // don't render the pixel
		Emissive,        // render the pixel solid with lighting
		AddAlpha,        // additive alpha
		Solid,           // don't add lighting to the pixel
		EmissiveAddAlpha // emissive additive alpha
	};

	Byte r, g, b, blend;
};

Color operator+(Color l, Color r);
Color operator-(Color l, Color r);
Color operator*(Color l, Color r);
Color operator*(Color l, Real r);
Color operator/(Color l, Real r);

Color Lerp(Color a, Color b, Real x);

// @algo Dither2x2
// @info Downsamples a 32-bit color to a 16-bit boundary based on where on the destination buffer the color is to be rendered. Used to improve quantization artifacts. 2x2 dither kernel.
// @in
//   c -> The color to be downsampled.
//   p -> The point on the destination buffer to be rendered. Used to sample the dithering kernel.
// @out The final color.
Color Dither2x2(Color c, UPoint p);
	
// @algo Dither3x3
// @info Downsamples a 32-bit color to a 16-bit boundary based on where on the destination buffer the color is to be rendered. Used to improve quantization artifacts. 3x3 dither kernel.
// @in
//   c -> The color to be downsampled.
//   p -> The point on the destination buffer to be rendered. Used to sample the dithering kernel.
// @out The final color.
Color Dither3x3(Color c, UPoint p);
	
// @algo Dither4x4
// @info Downsamples a 32-bit color to a 16-bit boundary based on where on the destination buffer the color is to be rendered. Used to improve quantization artifacts. 4x4 dither kernel.
// @in
//   c -> The color to be downsampled.
//   p -> The point on the destination buffer to be rendered. Used to sample the dithering kernel.
// @out The final color.
Color Dither4x4(Color c, UPoint p);
	
// @algo Dither8x8
// @info Downsamples a 32-bit color to a 16-bit boundary based on where on the destination buffer the color is to be rendered. Used to improve quantization artifacts. 8x8 dither kernel.
// @in
//   c -> The color to be downsampled.
//   p -> The point on the destination buffer to be rendered. Used to sample the dithering kernel.
// @out The final color.
Color Dither8x8(Color c, UPoint p);

// @algo Dither2x2
// @info Adds an offset to a texture UV (in texture space) to make for a bilinear approximation. 2x2 dither kernel used.
// @in
//   texture_space_uv -> UV coorinates in non-normalized texture space.
//   p -> The point on the destination buffer to be rendered. Used to sample the dithering kernel.
// @out The final texture coordinate to sample.
UPoint Dither2x2(Vector2 texture_space_uv, UPoint p);

// @algo Illum
// @in c -> A color.
// @out The grayscale (or illuminance) of the color.
Byte Illum(Color c);

// @algo Decode
// @info Decodes a 16-bit color into a 32-bit color.
// @in c -> The 16-bit color.
// @out the decoded 32-bit color.
Color Decode(UHInt c);
	
// @algo Encode
// @info Encodes a 32-bit color into a 16-bit color.
// @in c -> The 32-bit color.
// @out The 16-bit color.
UHInt Encode(Color c);

// @data Vertex
// @info Contains information the rendering API interpolates and uses to determine final color of a pixel.
struct Vertex
{
	Vector3 v;
	Vector2 t;
	Color   c;
};

// @data LVertex
// @info Contains information the rendering API interpolates and uses to determine final color of a pixel. Special use for light mapped triangles.
struct LVertex
{
	Vector3 v;
	Vector2 t;
	Vector2 l;
};

// @data SampleMode
// @info Contains all possible values for texture sampling.
enum SampleMode
{
	SampleMode_Nearest,
	SampleMode_Dither,
	SampleMode_Bilinear,
};

// @data Array
// @info A frequently used data structure used to contain an array of data stored linearly in memory.
template < typename type_t >
class Array
{
private:
	type_t *m_arr;
	UInt    m_size;

public:
	 Array( void )                 : m_arr(nullptr), m_size(0) {}
	 Array(const Array<type_t> &a) : Array()                   { Copy(a); }
	 explicit Array(UInt num)      : Array()                   { Create(num); }
	~Array( void )                                             { delete [] m_arr; }
	
	// @algo Create
	// @info Creates an array with the given number of elements.
	// @in num -> The number of elements to create.
	void Create(UInt num)
	{
		if (num == m_size) { return; }
		delete [] m_arr;
		m_arr = (num > 0) ? new type_t[num] : nullptr;
		m_size = num;
	}
	
	// @algo Destroy
	// @info Frees resources used by array.
	void Destroy( void )
	{
		delete [] m_arr;
		m_arr = nullptr;
		m_size = 0;
	}
	
	// @algo Copy
	// @info Copies an array.
	// @in a -> The array to copy.
	void Copy(const Array<type_t> &a)
	{
		if (this == &a) { return; }
		Create(a.m_size);
		for (UInt i = 0; i < m_size; ++i) {
			m_arr[i] = a.m_arr[i];
		}
	}
	
	// @algo =
	// @info Copies an array.
	// @in a -> The array to copy.
	// @out The array (self).
	Array<type_t> &operator=(const Array<type_t> &a)
	{
		Copy(a);
		return *this;
	}
	
	// @algo []
	// @info Used to fetch an element in the array.
	// @in i -> The index of the element to fetch.
	// @out The element.
	type_t &operator[](UInt i)
	{
		TINY3D_ASSERT(i < m_size);
		return m_arr[i];
	}
	const type_t &operator[](UInt i) const
	{
		TINY3D_ASSERT(i < m_size);
		return m_arr[i];
	}
	
	// @algo type_t*
	// @out The raw array of elements.
	operator type_t*( void )             { return m_arr; }
	operator const type_t*( void ) const { return m_arr; }
	
	// @algo GetSize
	// @out The number of elements in the array.
	UInt GetSize( void ) const { return m_size; }
};

}

#endif // TINY_STRUCTS_H
