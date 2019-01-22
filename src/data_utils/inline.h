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
#ifndef __INLINE_H__
#define __INLINE_H__

#include <math.h>

namespace mic {
namespace data_utils {

#define __min(x, y) 			(((x) < (y)) ? (x) : (y))
#define __max(x, y) 			(((x) > (y)) ? (x) : (y))

#define __negative(x) 			(~(x) + 1)

#define __mod64(x) 				((x) & 63)
#define __mod32(x) 				((x) & 31)
#define __mod16(x) 				((x) & 15)
#define __mod8(x) 				((x) & 7)
#define __mod4(x) 				((x) & 3)
#define __mod2(x) 				((x) & 1)

#define __sgn(x)				(x > 0) ? 1 : ((x < 0) ? -1 : 0)

#define PI 3.14159265358979323846

static __inline__ void __swapint(int* x, int* y) {

	// spares one register (hopefully)
	*x ^= *y;
	*y ^= *x; 
	*x ^= *y;

}

static __inline__ float sigmoid(float x) {

     return 1.0f / (1.0f + expf((float) -x));
}

static __inline__ float sigmoid_dx(float x) {

     return expf((float) x) / ((1.0f + expf((float) x)) * (1.0f + expf((float) x)));
}

static __inline__ float inverse_sigmoid(float y) {

	return logf (y / (1.0f - y) );
}

static __inline__ float sgn(float x) {

	return (x > 0.0f) ? 1.0f : ((x < 0.0f) ? -1.0f : 0.0f);
}

}//: namespace data_utils
}//: namespace mic

#endif
