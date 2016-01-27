#ifndef __TIME_SERIES_H__
#define __TIME_SERIES_H__

namespace mic {
namespace data_utils {

#ifdef __cplusplus 
extern "C" {
#endif

float population_equation(float t, float x);

#ifdef __cplusplus
}
#endif

}//: namespace data_utils
}//: namespace mic

#endif
