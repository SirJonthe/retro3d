#ifndef RETRO_DEFS_H
#define RETRO_DEFS_H

#include <stdint.h>
#include "MiniLib/MML/mmlMath.h"

namespace retro3d
{

template < typename num_t, typename real_t >
num_t Cyclerp(const num_t &a, const num_t &b, real_t x)
{
	return mmlLerp(
		a, b,
		real_t(1.0) - (mmlCos(x * mmlPI) * real_t(0.5) + real_t(0.5))
	);
}

template < typename num_t, typename real_t >
num_t Accelerp(const num_t &a, const num_t &b, const real_t &x)
{
	return Cyclerp(
		a, b,
		mmlClamp(real_t(0.0), x, real_t(1.0))
	);
}

template < typename num_t, typename real_t >
num_t Sigmoid(const num_t &a, const num_t &b, const real_t &x)
{
	return mmlLerp(a, b, x / (real_t(1.0) + mmlAbs(x)));
}

template < typename real_t >
real_t Sigmoid(const real_t &x)
{
	return Sigmoid(real_t(0.0), real_t(1.0), x);
}

template < typename num_t, typename real_t >
num_t Jerklerp(const num_t &a, const num_t &b, real_t x)
{
	return Accelerp(b, a, x);
}

}

#endif // RETRO_DEFS_H
