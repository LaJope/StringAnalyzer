#include <iostream>
#include <regex>
#include <string>

#include "Logger.h"
#include "StdinReader.h"

namespace sc {

std::string StdinReader::Read() {
  std::string input;

  const std::regex pattern("[a-zA-Z]+");

  while (true) {
    Logger::GetInstance().Write("Enter string (up to 64 symbols): ");
    std::getline(std::cin, input);

    if (!input.empty() && input.size() <= 64 &&
        std::regex_match(input, pattern))
      break;

    Logger::GetInstance().Error(
        "Size of the input string must be in range [1, 64]");
  }
  return input;
}

} // namespace sc
