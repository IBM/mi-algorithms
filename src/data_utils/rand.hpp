/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
unsigned uniform(unsigned i, unsigned m);
void permute(int permutation[], unsigned n);


#ifdef __cplusplus
}
#endif

}//: namespace data_utils
}//: namespace mic


#endif
