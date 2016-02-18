/*!
 * \file Timer.hpp
 * \brief 
 * \author tkornut
 * \date Feb 17, 2016
 */

#ifndef SRC_DATA_UTILS_TIMER_HPP_
#define SRC_DATA_UTILS_TIMER_HPP_

namespace mic {
namespace data_utils {

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

}//: namespace data_utils
}//: namespace mic


#endif /* SRC_DATA_UTILS_TIMER_HPP_ */
