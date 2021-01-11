#ifndef RETRO_RAND_H
#define RETRO_RAND_H

#include <cstdint>
#include "MiniLib/MML/mmlRandom.h"
#include "MiniLib/MML/mmlVector.h"

namespace retro3d
{

class Random
{
private:
	mmlRandom m_rand;

public:
	Random(uint64_t seed, uint64_t inc = 1);

	int32_t      Range(int32_t min, int32_t max);
	float        Range(float min, float max);
	float        Unit( void );
	uint32_t     Bits( void );
	mmlVector<2> Normal2( void );
	mmlVector<3> Normal3( void );
	mmlVector<2> InCircle( void );
	mmlVector<3> InSphere( void );
};

}

#endif // RETRO_RAND_H
