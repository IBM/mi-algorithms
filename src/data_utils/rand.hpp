#ifndef __RAND_H__
#define __RAND_H__

#include <stdlib.h>
#include <math.h>

#include <data_utils/mtrnd.hpp>

namespace mic {
namespace data_utils {

// random Number Generator
#define USE_MT_RNG							// use Mersenne Twister RNG?

#ifdef __cplusplus 
extern "C" {
#endif

void init_rand (void);
double get_rand_range(float minimum, float maximum);
double rand_real01(void);
unsigned long long rand_int(void);
unsigned long long rand_int_radius (unsigned long long center, unsigned long long radius, unsigned long long max);
unsigned long long rand_int_radius_2d (unsigned long long center, unsigned long long radius, unsigned long long width, unsigned long long max);
unsigned long long rand_int_radius_2d_gaussian (unsigned long long center, unsigned long long radius, unsigned long long width, unsigned long long max);
double gaussrand(void);

#ifdef __cplusplus
}
#endif

}//: namespace data_utils
}//: namespace mic


#endif
