#ifndef TINY_SIMD_H
#define TINY_SIMD_H

#define TINY_COMPILER_UNKNOWN 0
#define TINY_COMPILER_MSVC    1
#define TINY_COMPILER_GCC     2

#define TINY_SIMD_NONE    0
#define TINY_SIMD_SSE     1
#define TINY_SIMD_AVX256  2
#define TINY_SIMD_AVX512  3
#define TINY_SIMD_NEON    4
#define TINY_SIMD_ALTIVEC 5

// add support for AVX-256 and AVX-512 (#include <immintrin.h> on gcc and msvc)
// low priority: add support for PowerPC Altivec (#include <altivec.h> on gcc)
	// need a computer to test it on

// Detect instruction set
#if defined(__GNUC__)
	#define TINY_COMPILER TINY_COMPILER_GCC
	#ifdef TINY_FALLBACK_SCALAR
		#define TINY_SIMD TINY_SIMD_NONE
	#elif defined(__AVX__)
		#define TINY_SIMD TINY_SIMD_AVX256
		#ifdef __AVX2__
			#define TINY_SIMD_VER 2
		#endif
	#elif defined(__AVX512__)
		#define TINY_SIMD TINY_SIMD_AVX512
	#elif defined(__SSE__)
		#define TINY_SIMD  TINY_SIMD_SSE
		#ifdef __SSE4__
			#define TINY_SIMD_VER 4
		#elif defined(__SSE3__)
			#define TINY_SIMD_VER 3
		#elif defined(__SSE2__)
			#define TINY_SIMD_VER 2
		#endif
	#elif defined(__ARM_NEON__)
		// Enable on Raspberry Pi 2/3 using these compiler flags:
			// -mcpu=cortex-a7
			// -mfloat-abi=hard
			// -mfpu=neon-vfpv4
		#define TINY_SIMD TINY_SIMD_NEON
	#elif defined(__VEC__) && defined(__ALTIVEC__)
		// Use the "-maltivec" flag to enable PowerPC AltiVec support
		#define TINY_SIMD TINY_SIMD_ALTIVEC
	#else
		#define TINY_SIMD TINY_SIMD_NONE
		#warning No SIMD support, falling back to scalar
	#endif
#elif defined(_MSC_VER)
	#define TINY_COMPILER TINY_COMPILER_MSVC
	#ifdef TINY_FALLBACK_SCALAR
		#define TINY_SIMD TINY_SIMD_NONE
	#elif !defined(_M_CEE_PURE)
		#define TINY_SIMD TINY_SSE
	#else
		#define TINY_SIMD TINY_SIMD_NONE
		#warning No SIMD support, falling back to scalar
	#endif
#else
	#define TINY_COMPILER TINY_COMPILER_UNKNOWN
	#define TINY_SIMD     TINY_SIMD_NONE
	#warning No SIMD support, falling back to scalar
#endif

#ifndef TINY_SIMD_VER
	#define TINY_SIMD_VER 1
#endif

// SIMD instruction definitions
#if TINY_SIMD == TINY_SIMD_SSE
	#define TINY_WIDTH         4
	#define TINY_BYTE_ALIGN    16
	#define TINY_BLOCK_X       2
	#define TINY_BLOCK_Y       2
	#define TINY_NO_OFFSETS    { 0, 0, 0, 0 }
	#define TINY_OFFSETS       { 0, 1, 2, 3 }
	#define TINY_X_OFFSETS     { 0, 1, 0, 1 }
	#define TINY_Y_OFFSETS     { 0, 0, 1, 1 }
#elif TINY_SIMD == TINY_SIMD_NEON
	#define TINY_WIDTH         4
	#define TINY_BYTE_ALIGN    16
	#define TINY_BLOCK_X       2
	#define TINY_BLOCK_Y       2
	#define TINY_NO_OFFSETS    { 0, 0, 0, 0 }
	#define TINY_OFFSETS       { 0, 1, 2, 3 }
	#define TINY_X_OFFSETS     { 0, 1, 0, 1 }
	#define TINY_Y_OFFSETS     { 0, 0, 1, 1 }
#elif TINY_SIMD == TINY_SIMD_AVX256
	#define TINY_WIDTH         8
	#define TINY_BYTE_ALIGN    32
	#define TINY_BLOCK_X       4
	#define TINY_BLOCK_Y       2
	#define TINY_NO_OFFSETS    { 0, 0, 0, 0, 0, 0, 0, 0 }
	#define TINY_OFFSETS       { 0, 1, 2, 3, 4, 5, 6, 7 }
	#define TINY_X_OFFSETS     { 0, 1, 2, 3, 0, 1, 2, 3 }
	#define TINY_Y_OFFSETS     { 0, 0, 0, 0, 1, 1, 1, 1 }
#elif TINY_SIMD == TINY_SIMD_AVX512
	#define TINY_WIDTH         16
	#define TINY_BYTE_ALIGN    64
	#define TINY_BLOCK_X       4
	#define TINY_BLOCK_Y       4
	#define TINY_NO_OFFSETS    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0 }
	#define TINY_OFFSETS       { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }
	#define TINY_X_OFFSETS     { 0, 1, 2, 3, 0, 1, 2, 3, 0, 1,  2,  3,  0,  1,  2,  3 }
	#define TINY_Y_OFFSETS     { 0, 0, 0, 0, 1, 1, 1, 1, 2, 2,  2,  2,  3,  3,  3,  3 }
#elif TINY_SIMD == TINY_SIMD_ALTIVEC
	#define TINY_WIDTH         4
	#define TINY_BYTE_ALIGN    16
	#define TINY_BLOCK_X       2
	#define TINY_BLOCK_Y       2
	#define TINY_NO_OFFSETS    { 0, 0, 0, 0 }
	#define TINY_OFFSETS       { 0, 1, 2, 3 }
	#define TINY_X_OFFSETS     { 0, 1, 0, 1 }
	#define TINY_Y_OFFSETS     { 0, 0, 1, 1 }
#elif TINY_SIMD == TINY_SIMD_NONE
	#define TINY_WIDTH         1
	#define TINY_BYTE_ALIGN    1
	#define TINY_BLOCK_X       1
	#define TINY_BLOCK_Y       1
	#define TINY_NO_OFFSETS    { 0 }
	#define TINY_OFFSETS       { 0 }
	#define TINY_X_OFFSETS     TINY_OFFSETS
	#define TINY_Y_OFFSETS     TINY_OFFSETS
#endif

// Common defines
#define TINY_BYTE_ALIGN_MASK (~(TINY_BYTE_ALIGN - 1))
#define TINY_WIDTH_MASK      (TINY_WIDTH - 1)
#define TINY_WIDTH_INVMASK   (~TINY_WIDTH_MASK)
#define TINY_BLOCK_X_MASK    (TINY_BLOCK_X - 1)
#define TINY_BLOCK_Y_MASK    (TINY_BLOCK_Y - 1)
#define TINY_BLOCK_X_INVMASK (~TINY_BLOCK_X_MASK)
#define TINY_BLOCK_Y_INVMASK (~TINY_BLOCK_Y_MASK)
#define TINY_FLOOR(X)        ((X) & TINY_WIDTH_INVMASK)
#define TINY_CEIL(X)         TINY_FLOOR((X) + TINY_WIDTH_MASK)

// Include appropriate headers
// Some notes,
// Catchall include for MSVC is "#include <intrin.h>"
// Catchall include for GCC is "#include <x86intrin.h>"
#if TINY_SIMD == TINY_SIMD_AVX256 || TINY_SIMD == TINY_SIMD_AVX512
	#include <immintrin.h>
#elif TINY_SIMD == TINY_SIMD_SSE
	#include <xmmintrin.h>
#elif TINY_SIMD == TINY_SIMD_NEON
	#include <arm_neon.h>
#elif TINY_SIMD == TINY_SIMD_ALTIVEC
	#include <altivec.h>
	// undef these to avoid future collisions
	#undef vector
	#undef pixel
	#undef bool
#endif

#if ((-2 >> 1) == (-2 / 2)) && ((-2 << 1) == (-2 * 2))
	#define TINY_SIGNED_SHIFT 1
#endif

#define TINY_UNS_MAX (~0U)
#define TINY_TRUE_BITS TINY_UNS_MAX
#define TINY_FALSE_BITS 0


namespace tiny3d
{

class WideReal;
class WideSInt;

#if TINY_SIMD == TINY_SIMD_SSE

#if TINY_COMPILER == TINY_COMPILER_MSVC
	__declspec(align(TINY_BYTE_ALIGN))
#endif
union WideBool
{
	friend class WideReal;
	friend class WideSInt;
	template < int n >
	friend class wide_fixed;

private:
	__m128  f;
	__m128i u;

private:
	WideBool(const __m128i &r) : u(r) {}
	WideBool(const __m128 &r) : f(r) {}

public:
	typedef bool serial_t;

	WideBool( void ) {}
	WideBool(const WideBool &b) : f(b.f) {}
	WideBool(bool b) : u(_mm_set1_epi32(b ? int(TINY_TRUE_BITS) : int(TINY_FALSE_BITS))) {}
	WideBool(const bool *b) : u(_mm_setr_epi32(b[0] ? int(TINY_TRUE_BITS) : int(TINY_FALSE_BITS), b[1] ? int(TINY_TRUE_BITS) : int(TINY_FALSE_BITS), b[2] ? int(TINY_TRUE_BITS) : int(TINY_FALSE_BITS), b[3] ? int(TINY_TRUE_BITS) : int(TINY_FALSE_BITS))) {}

	WideBool operator||(const WideBool &r) const { return _mm_or_si128(u, r.u);  }
	WideBool operator&&(const WideBool &r) const { return _mm_and_si128(u, r.u); }

	WideBool operator|(const WideBool &r) const { return _mm_or_si128(u, r.u);  }
	WideBool operator&(const WideBool &r) const { return _mm_and_si128(u, r.u); }

	WideBool operator!( void ) const { return _mm_xor_si128(u, _mm_set1_epi32(int(TINY_TRUE_BITS))); }

	bool all_fail( void ) const { return _mm_movemask_epi8(u) == 0x0000; }
	bool all_pass( void ) const { return _mm_movemask_epi8(u) == 0xffff; }
}
#if TINY_COMPILER == TINY_COMPILER_GCC
	__attribute__((aligned(TINY_BYTE_ALIGN)))
#endif
;

#if TINY_COMPILER == TINY_COMPILER_MSVC
	__declspec(align(TINY_BYTE_ALIGN))
#endif
class WideReal
{
	friend class WideSInt;
	template < int n >
	friend class wide_fixed;

private:
	__m128 f;

private:
	WideReal(const __m128 &r) : f(r) {}

public:
	typedef float serial_t;

	WideReal( void ) {}
	WideReal(const WideReal &r) : f(r.f) {}
	WideReal(float val) : f(_mm_set1_ps(val)) {}
	WideReal(bool val) : f(_mm_set1_ps(val ? 1.0f : -1.0f)) {}
	explicit WideReal(const float *in) : f(_mm_loadu_ps(in)) {}
	inline explicit WideReal(const WideSInt &r);
	inline explicit WideReal(const WideBool &r);

	WideReal operator-( void ) const { return WideReal(0.0f) - *this; }

	WideReal &operator+=(const WideReal &r) { f = _mm_add_ps(f, r.f); return *this; }
	WideReal &operator-=(const WideReal &r) { f = _mm_sub_ps(f, r.f); return *this; }
	WideReal &operator*=(const WideReal &r) { f = _mm_mul_ps(f, r.f); return *this; }
	WideReal &operator/=(const WideReal &r) { f = _mm_div_ps(f, r.f); return *this; }
	WideReal &operator|=(const WideReal &r) { f = _mm_or_ps(f, r.f);  return *this; }
	WideReal &operator|=(const WideBool &r)  { f = _mm_or_ps(f, r.f);  return *this; }
	WideReal &operator&=(const WideReal &r) { f = _mm_and_ps(f, r.f); return *this; }
	WideReal &operator&=(const WideBool &r)  { f = _mm_and_ps(f, r.f); return *this; }

	WideReal operator+(const WideReal &r) const { return _mm_add_ps(f, r.f); }
	WideReal operator-(const WideReal &r) const { return _mm_sub_ps(f, r.f); }
	WideReal operator*(const WideReal &r) const { return _mm_mul_ps(f, r.f); }
	WideReal operator/(const WideReal &r) const { return _mm_div_ps(f, r.f); }
	WideReal operator|(const WideReal &r) const { return _mm_or_ps(f, r.f);  }
	WideReal operator|(const WideBool &r)  const { return _mm_or_ps(f, r.f);  }
	WideReal operator&(const WideReal &r) const { return _mm_and_ps(f, r.f); }
	WideReal operator&(const WideBool &r)  const { return _mm_and_ps(f, r.f); }

	WideBool operator==(const WideReal &r) const { return _mm_cmpeq_ps(f, r.f);  }
	WideBool operator!=(const WideReal &r) const { return _mm_cmpneq_ps(f, r.f); }
	WideBool operator< (const WideReal &r) const { return _mm_cmplt_ps(f, r.f);  }
	WideBool operator<=(const WideReal &r) const { return _mm_cmple_ps(f, r.f);  }
	WideBool operator> (const WideReal &r) const { return _mm_cmpgt_ps(f, r.f);  }
	WideBool operator>=(const WideReal &r) const { return _mm_cmpge_ps(f, r.f);  }

	void to_scalar(float *out) const { _mm_storeu_ps(out, f); }

	static WideReal mov_if_true(const WideReal &l, const WideReal &r, const WideBool &cond_mask)
	{
#if TINY_SIMD_VER < 2
		WideBool l_mask;
		l_mask.u = _mm_andnot_si128(cond_mask.u, _mm_set1_epi32(TINY_UNS_MAX));
		__m128 rc, lc;
		lc = _mm_and_ps(l.f, l_mask.f);
		rc = _mm_and_ps(r.f, cond_mask.f);
		rc = _mm_or_ps(rc, lc);
		return rc;
#else
		__m128 ret = l.f;
		_mm_maskmoveu_si128(*reinterpret_cast<const __m128i*>(&r), cond_mask.u, reinterpret_cast<char*>(&ret));
		return ret;
#endif
	}
	static WideReal max(const WideReal &a, const WideReal &b) { return _mm_max_ps(a.f, b.f); }
	static WideReal min(const WideReal &a, const WideReal &b) { return _mm_min_ps(a.f, b.f); }
	static WideReal sqrt(const WideReal &x)                     { return _mm_sqrt_ps(x.f); }
}
#if TINY_COMPILER == TINY_COMPILER_GCC
	__attribute__((aligned(TINY_BYTE_ALIGN)))
#endif
;

#if TINY_COMPILER == TINY_COMPILER_MSVC
	__declspec(align(TINY_BYTE_ALIGN))
#endif
template < int n >
class wide_fixed
{
	friend class WideSInt;

private:
	__m128i i;

private:
	wide_fixed(const __m128i &r) : i(r) {}

public:
#ifdef MML_FIXED_H_INCLUDED__
	typedef mml::fixed<int, n> serial_t;
#else
	typedef int serial_t;
#endif

	wide_fixed( void ) {}
	wide_fixed(const wide_fixed<n> &r) : i(r.i) {}
	wide_fixed(int val) : i(_mm_slli_epi32(_mm_set1_epi32(val), n)) {}
	wide_fixed(float val) : i(_mm_cvttps_epi32(_mm_mul_ps(_mm_set1_ps(val), _mm_set1_ps(1 << n)))) {}
	wide_fixed(bool val) : i(_mm_slli_epi32(_mm_set1_epi32(val ? 1 : 0), n)) {}
	explicit wide_fixed(const int *in) : i(_mm_slli_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(in)), n)) {}
	explicit wide_fixed(const float *in) : i(_mm_cvttps_epi32(_mm_mul_ps(_mm_loadu_ps(in), _mm_set1_ps(1 << n)))) {}
	inline explicit wide_fixed(const WideSInt &r);
	inline explicit wide_fixed(const WideReal &r);
	inline explicit wide_fixed(const WideBool &r);

	wide_fixed<n> operator-( void ) const { return wide_fixed<n>(0) - *this; }

	wide_fixed<n> &operator+=(const wide_fixed<n> &r) { i = _mm_add_epi32(i, r.i); return *this; }
	wide_fixed<n> &operator-=(const wide_fixed<n> &r) { i = _mm_sub_epi32(i, r.i); return *this; }
	wide_fixed<n> &operator*=(const wide_fixed<n> &r) {
		__m128i tmp1 = _mm_mul_epu32(i, r.i);
		__m128i tmp2 = _mm_mul_epu32( _mm_srli_si128(i, 4), _mm_srli_si128(r.i, 4));
		i = _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE (0,0,2,0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE (0,0,2,0)));
		i = _mm_srai_epi32(i, n);
		return *this;
	}
	wide_fixed<n> &operator|=(const wide_fixed<n> &r) { i = _mm_or_si128(i, r.i); return *this; }
	wide_fixed<n> &operator|=(const WideBool &r)     { i = _mm_or_si128(i, r.u); return *this; }
	wide_fixed<n> &operator&=(const wide_fixed<n> &r) { i = _mm_and_si128(i, r.i); return *this; }
	wide_fixed<n> &operator&=(const WideBool &r)     { i = _mm_and_si128(i, r.u); return *this; }

	wide_fixed<n> operator+(const wide_fixed<n> &r) const { return _mm_add_epi32(i, r.i); }
	wide_fixed<n> operator-(const wide_fixed<n> &r) const { return _mm_sub_epi32(i, r.i); }
	wide_fixed<n> operator*(const wide_fixed<n> &r) const { wide_fixed<n> o; o.i = i; o *= r; return o; }
	wide_fixed<n> operator|(const wide_fixed<n> &r) const { return _mm_or_si128(i, r.i);  }
	wide_fixed<n> operator|(const WideBool &r)     const { return _mm_or_si128(i, r.u);  }
	wide_fixed<n> operator&(const wide_fixed<n> &r) const { return _mm_and_si128(i, r.i); }
	wide_fixed<n> operator&(const WideBool &r)     const { return _mm_and_si128(i, r.u); }

	WideBool operator==(const wide_fixed<n> &r) const { return _mm_cmpeq_epi32(i, r.i); }
	WideBool operator!=(const wide_fixed<n> &r) const { return _mm_andnot_si128(_mm_cmpeq_epi32(i, r.i), _mm_set1_epi32(TINY_TRUE_BITS)); }
	WideBool operator< (const wide_fixed<n> &r) const { return _mm_cmplt_epi32(i, r.i); }
	WideBool operator<=(const wide_fixed<n> &r) const { return _mm_andnot_si128(_mm_cmpgt_epi32(i, r.i), _mm_set1_epi32(TINY_TRUE_BITS)); }
	WideBool operator> (const wide_fixed<n> &r) const { return _mm_cmpgt_epi32(i, r.i); }
	WideBool operator>=(const wide_fixed<n> &r) const { return _mm_andnot_si128(_mm_cmplt_epi32(i, r.i), _mm_set1_epi32(TINY_TRUE_BITS)); }

	void to_scalar(int *out) const { _mm_storeu_si128(reinterpret_cast<__m128i*>(out), _mm_srai_epi32(i, n)); }

	static wide_fixed<n> mov_if_true(const wide_fixed<n> &l, const wide_fixed<n> &r, const WideBool &cond_mask)
	{
#if TINY_SIMD_VER < 2
		__m128i l_mask;
		l_mask = _mm_andnot_si128(cond_mask.u, _mm_set1_epi32(TINY_UNS_MAX));
		__m128i rc, lc;
		lc = _mm_and_si128(l.i, l_mask);
		rc = _mm_and_si128(r.i, cond_mask.u);
		rc = _mm_or_si128(rc, lc);
		return rc;
#else
		__m128i ret = l.i;
		_mm_maskmoveu_si128(r.i, cond_mask.u, reinterpret_cast<char*>(&ret));
		return ret;
#endif
	}
	static wide_fixed<n> max(const wide_fixed<n> &a, const wide_fixed<n> &b)
	{
#if TINY_SIMD_VER < 4
		return wide_fixed<n>::mov_if_true(a, b, (a < b));
#else
		return _mm_max_epi32(a.i, b.i);
#endif
	}
	static wide_fixed<n> min(const wide_fixed<n> &a, const wide_fixed<n> &b)
	{
#if TINY_SIMD_VER < 4
		return wide_fixed<n>::mov_if_true(a, b, (a > b));
#else
		return _mm_min_epi32(a.i, b.i);
#endif
	}
}
#if TINY_COMPILER == TINY_COMPILER_GCC
	__attribute__((aligned(TINY_BYTE_ALIGN)))
#endif
;

#if TINY_COMPILER == TINY_COMPILER_MSVC
	__declspec(align(TINY_BYTE_ALIGN))
#endif
class WideSInt
{
	friend class WideReal;
	template < int n >
	friend class wide_fixed;

private:
	__m128i i;

private:
	WideSInt(const __m128i &r) : i(r) {}

public:
	typedef int serial_t;

	WideSInt( void ) {}
	WideSInt(const WideSInt &r) : i(r.i) {}
	WideSInt(int val) : i(_mm_set1_epi32(val)) {}
	WideSInt(bool val) : i(_mm_set1_epi32(val ? 1 : 0)) {}
	explicit WideSInt(const int *in) : i(_mm_loadu_si128(reinterpret_cast<const __m128i*>(in))) {}
	template < int n >
	inline explicit WideSInt(const wide_fixed<n> &f);
	inline explicit WideSInt(const WideReal &r);
	inline explicit WideSInt(const WideBool &r);

	WideSInt operator-( void ) const { return WideSInt(0) - *this; }

	WideSInt &operator+=(const WideSInt &r) { i = _mm_add_epi32(i, r.i); return *this; }
	WideSInt &operator-=(const WideSInt &r) { i = _mm_sub_epi32(i, r.i); return *this; }
	WideSInt &operator*=(const WideSInt &r) {
		__m128i tmp1 = _mm_mul_epu32(i, r.i);
		__m128i tmp2 = _mm_mul_epu32( _mm_srli_si128(i, 4), _mm_srli_si128(r.i, 4));
		i = _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE (0,0,2,0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE (0,0,2,0)));
		return *this;
	}
	WideSInt &operator|=(const WideSInt &r)  { i = _mm_or_si128(i, r.i);  return *this; }
	WideSInt &operator|=(const WideBool &r) { i = _mm_or_si128(i, r.u);  return *this; }
	WideSInt &operator&=(const WideSInt &r)  { i = _mm_and_si128(i, r.i); return *this; }
	WideSInt &operator&=(const WideBool &r) { i = _mm_and_si128(i, r.u); return *this; }
	WideSInt &operator<<=(int r) { i = _mm_slli_epi32(i, r); return *this; }
	WideSInt &operator>>=(int r) { i = _mm_srai_epi32(i, r); return *this; }
//		WideSInt &operator/=(const WideSInt &r) {}
//		WideSInt &operator%=(const WideSInt &r) { *this = *this - (*this / r) * r; return *this; }

	WideSInt operator+(const WideSInt &r)  const { return _mm_add_epi32(i, r.i); }
	WideSInt operator-(const WideSInt &r)  const { return _mm_sub_epi32(i, r.i); }
	WideSInt operator*(const WideSInt &r)  const { WideSInt o; o.i = i; o *= r; return o; }
	WideSInt operator|(const WideSInt &r)  const { return _mm_or_si128(i, r.i);  }
	WideSInt operator|(const WideBool &r) const { return _mm_or_si128(i, r.u);  }
	WideSInt operator&(const WideSInt &r)  const { return _mm_and_si128(i, r.i); }
	WideSInt operator&(const WideBool &r) const { return _mm_and_si128(i, r.u); }
	WideSInt operator<<(int r) const { return _mm_slli_epi32(i, r); }
	WideSInt operator>>(int r) const { return _mm_srai_epi32(i, r); }
//		WideSInt &operator/(const WideSInt &r) { WideSInt o = *this; return o /= r; }
//		WideSInt &operator%(const WideSInt &r) { return *this - (*this / r) * r; }

	WideBool operator==(const WideSInt &r) const { return _mm_cmpeq_epi32(i, r.i); }
	WideBool operator!=(const WideSInt &r) const { return _mm_andnot_si128(_mm_cmpeq_epi32(i, r.i), _mm_set1_epi32(int(TINY_TRUE_BITS))); }
	WideBool operator< (const WideSInt &r) const { return _mm_cmplt_epi32(i, r.i); }
	WideBool operator<=(const WideSInt &r) const { return _mm_andnot_si128(_mm_cmpgt_epi32(i, r.i), _mm_set1_epi32(int(TINY_TRUE_BITS))); }
	WideBool operator> (const WideSInt &r) const { return _mm_cmpgt_epi32(i, r.i); }
	WideBool operator>=(const WideSInt &r) const { return _mm_andnot_si128(_mm_cmplt_epi32(i, r.i), _mm_set1_epi32(int(TINY_TRUE_BITS))); }

	void to_scalar(int *out) const { _mm_storeu_si128(reinterpret_cast<__m128i*>(out), i); }

	static WideSInt mov_if_true(const WideSInt &l, const WideSInt &r, const WideBool &cond_mask)
	{
#if TINY_SIMD_VER < 2
		__m128i l_mask;
		l_mask = _mm_andnot_si128(cond_mask.u, _mm_set1_epi32(TINY_UNS_MAX));
		__m128i rc, lc;
		lc = _mm_and_si128(l.i, l_mask);
		rc = _mm_and_si128(r.i, cond_mask.u);
		rc = _mm_or_si128(rc, lc);
		return rc;
#else
		__m128i ret = l.i;
		_mm_maskmoveu_si128(r.i, cond_mask.u, reinterpret_cast<char*>(&ret));
		return ret;
#endif
	}
	static WideSInt max(const WideSInt &a, const WideSInt &b)
	{
#if TINY_SIMD_VER < 4

		return WideSInt::mov_if_true(a, b, (a < b));
#else
		return _mm_max_epi32(a.i, b.i);
#endif
	}
	static WideSInt min(const WideSInt &a, const WideSInt &b)
	{
#if TINY_SIMD_VER < 4
		return WideSInt::mov_if_true(a, b, (a > b));
#else
		return _mm_min_epi32(a.i, b.i);
#endif
	}
}
#if TINY_COMPILER == TINY_COMPILER_GCC
	__attribute__((aligned(TINY_BYTE_ALIGN)))
#endif
;

WideReal::WideReal(const WideSInt &r) : f(_mm_cvtepi32_ps(r.i)) {}
WideReal::WideReal(const WideBool &r) : WideReal(WideReal::mov_if_true(WideReal(1.0f), WideReal(0.0f), r)) {}

template < int n >
wide_fixed<n>::wide_fixed(const WideSInt &r) : i(_mm_slli_epi32(r.i, n)) {}
template < int n >
wide_fixed<n>::wide_fixed(const WideReal &r) : i(_mm_cvttps_epi32(_mm_mul_ps(r.f, _mm_set1_ps(1 << n)))) {}
template < int n >
wide_fixed<n>::wide_fixed(const WideBool &r) : wide_fixed(wide_fixed<n>::mov_if_true(wide_fixed<n>(WideSInt(1)), wide_fixed<n>(WideSInt(0)), r)) {}

WideSInt::WideSInt(const WideReal &r)   : i(_mm_cvttps_epi32(r.f)) {}
template < int n >
WideSInt::WideSInt(const wide_fixed<n> &r) : i(_mm_srai_epi32(r.i, n)) {}
WideSInt::WideSInt(const WideBool &r) : WideSInt(WideSInt::mov_if_true(WideSInt(1), WideSInt(0), r)) {}

#elif TINY_SIMD == TINY_SIMD_AVX256

// __m256, __m256i

#elif TINY_SIMD == TINY_SIMD_AVX512

// __m512, __m512i

#elif TINY_SIMD == TINY_SIMD_NEON && TINY_COMPILER == TINY_COMPILER_GCC

union WideBool
{
	friend class WideReal;
	friend class WideSInt;
	template < int n >
	friend class wide_fixed;

private:
	uint32x4_t  u;
	float32x4_t f;

private:
	WideBool(const uint32x4_t &r) : u(r) {}
	WideBool(const float32x4_t &r) : f(r) {}

public:
	typedef bool serial_t;

	WideBool( void ) {}
	WideBool(const WideBool &b) : f(b.f) {}
	WideBool(bool b) : u(vdupq_n_u32(b ? TINY_TRUE_BITS : TINY_FALSE_BITS)) {}
	WideBool(const bool *b) {
		u = vsetq_lane_u32(b[0] ? TINY_TRUE_BITS : TINY_FALSE_BITS, u, 0);
		u = vsetq_lane_u32(b[1] ? TINY_TRUE_BITS : TINY_FALSE_BITS, u, 1);
		u = vsetq_lane_u32(b[2] ? TINY_TRUE_BITS : TINY_FALSE_BITS, u, 2);
		u = vsetq_lane_u32(b[3] ? TINY_TRUE_BITS : TINY_FALSE_BITS, u, 3);
	}

	//WideBool( void ) : u(vdupq_n_u32(0)) {}

	WideBool operator||(const WideBool &r) const { return vorrq_u32(u, r.u); }
	WideBool operator&&(const WideBool &r) const { return vandq_u32(u, r.u); }

	WideBool operator|(const WideBool &r) const { return vorrq_u32(u, r.u); }
	WideBool operator&(const WideBool &r) const { return vandq_u32(u, r.u); }

	WideBool operator!( void ) const { return veorq_u32(u, vdupq_n_u32(TINY_TRUE_BITS)); }

	bool all_fail( void ) const
	{
		// http://stackoverflow.com/questions/15389539/fastest-way-to-test-a-128-bit-neon-register-for-a-value-of-0-using-intrinsics
		uint64x2_t v0   = vreinterpretq_u64_u32(u);
		uint64x1_t v0or = vorr_u64(vget_high_u64(v0), vget_low_u64(v0));
		uint32x2_t v1   = vreinterpret_u32_u64(v0or);
		return (vget_lane_u32(v1, 0) | vget_lane_u32(v1, 1)) == TINY_FALSE_BITS;
	}

	bool all_pass( void ) const
	{
		uint64x2_t v0    = vreinterpretq_u64_u32(u);
		uint64x1_t v0and = vand_u64(vget_high_u64(v0), vget_low_u64(v0));
		uint32x2_t v1    = vreinterpret_u32_u64(v0and);
		return (vget_lane_u32(v1, 0) & vget_lane_u32(v1, 1)) == TINY_TRUE_BITS;
	}

} __attribute__((aligned(TINY_BYTE_ALIGN)));

class WideReal
{
	friend class WideSInt;
	template < int n >
	friend class wide_fixed;

private:
	float32x4_t f;

private:
	WideReal(const float32x4_t &r) : f(r) {}

public:
	typedef float serial_t;

	WideReal( void ) {}
	WideReal(const WideReal &r) : f(r.f) {}
	WideReal(float val) : f(vdupq_n_f32(val)) {}
	WideReal(bool val) : f(vdupq_n_f32(val ? 1.0f : 0.0f)) {}
	explicit WideReal(const float *in) : f(vld1q_f32(in)) {}
	inline explicit WideReal(const WideSInt &r);
	inline explicit WideReal(const WideBool &r)

	WideReal operator-( void ) const { return WideReal(0.0f) - *this; }

	WideReal &operator+=(const WideReal &r) { f = vaddq_f32(f, r.f); return *this; }
	WideReal &operator-=(const WideReal &r) { f = vsubq_f32(f, r.f); return *this; }
	WideReal &operator*=(const WideReal &r) { f = vmulq_f32(f, r.f); return *this; }
	WideReal &operator/=(const WideReal &r) {
		float32x4_t inv;
		inv = vrecpeq_f32(r.f);
		// A few iterations of Newton-Raphson for accuracy
		inv = vmulq_f32(vrecpsq_f32(r.f, inv), inv);
		inv = vmulq_f32(vrecpsq_f32(r.f, inv), inv);
		// Multiply by 1/r
		f = vmulq_f32(f, inv);
		return *this;
	}
	WideReal &operator|=(const WideReal &r) { *(uint32x4_t*)(&f) = vorrq_u32(*(uint32x4_t*)(&f), *(uint32x4_t*)(&r.f)); return *this; }
	WideReal &operator|=(const WideBool &r) { *(uint32x4_t*)(&f) = vorrq_u32(*(uint32x4_t*)(&f), r.u); return *this; }
	WideReal &operator&=(const WideReal &r) { *(uint32x4_t*)(&f) = vandq_u32(*(uint32x4_t*)(&f), *(uint32x4_t*)(&r.f)); return *this; }
	WideReal &operator&=(const WideBool &r) { *(uint32x4_t*)(&f) = vandq_u32(*(uint32x4_t*)(&f), r.u); return *this; }

	WideReal operator+(const WideReal &r) const { return vaddq_f32(f, r.f); }
	WideReal operator-(const WideReal &r) const { return vsubq_f32(f, r.f); }
	WideReal operator*(const WideReal &r) const { return vmulq_f32(f, r.f); }
	WideReal operator/(const WideReal &r) const { WideReal o; o.f = f; o /= r; return o; }
	WideReal operator|(const WideReal &r) const { uint32x4_t o = vorrq_u32(*(uint32x4_t*)(&f), *(uint32x4_t*)(&r.f)); return *(float32x4_t*)(&o); }
	WideReal operator|(const WideBool &r)  const { uint32x4_t o = vorrq_u32(*(uint32x4_t*)(&f), r.u); return *(float32x4_t*)(&o); }
	WideReal operator&(const WideReal &r) const { uint32x4_t o = vandq_u32(*(uint32x4_t*)(&f), *(uint32x4_t*)(&r.f)); return *(float32x4_t*)(&o); }
	WideReal operator&(const WideBool &r)  const { uint32x4_t o = vandq_u32(*(uint32x4_t*)(&f), r.u); return *(float32x4_t*)(&o); }

	static WideReal max(const WideReal &a, const WideReal &b) { WideReal o; o.f = vmaxq_f32(a.f, b.f); return o; }
	static WideReal min(const WideReal &a, const WideReal &b) { WideReal o; o.f = vminq_f32(a.f, b.f); return o; }
	static WideReal sqrt(const WideReal &x) {
		WideReal o;
		o.f = vrsqrteq_f32(x.f);
		o.f = vrecpeq_f32(o.f);
		o.f = vmulq_f32(vrecpsq_f32(x.f, o.f), o.f);
		o.f = vmulq_f32(vrecpsq_f32(x.f, o.f), o.f);
		return o;
	}

	WideBool operator==(const WideReal &r) const { return vceqq_f32(f, r.f); }
	WideBool operator!=(const WideReal &r) const { return vmvnq_u32(vceqq_f32(f, r.f)); }
	WideBool operator< (const WideReal &r) const { return vcltq_f32(f, r.f); }
	WideBool operator<=(const WideReal &r) const { return vcleq_f32(f, r.f); }
	WideBool operator> (const WideReal &r) const { return vcgtq_f32(f, r.f); }
	WideBool operator>=(const WideReal &r) const { return vcgeq_f32(f, r.f); }

	void to_scalar(float *out) const { vst1q_f32(out, f); }

	static WideReal mov_if_true(const WideReal &l, const WideReal &r, const WideBool &cond_mask)
	{
		// NOTE: Watch out for compiler optimizations!!!
		union wide_bits
		{
			uint32x4_t u;
			float32x4_t f;
		};
		wide_bits l_mask;
		l_mask.u = vmvnq_u32(cond_mask.u);
		wide_bits rc, lc;
		lc.u = vandq_u32(*(uint32x4_t*)(&l.f), l_mask.u);
		rc.u = vandq_u32(*(uint32x4_t*)(&r.f), cond_mask.u);
		rc.u = vorrq_u32(rc.u, lc.u);
		return rc.f;
	}

} __attribute__((aligned(TINY_BYTE_ALIGN)));

template < int n >
class wide_fixed
{
	friend class WideSInt;

private:
	int32x4_t i;

private:
	wide_fixed(const int32x4_t &r) : i(r) {}

public:
#ifdef MML_FIXED_H_INCLUDED__
	typedef mml::fixed<int, n> serial_t;
#else
	typedef int serial_t;
#endif

	wide_fixed( void ) {}
	wide_fixed(const wide_fixed<n> &r) : i(r.i) {}
	wide_fixed(int val) : i(vshlq_n_s32(vdupq_n_s32(val), n)) {}
	wide_fixed(float val) : i() {}
	wide_fixed(bool val) : i(vshlq_n_s32(vdupq_n_s32(val ? 1 : 0), n)) {}
	explicit wide_fixed(const int *in) : i(vld1q_s32(in)) {}
	//explicit wide_fixed(const float *in) : i(vld1q_s32(in)) {  } // convert to int, arithmetic shift left by n
	inline explicit wide_fixed(const WideSInt &r);
	inline explicit wide_fixed(const WideReal &r);

	wide_fixed<n> operator-( void ) const { return wide_fixed<n>(0) - *this; }

	wide_fixed<n> &operator+=(const wide_fixed<n> &r) { i = vaddq_s32(i, r.i); return *this; }
	wide_fixed<n> &operator-=(const wide_fixed<n> &r) { i = vsubq_s32(i, r.i); return *this; }
	wide_fixed<n> &operator*=(const wide_fixed<n> &r) { i = vshrq_n_s32(vmulq_s32(i, r.i), n); return *this; }
	wide_fixed<n> &operator|=(const wide_fixed<n> &r) { i = vorrq_s32(i, r.i); return *this; }
	wide_fixed<n> &operator|=(const WideBool &r)     { i = vorrq_s32(i, *(const int32x4_t*)(&r.u)); return *this; }
	wide_fixed<n> &operator&=(const wide_fixed<n> &r) { i = vandq_s32(i, r.i); return *this; }
	wide_fixed<n> &operator&=(const WideBool &r)     { i = vandq_s32(i, *(const int32x4_t*)(&r.u)); return *this; }

	wide_fixed<n> operator+(const wide_fixed<n> &r) const { return vaddq_s32(i, r.i); }
	wide_fixed<n> operator-(const wide_fixed<n> &r) const { return vsubq_s32(i, r.i); }
	wide_fixed<n> operator*(const wide_fixed<n> &r) const { return vshrq_n_s32(vmulq_s32(i, r.i), n); }
	wide_fixed<n> operator|(const wide_fixed<n> &r) const { return vorrq_s32(i, r.i); }
	wide_fixed<n> operator|(const WideBool &r)     const { return vorrq_s32(i, *(const int32x4_t*)(&r.u)); }
	wide_fixed<n> operator&(const wide_fixed<n> &r) const { return vandq_s32(i, r.i); }
	wide_fixed<n> operator&(const WideBool &r)     const { return vandq_s32(i, *(const int32x4_t*)(&r.u)); }

	WideBool operator==(const wide_fixed<n> &r) const { return vceqq_s32(i, r.i); }
	WideBool operator!=(const wide_fixed<n> &r) const { return vmvnq_u32(vceqq_s32(i, r.i)); }
	WideBool operator< (const wide_fixed<n> &r) const { return vcltq_s32(i, r.i); }
	WideBool operator<=(const wide_fixed<n> &r) const { return vcleq_s32(i, r.i); }
	WideBool operator> (const wide_fixed<n> &r) const { return vcgtq_s32(i, r.i); }
	WideBool operator>=(const wide_fixed<n> &r) const { return vcgeq_s32(i, r.i); }

	void to_scalar(int *out) const { vst1q_s32(out, i); }

	static wide_fixed<n> mov_if_true(const wide_fixed<n> &l, const wide_fixed<n> &r, const WideBool &cond_mask)
	{
		WideBool l_mask;
		l_mask.u = vmvnq_u32(cond_mask.u);
		int32x4_t rc, lc;
		lc = vandq_s32(l.i, *(const int32x4_t*)(&l_mask.u));
		rc = vandq_s32(r.i, *(const int32x4_t*)(&cond_mask.u));
		rc = vorrq_s32(rc, lc);
		return rc;
	}

} __attribute__((aligned(TINY_BYTE_ALIGN)));

class WideSInt
{
	friend class WideReal;
	template < int n >
	friend class wide_fixed;

private:
	int32x4_t i;

private:
	WideSInt(const int32x4_t &r) : i(r) {}

public:
	typedef int serial_t;

	WideSInt( void ) {}
	WideSInt(const WideSInt &r) : i(r.i) {}
	WideSInt(int val) : i(vdupq_n_s32(val)) {}
	WideSInt(bool val) : i(vdupq_n_s32(val ? 1 : 0)) {}
	explicit WideSInt(const int *in) : i(vld1q_s32(in)) {}
	inline explicit WideSInt(const WideReal &r);
	template < int n >
	inline explicit WideSInt(const wide_fixed<n> &r);
	inline explicit WideSInt(const WideBool &r);

	// For implementing integer division on systems without hardware support (eg. ARM)
	// https://github.com/gcc-mirror/gcc/blob/master/libgcc/config/epiphany/divsi3.S

	WideSInt operator-( void ) const { return WideSInt(0) - *this; }

	WideSInt &operator+=(const WideSInt &r)  { i = vaddq_s32(i, r.i); return *this; }
	WideSInt &operator-=(const WideSInt &r)  { i = vsubq_s32(i, r.i); return *this; }
	WideSInt &operator*=(const WideSInt &r)  { i = vmulq_s32(i, r.i); return *this; }
	WideSInt &operator|=(const WideSInt &r)  { i = vorrq_s32(i, r.i); return *this; }
	WideSInt &operator|=(const WideBool &r) { i = vorrq_s32(i, *(const int32x4_t*)(&r.u)); return *this; }
	WideSInt &operator&=(const WideSInt &r)  { i = vandq_s32(i, r.i); return *this; }
	WideSInt &operator&=(const WideBool &r) { i = vandq_s32(i, *(const int32x4_t*)(&r.u)); return *this; }
	WideSInt &operator<<=(int r) { i = vshlq_n_s32(i, r); return *this; }
	WideSInt &operator>>=(int r) { i = vshlq_n_s32(i, -r); return *this; }
//		WideSInt &operator/=(const WideSInt &r) {}
//		WideSInt &operator%=(const WideSInt &r) { *this = *this - (*this / r) * r; return *this; }

	WideSInt operator+(const WideSInt &r)  const { return vaddq_s32(i, r.i); }
	WideSInt operator-(const WideSInt &r)  const { return vsubq_s32(i, r.i); }
	WideSInt operator*(const WideSInt &r)  const { return vmulq_s32(i, r.i); }
	WideSInt operator|(const WideSInt &r)  const { return vorrq_s32(i, r.i); }
	WideSInt operator|(const WideBool &r) const { return vorrq_s32(i, *(const int32x4_t*)(&r.u)); }
	WideSInt operator&(const WideSInt &r)  const { return vandq_s32(i, r.i); }
	WideSInt operator&(const WideBool &r) const { return vandq_s32(i, *(const int32x4_t*)(&r.u)); }
	WideSInt operator<<(int r) const { return vshlq_n_s32(i, r); }
	WideSInt operator>>(int r) const { return vshlq_n_s32(i, -r); }
//		WideSInt &operator/(const WideSInt &r) { WideSInt o = *this; return o /= r; }
//		WideSInt &operator%(const WideSInt &r) { return *this - (*this / r) * r; }

	WideBool operator==(const WideSInt &r) const { return vceqq_s32(i, r.i); }
	WideBool operator!=(const WideSInt &r) const { return vmvnq_u32(vceqq_s32(i, r.i)); }
	WideBool operator< (const WideSInt &r) const { return vcltq_s32(i, r.i); }
	WideBool operator<=(const WideSInt &r) const { return vcleq_s32(i, r.i); }
	WideBool operator> (const WideSInt &r) const { return vcgtq_s32(i, r.i); }
	WideBool operator>=(const WideSInt &r) const { return vcgeq_s32(i, r.i); }

	void to_scalar(int *out) const { vst1q_s32(out, i); }

	static WideSInt mov_if_true(const WideSInt &l, const WideSInt &r, const WideBool &cond_mask)
	{
		WideBool l_mask;
		l_mask.u = vmvnq_u32(cond_mask.u);
		int32x4_t rc, lc;
		lc = vandq_s32(l.i, *(int32x4_t*)(&l_mask.u));
		rc = vandq_s32(r.i, *(int32x4_t*)(&cond_mask.u));
		rc = vorrq_s32(rc, lc);
		return rc;
	}

} __attribute__((aligned(TINY_BYTE_ALIGN)));

WideReal::WideReal(const WideSInt &r) : f(vcvtq_f32_s32(r.i)) {}
WideReal::WideReal(const WideBool &r) : WideReal(WideReal::mov_if_true(WideReal(1.0f), WideReal(0.0f), r)) {}

template < int n >
wide_fixed<n>::wide_fixed(const WideSInt &r) : i(vshlq_n_s32(r.i, n)) {}
template < int n >
wide_fixed<n>::wide_fixed(const WideReal &r) : i(vcvtq_s32_f32(vmulq_n_f32(r.f, 1 << n))) {}
template < int n >
wide_fixed<n>::wide_fixed(const WideBool &r) : wide_fixed(wide_fixed<n>::mov_if_true(wide_fixed<n>(WideSInt(1)), wide_fixed<n>(WideSInt(0)), r)) {}

WideSInt::WideSInt(const WideReal &r) : i(vcvtq_s32_f32(r.f)) {}
template < int n >
WideSInt::WideSInt(const wide_fixed<n> &r) : i(vshrq_n_s32(r.i, n)) {}
WideSInt::WideSInt(const WideBool &r) : WideSInt(WideSInt::mov_if_true(WideSInt(1), WideSInt(0), r)) {}

#elif TINY_SIMD == TINY_SIMD_ALTIVEC

// https://gcc.gnu.org/onlinedocs/gcc-4.3.3/gcc/PowerPC-AltiVec-Built_002din-Functions.html
// http://www.nxp.com/assets/documents/data/en/reference-manuals/ALTIVECPEM.pdf

#error "AltiVec not supported yet."

union WideBool
{
	friend class WideReal;
	friend class WideSInt;
	template < int n >
	friend class wide_fixed

private:
	__vector float        f;
	__vector bool int     b;
	__vector unsigned int u;

private:
	WideBool(const __vector float &r) : f(r) {}
	WideBool(const __vector bool int &r) : b(r) {}
	WideBool(const __vector unsigned int &r) : u(r) {}

public:
	typedef bool serial_t;

	WideBool( void ) {}
	WideBool(const WideBool &b) : f(b.f) {}
	WideBool(bool b);
	WideBool(const bool *b);

	WideBool operator||(const WideBool &r) const { return vec_and(b, r.b); }
	WideBool operator&&(const WideBool &r) const { return vec_or(b, r.b); }

	WideBool operator!( void ) const { return vec_xor(b, WideBool(true).b); }

	bool all_fail( void ) const;
	bool all_pass( void ) const;
};

#elif TINY_SIMD == TINY_SIMD_NONE

#ifndef TINY_SIGNED_SHIFT
#include <limits.h>
#endif

union WideBool
{
	friend class WideReal;
	friend class WideSInt;
	template < int n >
	friend class wide_fixed;

private:
	unsigned int u;
	float        f;

public:
	typedef bool serial_t;

	WideBool( void ) {}
	WideBool(const WideBool &b) : u(b.u) {}
	WideBool(bool b) : u(b ? TINY_TRUE_BITS : TINY_FALSE_BITS) {}
	WideBool(const bool *b) : u(b[0] ? TINY_TRUE_BITS : TINY_FALSE_BITS) {}

	WideBool operator||(const WideBool &r) const { WideBool o; o.u = (u || r.u) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator&&(const WideBool &r) const { WideBool o; o.u = (u && r.u) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }

	WideBool operator|(const WideBool &r) const { WideBool o; o.u = (u | r.u) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator&(const WideBool &r) const { WideBool o; o.u = (u & r.u) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }

	WideBool operator!( void ) const { WideBool o; o.u = ~u; return o; }

	bool all_fail( void ) const { return u == TINY_FALSE_BITS; }
	bool all_pass( void ) const { return u == TINY_TRUE_BITS; }
};

class WideReal
{
	friend class WideSInt;
	template < int n >
	friend class wide_fixed;

private:
	float f;

public:
	typedef float serial_t;

	WideReal( void ) {}
	WideReal(const WideReal &r) : f(r.f) {}
	WideReal(float val) : f(val) {}
	WideReal(bool val) : f(val ? 1.0f : 0.0f) {}
	explicit WideReal(const float *in) : f(*in) {}
	inline explicit WideReal(const WideSInt &i);
	inline explicit WideReal(const WideBool &r);

	WideReal operator-( void ) const { return -f; }

	WideReal &operator+=(const WideReal &r) { f += r.f; return *this; }
	WideReal &operator-=(const WideReal &r) { f -= r.f; return *this; }
	WideReal &operator*=(const WideReal &r) { f *= r.f; return *this; }
	WideReal &operator/=(const WideReal &r) { f /= r.f; return *this; }
	WideReal &operator|=(const WideReal &r) { *(unsigned int*)(&f) |= *(unsigned int*)(&r.f); return *this; }
	WideReal &operator|=(const WideBool &r)  { *(unsigned int*)(&f) |= r.u; return *this;  }
	WideReal &operator&=(const WideReal &r) { *(unsigned int*)(&f) &= *(unsigned int*)(&r.f); return *this; }
	WideReal &operator&=(const WideBool &r)  { *(unsigned int*)(&f) &= r.u; return *this;  }

	WideReal operator+(const WideReal &r) const { return f + r.f; }
	WideReal operator-(const WideReal &r) const { return f - r.f; }
	WideReal operator*(const WideReal &r) const { return f * r.f; }
	WideReal operator/(const WideReal &r) const { return f / r.f; }
	WideReal operator|(const WideReal &r) const { unsigned int o = *(unsigned int*)(&f) | *(unsigned int*)(&r.f); return *(float*)(&o); }
	WideReal operator|(const WideBool &r)  const { unsigned int o = *(unsigned int*)(&f) | r.u; return *(float*)(&o); }
	WideReal operator&(const WideReal &r) const { unsigned int o = *(unsigned int*)(&f) & *(unsigned int*)(&r.f); return *(float*)(&o); }
	WideReal operator&(const WideBool &r)  const { unsigned int o = *(unsigned int*)(&f) & r.u; return *(float*)(&o); }

	static WideReal max(const WideReal &a, const WideReal &b) { return a.f < b.f ? b.f : a.f; }
	static WideReal min(const WideReal &a, const WideReal &b) { return a.f < b.f ? a.f : b.f; }
	static WideReal sqrt(const WideReal &x)                     { return ::sqrt(x.f); }

	WideBool operator==(const WideReal &r) const { WideBool o; o.u = (f == r.f) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator!=(const WideReal &r) const { WideBool o; o.u = (f != r.f) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator< (const WideReal &r) const { WideBool o; o.u = (f <  r.f) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator<=(const WideReal &r) const { WideBool o; o.u = (f <= r.f) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator> (const WideReal &r) const { WideBool o; o.u = (f >  r.f) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator>=(const WideReal &r) const { WideBool o; o.u = (f >= r.f) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }

	void to_scalar(float *out) const { *out = f; }

	static WideReal mov_if_true(const WideReal &l, const WideReal &r, const WideBool &cond_mask)
	{
		// This function needs to be way more complicated than it should be.
		// The compiler optimizes casting between float* and int* to shit.
		union bits
		{
			float        f;
			unsigned int u;
		};

		bits a = { l.f };
		bits b = { r.f };
		bits o;
		o.u = (a.u & ~cond_mask.u) | (b.u & cond_mask.u);
		return WideReal(o.f);
	}
};

template < int n >
class wide_fixed
{
	friend class WideSInt;

private:
	int i;

private:
	inline static int signed_rshift(int x, int b)
	{
		return
			(x >> b)
		#ifndef TINY_SIGNED_SHIFT
			| (x & (1 << (sizeof(int) * CHAR_BIT - 1)))
		#endif
		;
	}

	inline static int signed_lshift(int x, int b)
	{
		return
		#ifndef TINY_SIGNED_SHIFT
			x * (1 << b)
		#else
			(x << b)
		#endif
		;
	}

public:
#ifdef MML_FIXED_H_INCLUDED__
	typedef mml::fixed<int, n> serial_t;
#else
	typedef int serial_t;
#endif

	wide_fixed( void ) {}
	wide_fixed(const wide_fixed<n> &f) : i(f.i) {}
	wide_fixed(int val) : i(signed_lshift(val, n)) {}
	wide_fixed(float val) : i(val * (1<<n)) {}
	wide_fixed(bool val) : i(int(val ? 1 : 0) * (1<<n)) {}
	explicit wide_fixed(const int *in) : i(signed_lshift(*in, n)) {}
	explicit wide_fixed(const float *in) : i(*in * (1<<n)) {}
	inline explicit wide_fixed(const WideSInt &r);
	inline explicit wide_fixed(const WideReal &r);
	inline explicit wide_fixed(const WideBool &r);

	wide_fixed<n> operator-( void ) const { wide_fixed<n> o; o.i = -i; return o; }

	wide_fixed<n> &operator+=(const wide_fixed<n> &r) { i += r.i; return *this; }
	wide_fixed<n> &operator-=(const wide_fixed<n> &r) { i -= r.i; return *this; }
	wide_fixed<n> &operator*=(const wide_fixed<n> &r) { i = signed_rshift(i * r.i, n); return *this; }
	wide_fixed<n> &operator|=(const wide_fixed<n> &r) { i |= r.i; return *this; }
	wide_fixed<n> &operator|=(const WideBool &r)  { i |= r.u; return *this; }
	wide_fixed<n> &operator&=(const wide_fixed<n> &r) { i &= r.i; return *this; }
	wide_fixed<n> &operator&=(const WideBool &r)  { i &= r.u; return *this; }

	wide_fixed<n> operator+(const wide_fixed<n> &r) const { return i + r.i; }
	wide_fixed<n> operator-(const wide_fixed<n> &r) const { return i - r.i; }
	wide_fixed<n> operator*(const wide_fixed<n> &r) const { return signed_rshift(i * r.i, n); }
	wide_fixed<n> operator|(const wide_fixed<n> &r) const { return i | r.i; }
	wide_fixed<n> operator|(const WideBool &r)  const { return i | r.u; }
	wide_fixed<n> operator&(const wide_fixed<n> &r) const { return i & r.i; }
	wide_fixed<n> operator&(const WideBool &r)  const { return i & r.u; }

	WideBool operator==(const wide_fixed<n> &r) const { WideBool o; o.u = (i == r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator!=(const wide_fixed<n> &r) const { WideBool o; o.u = (i != r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator< (const wide_fixed<n> &r) const { WideBool o; o.u = (i <  r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator<=(const wide_fixed<n> &r) const { WideBool o; o.u = (i <= r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator> (const wide_fixed<n> &r) const { WideBool o; o.u = (i >  r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator>=(const wide_fixed<n> &r) const { WideBool o; o.u = (i >= r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }

	void to_scalar(int *out) const { *out = signed_rshift(i, n); }

	static wide_fixed<n> mov_if_true(const wide_fixed<n> &l, const wide_fixed<n> &r, const WideBool &cond_mask)
	{
		unsigned int o = (*(unsigned int*)(&l.i) & ~cond_mask.u) | (*(unsigned int*)(&r.i) & cond_mask.u);
		wide_fixed<n> out;
		out.i = *(int*)&o;
		return out;
	}
};

class WideSInt
{
	friend class WideReal;
	template < int n >
	friend class wide_fixed;

private:
	int i;

public:
	typedef int serial_t;

	WideSInt( void ) {}
	WideSInt(const WideSInt &r) : i(r.i) {}
	WideSInt(int val) : i(val) {}
	WideSInt(bool val) : i(val ? 1 : 0) {}
	explicit WideSInt(const int *in) : i(*in) {}
	inline explicit WideSInt(const WideReal &f);
	template < int n >
	inline explicit WideSInt(const wide_fixed<n> &f);
	inline explicit WideSInt(const WideBool &r);

	WideSInt operator-( void ) const { return -i; }

	WideSInt &operator+=(const WideSInt &r)  { i += r.i; return *this; }
	WideSInt &operator-=(const WideSInt &r)  { i -= r.i; return *this; }
	WideSInt &operator*=(const WideSInt &r)  { i *= r.i; return *this; }
	WideSInt &operator|=(const WideSInt &r)  { i |= r.i; return *this; }
	WideSInt &operator|=(const WideBool &r) { i |= r.u; return *this; }
	WideSInt &operator&=(const WideSInt &r)  { i &= r.i; return *this; }
	WideSInt &operator&=(const WideBool &r) { i &= r.u; return *this; }
	WideSInt &operator<<=(int r) { i <<= r; return *this; }
	WideSInt &operator>>=(int r) { i >>= r; return *this; }
//		WideSInt &operator/=(const WideSInt &r) {}
//		WideSInt &operator%=(const WideSInt &r) { *this = *this - (*this / r) * r; return *this; }

	WideSInt operator+(const WideSInt &r)  const { return i + r.i; }
	WideSInt operator-(const WideSInt &r)  const { return i - r.i; }
	WideSInt operator*(const WideSInt &r)  const { return i * r.i; }
	WideSInt operator|(const WideSInt &r)  const { return i | r.i; }
	WideSInt operator|(const WideBool &r) const { return i | r.u; }
	WideSInt operator&(const WideSInt &r)  const { return i & r.i; }
	WideSInt operator&(const WideBool &r) const { return i & r.u; }
	WideSInt operator<<(int r) const { return i << r; }
	WideSInt operator>>(int r) const { return i >> r; }
//		WideSInt &operator/(const WideSInt &r) { WideSInt o = *this; return o /= r; }
//		WideSInt &operator%(const WideSInt &r) { return *this - (*this / r) * r; }

	WideBool operator==(const WideSInt &r) const { WideBool o; o.u = (i == r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator!=(const WideSInt &r) const { WideBool o; o.u = (i != r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator< (const WideSInt &r) const { WideBool o; o.u = (i <  r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator<=(const WideSInt &r) const { WideBool o; o.u = (i <= r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator> (const WideSInt &r) const { WideBool o; o.u = (i >  r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }
	WideBool operator>=(const WideSInt &r) const { WideBool o; o.u = (i >= r.i) ? TINY_TRUE_BITS : TINY_FALSE_BITS; return o; }

	void to_scalar(int *out) const { *out = i; }

	static WideSInt mov_if_true(const WideSInt &l, const WideSInt &r, const WideBool &cond_mask)
	{
		unsigned int o = (*(unsigned int*)(&l.i) & ~cond_mask.u) | (*(unsigned int*)(&r.i) & cond_mask.u);
		return WideSInt(*(int*)(&o));
	}
};

WideReal::WideReal(const WideSInt &r) : f((float)r.i) {}
WideReal::WideReal(const WideBool &r) : WideReal(WideReal::mov_if_true(WideReal(1.0f), WideReal(0.0f), r)) {}

template < int n >
wide_fixed<n>::wide_fixed(const WideSInt &r) : i(wide_fixed<n>::signed_lshift(r.i, n)) {}
template < int n >
wide_fixed<n>::wide_fixed(const WideReal &r) : i((int)(r.f * (1<<n))) {}
template < int n >
wide_fixed<n>::wide_fixed(const WideBool &r) : wide_fixed(wide_fixed<n>::mov_if_true(wide_fixed<n>(WideSInt(1)), wide_fixed<n>(WideSInt(0)), r)) {}

template < int n >
WideSInt::WideSInt(const wide_fixed<n> &r) : i(wide_fixed<n>::signed_rshift(r.i, n)) {}
WideSInt::WideSInt(const WideReal &r) : i((int)r.f) {}
WideSInt::WideSInt(const WideBool &r) : WideSInt(WideSInt::mov_if_true(WideSInt(1), WideSInt(0), r)) {}

#endif

}

#endif // TINY_SIMD_H
