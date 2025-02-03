#pragma once

#include <cstdint>
#include <vector>

namespace sp {

class IWriter {
public:
  virtual void Write(std::vector<std::pair<char, uint8_t>> &) = 0;
};

} // namespace sp
