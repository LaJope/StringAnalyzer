#include "Logger.h"
#include <iostream>
#include <ostream>

// Logger public

Logger &Logger::GetInstance() {
  static Logger logger;
  return logger;
}

void Logger::Write(const std::string &message, bool noNewline) {
  std::lock_guard<std::mutex> lock(m_write_lock);
  std::cout << message;
  if (noNewline)
    std::cout << std::flush;
  else
    std::cout << std::endl;
}
void Logger::Log(const std::string &message) {
  if (m_verbose) {
    std::lock_guard<std::mutex> lock(m_write_lock);
    std::cout << "[LOG at " << m_progName << "] " << message << std::endl;
  }
}
void Logger::Warning(const std::string &message) {
  if (m_verbose) {
    std::lock_guard<std::mutex> lock(m_write_lock);
    std::cerr << "[WARNING at " << m_progName << "] " << message << std::endl;
  }
}

void Logger::Error(const std::string &message) {
  std::lock_guard<std::mutex> lock(m_write_lock);
  std::cerr << "[ERROR at " << m_progName << "] " << message << std::endl;
}

void Logger::SetVerbose(bool verbose) { m_verbose = verbose; }
void Logger::SetProgramName(const std::string &name) { m_progName = name; }

bool Logger::GetVerbose() { return m_verbose; }
