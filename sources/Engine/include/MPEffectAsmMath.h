#ifndef MPEffectAsmMath_H
#define MPEffectAsmMath_H

#include <stdlib.h>

/* rand(). */
#define RAND_MAX 0x7fff


// 
__forceinline float asm_rand()
{
#if 0
	#if _MSC_VER >= 1300

	static unsigned __int64 q = time( NULL );

	_asm {
		movq mm0, q

		// do the magic MMX thing
		pshufw mm1, mm0, 0x1E
		paddd mm0, mm1

		// move to integer memory location and free MMX
		movq q, mm0
		emms
	}

	return float( q );
	#endif
#else
	// VC6  pshufw
	return float( rand() );
#endif

}

// 
__forceinline float asm_rand_max()
{
#if 0
	#if _MSC_VER >= 1300

	return std::numeric_limits< unsigned __int64 >::max();
	return 9223372036854775807.0f;
	#endif
#else
	// VC6  unsigned __int64
	return float( RAND_MAX );
#endif

}

#endif
