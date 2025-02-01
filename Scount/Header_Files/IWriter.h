#pragma once

#include <cstdint>
#include <map>

namespace sc {

class IWriter {
public:
  virtual int Write(std::map<char, uint8_t>) = 0;
  virtual ~IWriter(){};
};

} // namespace sc
