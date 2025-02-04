#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "AppSettings.h"
#include "Logger.h"

// AppSettings public

AppSettings::AppSettings(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    std::string opt = argv[i];

    if (opt[0] == '-' && opt.length() > 2 && opt[1] != '-') {
      Logger::GetInstance().Error(
          "Cannot combine multiple singular flags in one. Ignoring " + opt +
          " flag...");
      continue;
    }

    if (auto j = NoArgs.find(opt); j != NoArgs.end()) {
      j->second();
      continue;
    }

    if (auto k = OneArgs.find(opt); k != OneArgs.end()) {
      if (++i < argc)
        k->second(argv[i]);
      else
        throw std::runtime_error{"Missing param after " + opt};
      continue;
    }

    Logger::GetInstance().Error("Unrecognized command-line option " + opt +
                                ". Ignoring...");
  }
}

// AppSettings private

void AppSettings::setHelpToTrue() { m_help = true; }
void AppSettings::setVerboseToTrue() { m_verbose = true; }
void AppSettings::setToJsonToTrue() { m_toJson = true; }
void AppSettings::setStartSprintToTrue() { m_startSprint = true; }
void AppSettings::setOutfile(std::string fileName) {
  m_outfile = fileName.substr(0, fileName.find("."));
  setToJsonToTrue();
}
void AppSettings::setPort(int port) {
  if (!m_toJson) {
    m_port = port;
    return;
  }
  Logger::GetInstance().Error("You cannot write to json and use sprint at the "
                              "same time. Ignoring port argument");
}
void AppSettings::setSprintPath(std::string path) {
  if (!m_toJson) {
    m_sprintPath = path;
    return;
  }
  Logger::GetInstance().Error("You cannot write to json and use sprint at the "
                              "same time. Ignoring path argument");
}
