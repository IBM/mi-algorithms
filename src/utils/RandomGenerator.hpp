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
 * \file RandomGenerator.hpp
 * \brief Contains declaration of a random generator singleton.
 * \author tkornuta
 * \date Dec 24, 2015
 */

#ifndef SRC_DATA_UTILS_RANDOMGENERATOR_HPP_
#define SRC_DATA_UTILS_RANDOMGENERATOR_HPP_

#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>

#include <random>

namespace mic {
namespace utils {

/*!
 * \brief Random generator - defined in the form of a singleton, with double-checked locking pattern (DCLP) based access to instance.
 * \author tkornuta
 */
class RandomGenerator {
public:

	/*!
	 * Method for accessing the object instance, with double-checked locking optimization.
	 * @return Instance of ApplicationState singleton.
	 */
	static RandomGenerator* getInstance();


	/*!
	 * Return a random integer from range <0, RAND_MAX> - uniform distribution.
	 * @return Random integer.
	 */
	uint64_t uniRandInt(int min = 0, int max = RAND_MAX);

	/*!
	 * Return a random real number from range <min, max> - uniform distribution.
	 * @param min Min value.
	 * @param max Max value.
	 * @return Random real value.
	 */
	double uniRandReal(double min = 0, double max = 1);

	/*!
	 * Return a random real number - normal distribution.
	 * @param mean Mean.
	 * @param variance Variance.
	 * @return Random real value.
	 */
	double normRandReal(double mean = 0, double variance = 1);

private:
    /*!
     * Private instance - accessed as atomic operation.
     */
	static boost::atomic<RandomGenerator*> instance_;

	/*!
	 * Mutex used for instantiation of the instance.
	 */
	static boost::mutex instantiation_mutex;

	/*!
	 * Private constructor. Initialize pseudo-random generator and distributions parameters.
	 */
	RandomGenerator();

	/*!
	 * Random device used for generation of random numbers.
	 */
	std::random_device rd;

	/*!
	 *  Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits.
	 */
	std::mt19937_64 rng_mt19937_64;

	/// Uniform distribution from 0 to RAND_MAX (integers).
	std::uniform_int_distribution<> uniform_int_dist;

	/// Uniform distribution from 0 to 1 (real values).
	std::uniform_real_distribution<> uniform_real_dist;

	/// Normal distribution from 0 to 1 (real values).
	std::normal_distribution<> normal_real_dist;

};

/*!
 * \brief Macro returning random generator instance.
 * \author tkornuta
 */
#define RAN_GEN mic::utils::RandomGenerator::getInstance()


} /* namespace utils */
} /* namespace mic */

#endif /* SRC_DATA_UTILS_RANDOMGENERATOR_HPP_ */
