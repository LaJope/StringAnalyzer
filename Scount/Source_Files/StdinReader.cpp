#include <iostream>
#include <regex>
#include <string>

#include "Logger.h"
#include "StdinReader.h"

namespace sc {

StdinReader::StdinReader() {
  Logger::GetInstance().Write(
      "You can begin entering strings (up to 64 symbols)( - to quit)");
}

std::string StdinReader::Read(bool &quit) {
  std::string input;

  const std::regex pattern("[a-zA-Z]+");

  while (true) {
    std::getline(std::cin, input);

    if (input == "-") {
      Logger::GetInstance().Log("Quiting...");
      quit = true;
      return "None";
    }

    if (input.empty() || 64 < input.size()) {
      Logger::GetInstance().Error(
          "Size of the input string must be in range [1, 64]");
      continue;
    }

    if (!std::regex_match(input, pattern)) {
      Logger::GetInstance().Error(
          "Input must ocntain only characters of latin alphabet");
      continue;
    }
    break;
  }
  Logger::GetInstance().Log("Got user input. Size: " +
                            std::to_string(input.length()));
  return input;
}

} // namespace sc
