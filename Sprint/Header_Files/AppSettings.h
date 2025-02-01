#pragma once

#include <cstdlib>
#include <functional>
#include <string>
#include <unordered_map>

class AppSettings {
public:
  bool m_help = false;
  bool m_verbose = false;

  int m_port = 6060;

public:
  AppSettings(int argc, char *argv[]);

private:
  using NoArgHandle = std::function<void()>;
  using OneArgHandle = std::function<void(std::string)>;

  const std::unordered_map<std::string, NoArgHandle> NoArgs{
      {"-h", [this]() { setHelpToTrue(); }},
      {"--help", [this]() { setHelpToTrue(); }},

      {"-v", [this]() { setVerboseToTrue(); }},
      {"--verbose", [this]() { setVerboseToTrue(); }},
  };

  const std::unordered_map<std::string, OneArgHandle> OneArgs{
      {"-p", [this](std::string v) { setPort(std::atoi(v.c_str())); }},
      {"--port", [this](std::string v) { setPort(std::atoi(v.c_str())); }},
  };

private:
  void setHelpToTrue();
  void setVerboseToTrue();
  void setPort(int);
};
