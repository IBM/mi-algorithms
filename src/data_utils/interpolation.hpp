#ifndef __INTERPOLATION_H__
#define __INTERPOLATION_H__

namespace mic {
namespace data_utils {

#ifdef __cplusplus 
extern "C" {
#endif

double linear_interpolation(double value, double x0, double x1);
double bilinear_interpolation(double value_x, double value_y, double ul, double ur, double ll, double lr);

#ifdef __cplusplus
}
#endif

#endif


}//: namespace data_utils
}//: namespace mic

