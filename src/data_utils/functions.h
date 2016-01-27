/*
* @File: functions.h
* @Author: kamilrocki
* @Email: kmrocki@us.ibm.com
* @Created: 2015-04-29 09:14:44
* @Last Modified by:   kamilrocki
* @Last Modified time: 2015-05-15 15:23:40
*/

#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include <cmath>

#include <data_utils/RandomGenerator.hpp>


#define __min(x, y) 			(((x) < (y)) ? (x) : (y))
#define __max(x, y) 			(((x) > (y)) ? (x) : (y))
#define __sgn(x)				(x > 0) ? 1 : ((x < 0) ? -1 : 0)

template< class T >
T _copy(T x);
template< class T >
T _zero(T x) { return (T)0; }
template< class T >
T _one(T x);
template< class T >
T _square(T x) { return (T)(x * x); }
template< class T >
T _sqrt(T x);
template< class T >
T _ReLU(T x) { return (T)__max((T)0, x); }
template< class T >
T _sgn(T x) { return __sgn(x); }

template< class T >
T _sigmoid(T x) { return (T)1 / ((T)1 + expf(-x)); };

template< class T >
T _recip(T x);
template< class T >
T _round(T x);
template< class T >
T _exp(T x);
template< class T >
T _log(T x);
template< class T >
T _H2(T x) { T val = -x * std::log2(x) - (1.0f-x) * std::log2(1.0f-x); if (std::isnan(val)) return (T)0; else return val; };

template< class T >
T _tanh(T x);

template< class T >
T _rand(T x) { return RAN_GEN->uniRandReal(); }

template< class T > 
T _randn(T x) { return RAN_GEN->normRandReal(); }

template< class T > 
T _sgn(T x);
template< class T >
T _positive(T x);
template< class T >
T _negative(T x);
template< class T >
T _divide(T x, T y);
template< class T >
T _mult(T x, T y) { return x * y; }

template< class T >
T _add(T x, T y) { return x + y; }

template< class T >
T _sub(T x, T y) { return x - y; }
template< class T >
T _max(T x, T y);
template< class T >
T _min(T x, T y);
template< class T >
T _compare(T x, T y) { return (T)(x > y); }
template< class T >
T _smaller(T x, T y);
template< class T >
T _equal(T x, T y);
template< class T >
T _notEqual(T x, T y);

#endif
