#ifndef RETRO_ASSERT_H
#define RETRO_ASSERT_H

#include <assert.h>

#ifdef RETRO3D_ASSERT
	#undef RETRO3D_ASSERT
#endif

#ifdef RETRO3D_DEBUG
	#define RETRO3D_ASSERT(x) assert(x)
#else
	#define RETRO3D_ASSERT(x)
#endif

#endif // RETRO_ASSERT_H
