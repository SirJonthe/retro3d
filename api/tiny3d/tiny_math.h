#ifndef TINY_MATH_H
#define TINY_MATH_H

#include <limits>
#include "tiny_system.h"

namespace tiny3d
{

// @algo Min
// @info Returns the minimum number given two input values.
// @in a, b -> Test values.
// @out The minimum value.
template < typename num_t >  num_t  Min(num_t a, num_t b)          { return a < b ? a : b; }
	
// @algo Max
// @info Returns the maximum number given two input values.
// @in a, b -> Test values.
// @out The maximum value.
template < typename num_t >  num_t  Max(num_t a, num_t b)          { return a > b ? a : b; }
	
// @algo Min
// @info Returns the minimum number given three input values.
// @in a, b, c -> Test values.
// @out The minimum value.
template < typename num_t >  num_t  Min(num_t a, num_t b, num_t c) { return tiny3d::Min(tiny3d::Min(a, b), c); }
	
// @algo Max
// @info Returns the maximum number given three input values.
// @in a, b -> Test values.
// @out The maximum value.
template < typename num_t >  num_t  Max(num_t a, num_t b, num_t c) { return tiny3d::Max(tiny3d::Max(a, b), c); }
	
// @algo Swap
// @info Swaps two elements.
// @inout a, b -> The elements to be swapped.
template < typename type_t > void   Swap(type_t &a, type_t &b)     { type_t c = a; a = b; b = c; }
	
// @algo Sign2
// @info Returns the sign of the input signed number.
// @in x -> Signed number.
// @out The sign (-1, 1).
template < typename sint_t > sint_t Sign2(sint_t x)                { return x < 0 ? -1 : 1; }
	
// @algo Sign3
// @info Returns the sign of the input signed number.
// @in x -> Signed number
// @out The sign (-1, 0, 1).
template < typename sint_t > sint_t Sign3(sint_t x)                { return x < 0 ? -1 : (x > 0 ? 1 : 0); }

// @algo Abs
// @info Returns the absolute value of the input.
// @in x -> A value.
// @out The absolute value.
template < typename type_t > type_t Abs(type_t x)                           { return tiny3d::Max(x, -x); }
	
// @algo Clamp
// @info Contains a given input to a specified interval by saturating value at end points on over-/underflow.
// @in
//   min, max -> The interval.
//   x -> The value.
// @out The clamped value.
template < typename type_t > type_t Clamp(type_t min, type_t x, type_t max) { return tiny3d::Min(tiny3d::Max(x, min), max); }
	
// @algo Wrap
// @info Constains a given input to a specified interval by wrapping around value at end points on over-/underflow.
// @in
//   min, max -> The interval.
//   x -> The value.
// @out The wrapped value.
template < typename type_t > type_t Wrap(type_t min, type_t x, type_t max)  { return min + (x + tiny3d::Abs(x) * (max - min) - min) % (max - min); }

// @algo Log
// @info Returns integer log using specified base.
// @in
//   base -> The base used to get the log.
//   x -> The input to check the log.
// @out The log.
UInt  Log(UInt base, UInt x);
	
	// @algo Log
	// @info Returns integer log using 10 as base.
	// @in
	//   x -> The input to check the log.
	// @out The log.
UInt  Log10(UInt x);
	
// @algo Log
// @info Returns integer log using 2 as base.
// @in
//   x -> The input to check the log.
// @out The log.
UInt  Log2(UInt x);
	
// @algo Exp
// @info Returns integer exponent computation.
// @in
//   x -> The base.
//   n -> The exponent.
// @out x^n.
UXInt Exp(UInt x, UInt n);
	
// @algo IsPow2
// @info Checks if a number is a power of 2.
// @in
//   x -> The input to check.
// @out TRUE if number is a power of 2.
bool  IsPow2(UInt x);
	
// @algo Interleave16Bits
// @info Interleaves two 16-bit bit patterns into one 32-bit bit pattern.
// @in
//   base -> The base used to get the log.
//   a, b -> The bit patterns to interleave.
// @out 0bABABABABABABABABABABABABABABABAB.
UInt  Interleave16Bits(UHInt a, UHInt b);
	
// @algo Interleave8Bits
// @info Interleaves two 8-bit bit patterns into one 16-bit bit pattern.
// @in
//   base -> The base used to get the log.
//   a, b -> The bit patterns to interleave.
// @out 0bABABABABABABABAB.
UHInt Interleave8Bits(Byte a, Byte b);

// @algo SetBit
// @info Sets the specified bit to 1.
// @in
//   i -> The bit index to modify.
// @inout
//   bits -> The bit pattern to modify.
template < typename int_t >
void SetBit(int_t &bits, tiny3d::UInt i) { bits |= (1 << i); }
	
// @algo ClearBit
// @info Sets the specified bit to 0.
// @in
//   i -> The bit index to modify.
// @inout
//   bits -> The bit pattern to modify.
template < typename int_t >
void ClearBit(int_t &bits, tiny3d::UInt i) { bits &= (~(1 << i)); }
	
// @algo ToggleBit
// @info Sets the specified bit to 1 if 0, or 0 if 1.
// @in
//   i -> The bit index to modify.
// @inout
//   bits -> The bit pattern to modify.
template < typename int_t >
void ToggleBit(int_t &bits, tiny3d::UInt i) { bits ^= (1 << i); }
	
// @algo TestBit
// @info Sets the specified bit to 1.
// @in
//   bits -> The bit pattern to check.
//   i -> The bit index to check.
// @out The bit state (TRUE if 1, FALSE if 0).
template < typename int_t >
bool TestBit(int_t bits, tiny3d::UInt i) { return (bits & (1 << i)) != 0; }
	
// @algo ReadBit
// @info Returns the bit pattern with only the state of one specified bit.
// @in
//   bits -> The bit pattern to check.
//   i -> The bit index to check.
// @out The bit state.
template < typename int_t >
int_t ReadBit(int_t bits, tiny3d::UInt i) { return (bits >> i) & 1; }

/*class XReal;

// if 1 unit = 1 meter, where standard 16 gives us a range of -16 - 16 kms (32 km) with a precision of 1/65 mm
class Real
{
private:
	friend class XReal;
	typedef SInt real_t;
	static constexpr real_t S_INF_BIT  = std::numeric_limits<real_t>::max();
	static constexpr real_t S_NINF_BIT = std::numeric_limits<real_t>::min() + 1;
	static constexpr real_t S_NAN_BIT  = std::numeric_limits<real_t>::min();
	static constexpr real_t S_PREC     = (CHAR_BIT * sizeof(real_t)) / 2;
	static constexpr real_t S_DEC_STEP = 1;
	static constexpr real_t S_INT_STEP = 1 << S_PREC;

private:
	real_t x;

public:
	Real( void );
	Real(const Real &r);
	explicit Real(SInt i);
	explicit Real(float f);
	explicit operator SInt( void ) const;

	static Real Inf( void );
	static Real NInf( void );
	static Real NaN( void );
	static Real SmallestInt( void );
	static Real SmallestFrac( void );
	static Real Max( void );
	static Real Min( void );
	bool IsNaN( void )  const;
	bool IsInf( void )  const;
	bool IsNInf( void ) const;

	Real operator-( void ) const;
	Real &operator+=(Real r);
	Real &operator-=(Real r);
	Real &operator*=(Real r);
	Real &operator/=(Real r);
	Real &operator+=(SInt r);
	Real &operator-=(SInt r);
	Real &operator*=(SInt r);
	Real &operator/=(SInt r);

	bool operator ==(Real r) const;
	bool operator !=(Real r) const;
	bool operator  <(Real r) const;
	bool operator  >(Real r) const;
	bool operator <=(Real r) const;
	bool operator >=(Real r) const;

	float ToFloat( void ) const;
	UInt  ToBits( void )  const;

	static constexpr UInt Precision( void ) { return UInt(S_PREC); }

	friend Real Frac(Real r);
	friend SInt Trunc(Real r);
	friend SInt Round(Real r);
	friend SInt Ceil(Real r);
	friend SInt Floor(Real r);
};

Real operator +(Real l, Real r);
Real operator -(Real l, Real r);
Real operator *(Real l, Real r);
Real operator /(Real l, Real r);
Real operator +(Real l, SInt r);
Real operator -(Real l, SInt r);
Real operator *(Real l, SInt r);
Real operator /(Real l, SInt r);
Real operator +(SInt l, Real r);
Real operator -(SInt l, Real r);
Real operator *(SInt l, Real r);
Real operator /(SInt l, Real r);
bool operator==(SInt l, Real r);
bool operator==(Real l, SInt r);
bool operator!=(SInt l, Real r);
bool operator!=(Real l, SInt r);
bool operator <(SInt l, Real r);
bool operator <(Real l, SInt r);
bool operator >(SInt l, Real r);
bool operator >(Real l, SInt r);
bool operator<=(SInt l, Real r);
bool operator<=(Real l, SInt r);
bool operator>=(SInt l, Real r);
bool operator>=(Real l, SInt r);*/

/*template < typename type_t = Real >
Real Wrap(Real min, Real x, Real max)
{
	SXInt smin    = SXInt(*reinterpret_cast<SInt*>(&min));
	SXInt y       = SXInt(*reinterpret_cast<SInt*>(&x));
	SXInt smax    = SXInt(*reinterpret_cast<SInt*>(&max));
	SXInt span    = smax - smin;
	SInt  wrapped = SInt(smin + (y + (tiny3d::Abs(y) * span) - smin) % span);
	return *reinterpret_cast<Real*>(&wrapped);
}*/

// @algo Pi
// @out Pi
Real  Pi( void );
	
// @algo Tau
// @out 2*pi
Real  Tau( void );
	
// @algo Sin
// @in rad -> Radians.
// @out The sine.
Real  Sin(Real rad);
	
// @algo Cos
// @in rad -> Radians.
// @out The cosine.
Real  Cos(Real rad);
	
// @algo Sqrt
// @in x -> A value.
// @out The square root of x.
Real  Sqrt(Real x);
	
// @algo Frac
// @in r -> A value.
// @out The fractional part of r.
Real  Frac(Real r);
	
// @algo Trunc
// @info Rounds a value by truncating it.
// @in r -> A value.
// @out The trancated value.
SInt  Trunc(Real r);
	
// @algo Round
// @info Rounds a value.
// @in r -> A value.
// @out The rounded value.
SInt  Round(Real r);
	
// @algo Ceil
// @input Rounds a value to the next nearest whole integer.
// @in r -> A value.
// @out The rounded value.
SInt  Ceil(Real r);
	
// @algo Floor
// @info Rounds a value to the previous nearest while integer.
// @in r -> A value.
// @out The rounded value.
SInt  Floor(Real r);
	
// @algo Lerp
// @info Linearly interpolates a value.
// @in
//   a, b -> The end points.
//   x -> The interpolant.
// @out The interpolated value.
template < typename type_t > type_t Lerp(const type_t &a, const type_t &b, Real x) { return a + (b - a) * x; }

// @algo Bilerp
// @info Does bilinear interpolation between four values.
// @in
//  aa, ab -> Upper-left and upper-right end points.
//  ba, bb -> Lower-left and lower-right end points.
//  x -> Horizontal interpolant.
//  y -> Vertical interpolant.
// @out The interpolated value.
template < typename type_t > type_t Bilerp(const type_t &aa, const type_t &ab, const type_t &ba, const type_t &bb, Real x, Real y)
{
	return Lerp(
		Lerp(aa, ab, x),
		Lerp(ba, bb, x),
		y
	);
}

/*class XReal
{
private:
	typedef SXInt real_t;
	static constexpr real_t S_INF_BIT  = std::numeric_limits<real_t>::max();
	static constexpr real_t S_NINF_BIT = std::numeric_limits<real_t>::min() + 1;
	static constexpr real_t S_NAN_BIT  = std::numeric_limits<real_t>::min();
	static constexpr real_t S_PREC     = (CHAR_BIT * sizeof(real_t)) / 2;
	static constexpr real_t S_DEC_STEP = 1;
	static constexpr real_t S_INT_STEP = real_t(1) << S_PREC;

private:
	real_t x;

public:
	XReal( void );
	XReal(const XReal &r);
	explicit XReal(SInt i);
	explicit XReal(Real r);
	explicit operator SInt( void ) const;
	explicit operator Real( void ) const;

	static XReal Inf( void );
	static XReal NInf( void );
	static XReal NaN( void );
	static XReal SmallestInt( void );
	static XReal SmallestFrac( void );
	static XReal Max( void );
	static XReal Min( void );
	bool IsNaN( void )  const;
	bool IsInf( void )  const;
	bool IsNInf( void ) const;

	XReal operator-( void ) const;
	XReal &operator+=(XReal r);
	XReal &operator-=(XReal r);
	XReal &operator*=(XReal r);
	XReal &operator+=(SInt r);
	XReal &operator-=(SInt r);
	XReal &operator*=(SInt r);
	XReal &operator/=(SInt r);

	bool operator ==(XReal r) const;
	bool operator !=(XReal r) const;
	bool operator  <(XReal r) const;
	bool operator  >(XReal r) const;
	bool operator <=(XReal r) const;
	bool operator >=(XReal r) const;

	UXInt ToBits( void )  const;

	static constexpr UInt Precision( void ) { return UInt(S_PREC); }
};

XReal operator +(XReal l, XReal r);
XReal operator -(XReal l, XReal r);
XReal operator *(XReal l, XReal r);
XReal operator +(XReal l, SInt  r);
XReal operator -(XReal l, SInt  r);
XReal operator *(XReal l, SInt  r);
XReal operator /(XReal l, SInt  r);
XReal operator +(SInt  l, XReal r);
XReal operator -(SInt  l, XReal r);
XReal operator *(SInt  l, XReal r);
bool  operator==(SInt  l, XReal r);
bool  operator==(XReal l, SInt  r);
bool  operator!=(SInt  l, XReal r);
bool  operator!=(XReal l, SInt  r);
bool  operator <(SInt  l, XReal r);
bool  operator <(XReal l, SInt  r);
bool  operator >(SInt  l, XReal r);
bool  operator >(XReal l, SInt  r);
bool  operator<=(SInt  l, XReal r);
bool  operator<=(XReal l, SInt  r);
bool  operator>=(SInt  l, XReal r);
bool  operator>=(XReal l, SInt  r);*/

// @data Vector3
// @info A 3D vector.
class Vector3
{
public:
	Real x, y, z;

public:
	Vector3( void );
	Vector3(Real _x, Real _y, Real _z);
	Vector3(const Vector3 &r) = default;
	Vector3 &operator=(const Vector3 &r) = default;

	Vector3 &operator+=(const Vector3 &r);
	Vector3 &operator-=(const Vector3 &r);
	Vector3 &operator*=(const Vector3 &r);
	Vector3 &operator/=(const Vector3 &r);
	Vector3 &operator*=(Real r);
	Vector3 &operator/=(Real r);
	Vector3 &operator*=(SInt r);
	Vector3 &operator/=(SInt r);

	// @algo IsNormal
	// @out TRUE if vector has unit length.
	bool IsNormal( void ) const;

	Real &operator[](UInt i);
	const Real &operator[](UInt i) const;
};

Vector3 operator+(Vector3 l, const Vector3 &r);
Vector3 operator-(Vector3 l, const Vector3 &r);
Vector3 operator*(Vector3 l, const Vector3 &r);
Vector3 operator/(Vector3 l, const Vector3 &r);
Vector3 operator*(Vector3 l, Real r);
Vector3 operator/(Vector3 l, Real r);
Vector3 operator-(const Vector3 &u);
Vector3 operator*(Vector3 l, SInt r);
Vector3 operator/(Vector3 l, SInt r);

// @algo Min
// @info Returns the minimum individual values between two input vectors.
// @in a, b -> Input vectors.
// @out The minimum values.
Vector3 Min(const Vector3 &a, const Vector3 &b);
	
// @algo Max
// @info Returns the maximum individual values between two input vectors.
// @in a, b -> Input vectors.
// @out The maximum values.
Vector3 Max(const Vector3 &a, const Vector3 &b);
	
// @algo Dot
// @info The dot product between two vectors.
// @in l, r -> Left and right hand side vectors.
// @out The dot product.
Real    Dot(const Vector3 &l, const Vector3 &r);
	
// @algo Cross
// @info The cross product between two vectors.
// @in l, r -> Left and right hand side vectors.
// @out The cross product.
Vector3 Cross(const Vector3 &l, const Vector3 &r);
	
// @algo Len
// @in The vector to retrieve the length from.
// @out The length of the vector.
Real    Len(const Vector3 &v);
	
// @algo Normalize
// @in The vector to normalize.
// @out The normalized vector.
Vector3 Normalize(const Vector3 &v);
	
// @algo Area
// @info The area defined by a triangle.
// @in a, b, c -> The triangle vertices.
// @out The area.
Real    Area(const Vector3 &a, const Vector3 &b, const Vector3 &c);

// @data Vector2
// @info A 2D vector.
class Vector2
{
public:
	Real x, y;

public:
	Vector2( void );
	Vector2(Real _x, Real _y);
	Vector2(const Vector2 &r) = default;
	Vector2 &operator=(const Vector2 &r) = default;

	Vector2 &operator+=(const Vector2 &r);
	Vector2 &operator-=(const Vector2 &r);
	Vector2 &operator*=(const Vector2 &r);
	Vector2 &operator/=(const Vector2 &r);
	Vector2 &operator*=(Real r);
	Vector2 &operator/=(Real r);
	Vector2 &operator*=(SInt r);
	Vector2 &operator/=(SInt r);

	// @algo IsNormal
	// @out TRUE if vector has unit length.
	bool IsNormal( void ) const;

	Real &operator[](UInt i);
	const Real &operator[](UInt i) const;
};

Vector2 operator+(Vector2 l, const Vector2 &r);
Vector2 operator-(Vector2 l, const Vector2 &r);
Vector2 operator*(Vector2 l, const Vector2 &r);
Vector2 operator/(Vector2 l, const Vector2 &r);
Vector2 operator*(Vector2 l, Real r);
Vector2 operator/(Vector2 l, Real r);
Vector2 operator-(const Vector2 &u);
Vector2 operator*(Vector2 l, SInt r);
Vector2 operator/(Vector2 l, SInt r);

// @algo Min
// @info Returns the minimum individual values between two input vectors.
// @in a, b -> Input vectors.
// @out The minimum values.
Vector2 Min(const Vector2 &a, const Vector2 &b);
	
// @algo Max
// @info Returns the maximum individual values between two input vectors.
// @in a, b -> Input vectors.
// @out The maximum values.
Vector2 Max(const Vector2 &a, const Vector2 &b);
	
// @algo Dot
// @info The dot product between two vectors.
// @in l, r -> Left and right hand side vectors.
// @out The dot product.
Real    Dot(const Vector2 &l, const Vector2 &r);
	
// @algo Len
// @in The vector to retrieve the length from.
// @out The length of the vector.
Real    Len(const Vector2 &v);
	
// @algo Normalize
// @in The vector to normalize.
// @out The normalized vector.
Vector2 Normalize(const Vector2 &v);
	
// @algo Area
// @info The area defined by a triangle.
// @in a, b, c -> The triangle vertices.
// @out The area.
Real    Area(const Vector2 &a, const Vector2 &b, const Vector2 &c);

// @data Matrix3x3
// @info A 3D matrix.
class Matrix3x3
{
public:
	Vector3 x, y, z;

public:
	Matrix3x3( void );
	Matrix3x3(const Vector3 &_x, const Vector3 &_y, const Vector3 &_z);
	Matrix3x3(Real xx, Real xy, Real xz, Real yx, Real yy, Real yz, Real zx, Real zy, Real zz);
	Matrix3x3(const Matrix3x3 &m) = default;
	Matrix3x3 &operator=(const Matrix3x3 &m) = default;

	Vector3 &operator[](UInt row);
	const Vector3 &operator[](UInt row) const;
};

// @algo Identity3
// @out The identity matrix.
Matrix3x3 Identity3( void );
	
// @algo Transp
// @in m -> A matrix.
// @out The transposed matrix.
Matrix3x3 Transp(const Matrix3x3 &m);
	
// @algo Inv
// @info Returns inverse matrix. Not all matrices are invertible.
// @in m -> A matrix.
// @out The invertex matrix.
Matrix3x3 Inv(const Matrix3x3 &m);
	
// @algo Euler
// @info Returns an Euler rotation matrix.
// @in head, pitch, roll -> Angles in radians.
// @out The rotation matrix.
Matrix3x3 Euler(Real head, Real pitch, Real roll);
	
// @algo AxisAngle
// @info Returns a rotation matrix based on an axis and an angle.
// @in
//   axis -> The axis to rotate about.
//   angle -> The angle to rotate in radians.
// @out The rotation matrix.
Matrix3x3 AxisAngle(Vector3 axis, Real angle);
	
// @algo RotateX
// @in angle -> The angle in radians.
// @out The rotation matrix.
Matrix3x3 RotateX(Real angle);
	
// @algo RotateY
// @in angle -> The angle in radians.
// @out The rotation matrix.
Matrix3x3 RotateY(Real angle);
	
// @algo RotateZ
// @in angle -> The angle in radians.
// @out The rotation matrix.
Matrix3x3 RotateZ(Real angle);

Matrix3x3 operator*(const Matrix3x3 &l, Matrix3x3 r);
Matrix3x3 operator*(const Matrix3x3 &l, Real r);
Vector3   operator*(const Vector3 &l, const Matrix3x3 &r);

// @data Matrix2x2
// @info A 2D matrix.
class Matrix2x2
{
public:
	Vector2 x, y;

public:
	Matrix2x2( void );
	Matrix2x2(const Vector2 &_x, const Vector2 &_y);
	Matrix2x2(Real xx, Real xy, Real yx, Real yy);
	Matrix2x2(const Matrix2x2 &m) = default;
	Matrix2x2 &operator=(const Matrix2x2 &m) = default;

	Vector2 &operator[](UInt row);
	const Vector2 &operator[](UInt row) const;
};

// @algo Identity2
// @out The identity matrix.
Matrix2x2 Identity2( void );
	
// @algo Transp
// @in m -> A matrix.
// @out The transposed matrix.
Matrix2x2 Transp(const Matrix2x2 &m);
	
// @algo Inv
// @info Returns inverse matrix. Not all matrices are invertible.
// @in m -> A matrix.
// @out The invertex matrix.
Matrix2x2 Inv(const Matrix2x2 &m);
	
// @algo Rotate2
// @in angle -> The angle in radians.
// @out The rotation matrix.
Matrix2x2 Rotate2(Real angle);

Matrix2x2 operator*(const Matrix2x2 &l, Matrix2x2 r);
Matrix2x2 operator*(const Matrix2x2 &l, Real r);
Vector2   operator*(const Vector2 &l, const Matrix2x2 &r);

// @data Matrix4x4
// @info A 4D matrix.
class Matrix4x4
{
private:
	Real m[4][4];

private:
	static Real Dot(const Real *l, const Real *r);

public:
	Matrix4x4( void );
	Matrix4x4(Real xx, Real xy, Real xz, Real xw, Real yx, Real yy, Real yz, Real yw, Real zx, Real zy, Real zz, Real zw, Real wx, Real wy, Real wz, Real ww);
	Matrix4x4(const Matrix4x4 &m) = default;
	Matrix4x4 &operator=(const Matrix4x4 &m) = default;

	Real *operator[](UInt row);
	const Real *operator[](UInt row) const;

	Matrix4x4 operator*(Matrix4x4 r) const;
	Matrix4x4 operator*(Real r) const;
};

// @algo Identity4
// @out The identity matrix.
Matrix4x4 Identity4( void );
	
// @algo Transp
// @in m -> A matrix.
// @out The transposed matrix.
Matrix4x4 Transp(const Matrix4x4 &m);
	
// @algo Transform
// @info Constructs a transform given a basis and a position.
// @in
//   rot -> The basis of the transform.
//   pos -> The translation of the transform.
// @out The transform matrix.
Matrix4x4 Transform(const Matrix3x3 &rot, const Vector3 &pos);
	
// @algo Rotation
// @info Returns the basis of the transform.
// @in transform -> The transform.
// @out The basis.
Matrix3x3 Rotation(const Matrix4x4 &transform);
	
// @algo Translation
// @info Returns the translation (position) of the transform.
// @in transform -> The transform.
// @out The translation.
Vector3   Translation(const Matrix4x4 &transform);
	
// @algo Inv
// @info Returns inverse matrix. Not all matrices are invertible.
// @in m -> A matrix.
// @out The invertex matrix.
Matrix4x4 Inv(const Matrix4x4 &m);

Vector3 operator*(const Vector3 &l, const Matrix4x4 &r);

}

#endif // TINY_MATH_H
