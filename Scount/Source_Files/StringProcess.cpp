#include "StringProcess.h"
#include "Logger.h"
#include <string>

namespace sc {

int StringProcess::Process(std::string str, std::map<char, uint8_t> &buffer) {
  Logger::GetInstance().Log("Starting processing string: " + str);
  for (auto &elem : str) {
    buffer[elem] += 1;
  }

  int ind = 0;
  std::string output;
  for (auto it = buffer.begin(); it != buffer.end(); it++) {
    ind++;
    char row[10];
    sprintf(row, "{%c : %d}%s", it->first, it->second,
            (ind % 5 == 0 ? "\n" : " "));

    output += row;
  }
  if (ind % 5 != 0)
    output += "\n";
  Logger::GetInstance().Write(output, true);
  Logger::GetInstance().Log("Ended processing of string: " + str);
  return 0;
}

} // namespace sc
