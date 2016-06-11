#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__

#include <string>
#include <vector>

namespace mic {
  namespace data_utils {

    std::vector<size_t> tokenize_uint(std::string str, char c);
    std::vector<float> tokenize_float(std::string str, char c);

  }//: namespace data_utils
}//: namespace mic

#endif  /* __TOKENIZE_H__ */
