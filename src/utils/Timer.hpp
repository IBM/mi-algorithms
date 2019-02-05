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
 * \file Timer.hpp
 * \brief 
 * \author tkornut
 * \date Feb 17, 2016
 */

#ifndef SRC_DATA_UTILS_TIMER_HPP_
#define SRC_DATA_UTILS_TIMER_HPP_

namespace mic {
namespace utils {

/*!
 * \brief Timer class.
 * \author krocki
 */
class Timer {

	public:

		Timer() = default;
		~Timer() = default;

		void start(void) {

			gettimeofday(&s, NULL);
		}

		double end(void) {

		    struct timeval  diff_tv;
		    gettimeofday(&e, NULL);

		    diff_tv.tv_usec = e.tv_usec - s.tv_usec;
		    diff_tv.tv_sec = e.tv_sec - s.tv_sec;

		    if (s.tv_usec > e.tv_usec) {
		        diff_tv.tv_usec += 1000000;
		        diff_tv.tv_sec--;
		    }

		    return (double) diff_tv.tv_sec + ( (double) diff_tv.tv_usec / 1000000.0);

		}

	protected:

		struct timeval s;
		struct timeval e;
};

}//: namespace utils
}//: namespace mic


#endif /* SRC_DATA_UTILS_TIMER_HPP_ */
