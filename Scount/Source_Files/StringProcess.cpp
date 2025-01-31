#include "StringProcess.h"

namespace sc {

int StringProcess::Process(std::string str, std::map<char, uint8_t> &buffer) {
  for (auto &elem : str) {
    buffer[elem] += 1;
  }
  return 0;
}

} // namespace sc
