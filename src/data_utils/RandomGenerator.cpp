/*!
 * \file Random.cpp
* \brief Contains definition of methods of a random generator singleton.
  * \author tkornut
 * \date Dec 24, 2015
 */

#include  <data_utils/RandomGenerator.hpp>

#include <logger/Log.hpp>

namespace mic {
namespace data_utils {

// Init generator instance - as NULL.
boost::atomic<RandomGenerator*> RandomGenerator::instance_(NULL);

// Initilize mutex.
boost::mutex RandomGenerator::instantiation_mutex;


RandomGenerator* RandomGenerator::getInstance() {
	// Try to load the instance - first check.
	RandomGenerator* tmp = instance_.load(boost::memory_order_consume);
	// If instance does not exist.
	if (!tmp) {
		// Enter critical section.
		boost::mutex::scoped_lock guard(instantiation_mutex);
		// Try to load the instance - second check.
		tmp = instance_.load(boost::memory_order_consume);
		// If still does not exist - create new instance.
		if (!tmp) {
			tmp = new RandomGenerator;
			instance_.store(tmp, boost::memory_order_release);
		}//: if
		// Exit critical section.
	}//: if
	// Return instance.
	return tmp;
}


RandomGenerator::RandomGenerator() :
		rng_mt19937_64(rd()),
		uniform_int_dist(0, RAND_MAX),
		uniform_real_dist(0, 1),
		normal_real_dist(0, 1)
{
	// TODO Auto-generated constructor stub

}


uint64_t RandomGenerator::uniRandInt() {
	LOG(LTRACE)<<"uniRandInt";
    return uniform_int_dist(rng_mt19937_64);
}

double RandomGenerator::uniRandReal(double min, double max) {
	LOG(LTRACE)<<"uniRandReal";
    return (uniform_real_dist(rng_mt19937_64) + min)/(max - min);
}

double RandomGenerator::normRandReal(double mean, double variance) {
	LOG(LTRACE)<<"normRandReal";
    return (normal_real_dist(rng_mt19937_64) * variance + mean);
}


} /* namespace data_utils */
} /* namespace mic */
