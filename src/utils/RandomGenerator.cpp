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
/*!
 * \file Random.cpp
* \brief Contains definition of methods of a random generator singleton.
  * \author tkornut
 * \date Dec 24, 2015
 */

#include  <utils/RandomGenerator.hpp>

#include <logger/Log.hpp>

namespace mic {
namespace utils {

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


uint64_t RandomGenerator::uniRandInt(int min, int max) {
	LOG(LTRACE)<<"uniRandInt";
	std::uniform_int_distribution<> dist(min, max);
	return dist(rng_mt19937_64);
    //return uniform_int_dist(rng_mt19937_64);
}

double RandomGenerator::uniRandReal(double min, double max) {
	LOG(LTRACE)<<"uniRandReal";
    return (uniform_real_dist(rng_mt19937_64) + min)/(max - min);
}

double RandomGenerator::normRandReal(double mean, double variance) {
	LOG(LTRACE)<<"normRandReal";
    return (normal_real_dist(rng_mt19937_64) * variance + mean);
}


} /* namespace utils */
} /* namespace mic */
