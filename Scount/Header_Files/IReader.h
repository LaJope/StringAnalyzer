#pragma once

#include <string>

namespace sc {

class IReader {
public:
  virtual std::string Read(bool &) = 0;
  virtual ~IReader() {}
};

} // namespace sc
