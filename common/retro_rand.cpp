#include "retro_rand.h"

retro3d::Random::Random(uint64_t seed, uint64_t inc) : m_rand(seed, inc)
{}

int32_t retro3d::Random::Range(int32_t min, int32_t max)
{
	return m_rand.GetInt(min, max);
}

float retro3d::Random::Range(float min, float max)
{
	return m_rand.GetFloat(min, max);
}

float retro3d::Random::Unit( void )
{
	return m_rand.GetFloat();
}

uint32_t retro3d::Random::Bits( void )
{
	return m_rand.GetUint();
}

mmlVector<2> retro3d::Random::Normal2( void )
{
	const float a = Range(0.0f, mmlTAU);
	mmlVector<2> v;
	v[0] = mmlCos(a);
	v[1] = mmlSin(a);
	return v;
}

mmlVector<3> retro3d::Random::Normal3( void )
{
	const float a = Range(0.0f, mmlTAU);
	const float b = Range(0.0f, mmlTAU);
	mmlVector<3> v;
	v[0] = mmlSin(b) * mmlCos(a);
	v[1] = mmlSin(b) * mmlSin(a);
	v[2] = mmlCos(b);
	return v;
}

mmlVector<2> retro3d::Random::InCircle( void )
{
	return Normal2() * Range(-1.0f, 1.0f);
}

mmlVector<3> retro3d::Random::InSphere( void )
{
	return Normal3() * Range(-1.0f, 1.0f);
}
