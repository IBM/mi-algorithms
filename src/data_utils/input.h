#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdio.h>

namespace mic {
namespace data_utils {


#define IS_BIT_SET_01(bits, x) ((bits)[(x)])

// set a bit at position x in 'bits' /1
//#define SET_BIT(bits, x)	((bits) |= (1ULL << x))
#define SET_BIT(bits, x, val)  ((bits)[(x)] = (val))

// reset a bit at position x in 'bits' /0
//#define RESET_BIT(bits, x)  ((bits) &= (1ULL << x))
#define RESET_BIT(bits, x) ((bits)[(x)] = 0.0f)

// flip a bit at position x in 'bits' /0 to 1, 1 to 0
//#define FLIP_BIT(bits, x)  ((bits) ^= (1ULL << x))
#define FLIP_BIT(bits, x) ((bits)[(x)] = 1.0f - (bits)[(x)])

__inline__ void RESET_BITS(float* bits, unsigned long range_start, unsigned long range_end) {

	for (unsigned long i = range_start; i < range_end; i++)
		bits[i] = 0.0f;

}

__inline void PRINT_BITS(float* bits, unsigned long range_start, unsigned long range_end) {

	for (unsigned long i = range_start; i < range_end; i++)
		printf("%f", bits[i]);

	printf("\n");
	
}

}//: namespace data_utils
}//: namespace mic

#endif
