// #include "system_utils/timer.h"


#include <data_utils/rand.hpp>

#include <sys/time.h>

namespace mic {
namespace data_utils {

void init_rand (void) {

	// standard rand()
    struct timeval t;
    gettimeofday (&t, NULL);
    srand ( (unsigned int) ( (t.tv_sec * 1000) + (t.tv_usec / 1000) ) );

#ifdef USE_MT_RNG
    // MT RNG
    unsigned long long init[4]={(unsigned long long) ( (t.tv_sec * 1000) + (t.tv_usec / 1000) ), 
    								23456ULL, 0x34567ULL, 0x45678ULL}, length=4;
    init_by_array64(init, length);
#endif

}

double get_rand_range(float minimum, float maximum) {

	return ( fabs(maximum - minimum) * rand_real01() + minimum);

}

double rand_real01(void) {

#ifdef USE_MT_RNG
	return genrand64_real2();
#else
	return get_rand_range(0.0f, 1.0f);
#endif

}

unsigned long long rand_int(void) {

#ifdef USE_MT_RNG
	return genrand64_int64();
#else
	return (unsigned long long)rand();
#endif

}

unsigned long long rand_int_radius (unsigned long long center, unsigned long long radius, unsigned long long max) {

    unsigned long long range = radius * 2;
    unsigned long long rand_num = (unsigned long long)((long double)range * rand_real01());

    center -= radius;
    center += rand_num;

    return ((center + max) % max);

}

unsigned long long rand_int_radius_2d (unsigned long long center, unsigned long long radius, unsigned long long width, unsigned long long max) {

    unsigned long long range = radius * 2;
    unsigned long long rand_num_x = (unsigned long long)((long double)range * rand_real01());
    unsigned long long rand_num_y = (unsigned long long)((long double)range * rand_real01());

    center -= radius * width + radius;
    center += rand_num_x + rand_num_y * width;

    return ((center + max) % max);

}

unsigned long long rand_int_radius_2d_gaussian (unsigned long long center, unsigned long long radius, unsigned long long width, unsigned long long max) {

    unsigned long long range = radius * 2;
    unsigned long long rand_num_x = (unsigned long long)((long double)range * gaussrand() * 0.4);
    unsigned long long rand_num_y = (unsigned long long)((long double)range * gaussrand() * 0.4);

    center -= radius/2 * width + radius/2;
    center += rand_num_x + rand_num_y * width;

    return ((center + max) % max);

}

#define PI 3.141592654

double gaussrand(void)
{
    static double U, V;
    static int phase = 0;
    double Z;

    if(phase == 0) {
        U = (rand() + 1.) / (RAND_MAX + 2.);
        V = rand() / (RAND_MAX + 1.);
        Z = sqrt(-2 * log(U)) * sin(2 * PI * V);
    } else
        Z = sqrt(-2 * log(U)) * cos(2 * PI * V);

    phase = 1 - phase;

    return Z;
}

}//: namespace data_utils
}//: namespace mic

