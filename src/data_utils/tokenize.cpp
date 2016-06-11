#include <data_utils/tokenize.hpp>
#include <logger/Log.hpp>

namespace mic {
  namespace data_utils {

    std::vector<size_t> tokenize_uint(std::string stringIn, char c = ' ') {
      std::vector<size_t> result;
      const char *str = stringIn.c_str();
      //LOG(LSTATUS) << "initial string = |" << *str << "|";
      while ((*str == c) && *str) {str++;} // jump over initial whitespace
      do {
        const char *begin = str;
        //LOG(LSTATUS) << "begin = |" << *begin << "|";
        while ((*str != c) && *str) {str++;} // find space at end of token
        std::string tempString(begin, str);
        result.push_back(atoi(tempString.c_str()));
        //LOG(LSTATUS) << "now string = |" << *str << "|, tempString = |" << tempString << "|";
        while ((*str == c) && *str) {str++;} // jump over extra whitespace between tokens
        if (*str) str--; // jump back so we're at next token, unless end-of-string
      } while (0 != *str++);
      return result;
    }

    std::vector<float> tokenize_float(std::string stringIn, char c = ' ') {
      std::vector<float> result;
      const char *str = stringIn.c_str();
      do {
        const char *begin = str;
        while (*str != c && *str) str++;
        std::string tempString(begin, str);
        result.push_back(atof(tempString.c_str()));
      } while (0 != *str++);
      return result;
    }


  }//: namespace data_utils
}//: namespace mic
