#pragma once

#include <cstdint>
#include <vector>

namespace sp {

class IReader {
public:
  virtual int Read(std::vector<std::pair<char, uint8_t>> &) = 0;
  virtual ~IReader() {};
};

} // namespace sp
