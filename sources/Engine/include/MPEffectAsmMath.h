#ifndef MPEffectAsmMath_H
#define MPEffectAsmMath_H

#include <stdlib.h>

#define RAND_MAX 0x7fff


__forceinline float asm_rand() {
	return float(rand());
}

__forceinline float asm_rand_max() {
	return float(RAND_MAX);
}

#endif
