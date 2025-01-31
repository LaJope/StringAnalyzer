#pragma once

#include <mutex>
#include <string>

class Logger {
public:
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  static Logger &GetInstance();

  void Write(const std::string &message...);
  void Log(const std::string &message);
  void Warning(const std::string &message);
  void Error(const std::string &message);

  void SetVerbose(bool verbose);

private:
  std::mutex m_write_lock;
  bool m_verbose;

private:
  Logger() : m_verbose(false) {}
};
