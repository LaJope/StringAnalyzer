#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "AppSettings.h"
#include "HelpMessage.h"
#include "Logger.h"
#include "ScountReader.h"

int main(int argc, char *argv[]) {
  AppSettings settings(argc, argv);

  if (settings.m_help) {
    Logger::GetInstance().Write(HelpMessage, true);
    return 0;
  }

  Logger::GetInstance().SetVerbose(settings.m_verbose);

  Logger::GetInstance().SetProgramName("Sprint");
  Logger::GetInstance().Log("Sprint started");

  std::vector<std::pair<char, uint8_t>> buf;
  sp::ScountReader hello(settings.m_port);

  while (true) {
    int err = hello.Read(buf);
    if (err)
      break;

    std::sort(buf.begin(), buf.end(), [](const auto &lch, const auto &rch) {
      return lch.second < rch.second;
    });

    std::string wordOutput;
    wordOutput.reserve(buf.size());
    for (auto &elem : buf) {
      wordOutput.append(1, elem.first);
    }
    Logger::GetInstance().Write(wordOutput);

    buf.clear();
  }

  return 0;
}
