#include "StringProcess.h"
#include "Logger.h"

namespace sc {

int StringProcess::Process(std::string str, std::map<char, uint8_t> &buffer) {
  Logger::GetInstance().Log("Starting processing string: " + str);
  for (auto &elem : str) {
    buffer[elem] += 1;
  }
  Logger::GetInstance().Log("Ended processing of string: " + str);
  return 0;
}

} // namespace sc
