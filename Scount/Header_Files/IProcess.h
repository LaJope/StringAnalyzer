#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace sc {

class IProcess {
public:
  virtual int Process(std::string, std::map<char, uint8_t> &) = 0;

  virtual ~IProcess(){};
};

} // namespace sc
