#pragma once

#include <cstdint>
#include <map>

namespace sc {

class IWriter {
public:
  virtual int Write(std::map<char, uint8_t> &) = 0;
};

} // namespace sc

// namespace sc {
//
// template <typename type> class IWriter {
// public:
//   virtual int Write(std::map<type, int> &buffer) = 0;
// };
//
// } // namespace sc
