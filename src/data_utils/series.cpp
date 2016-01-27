#include <data_utils/series.hpp>

namespace mic {
namespace data_utils {


float population_equation(float x, float t) {

	return t * x * (1 - x);

}

}//: namespace data_utils
}//: namespace mic
