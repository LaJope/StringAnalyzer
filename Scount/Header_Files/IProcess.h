#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace sc {

class IProcess {
public:
  virtual int Process(std::string, std::map<char, uint8_t> &) = 0;
};

} // namespace sc

// namespace sc {
//
// template <typename type> class IProcess {
// public:
//   virtual int Process(std::map<type, int> &buffer) = 0;
// };
//
// }// namespace sc
