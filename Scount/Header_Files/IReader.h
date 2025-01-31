#pragma once

#include <string>

namespace sc {

class IReader {
public:
  virtual std::string Read() = 0;
};

} // namespace sc

// namespace sc {
//
// template <typename type> class IReader {
// public:
//   virtual type Read() = 0;
// };
//
// } // namespace sc
