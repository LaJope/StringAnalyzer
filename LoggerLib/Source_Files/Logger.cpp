#include "Logger.h"
#include <iostream>
#include <ostream>

// Logger public

Logger &Logger::GetInstance() {
  static Logger logger;
  return logger;
}

void Logger::Write(const std::string &message...) {
  std::lock_guard<std::mutex> lock(m_write_lock);
  std::cout << message << std::flush;
}
void Logger::Log(const std::string &message) {
  if (m_verbose) {
    std::lock_guard<std::mutex> lock(m_write_lock);
    std::cout << "[LOG] " << message << std::endl;
  }
}
void Logger::Warning(const std::string &message) {
  if (m_verbose) {
    std::lock_guard<std::mutex> lock(m_write_lock);
    std::cerr << "[WARNING] " << message << std::endl;
  }
}

void Logger::Error(const std::string &message) {
  std::lock_guard<std::mutex> lock(m_write_lock);
  std::cerr << "[ERROR] " << message << std::endl;
}

void Logger::SetVerbose(bool verbose) { m_verbose = verbose; }
