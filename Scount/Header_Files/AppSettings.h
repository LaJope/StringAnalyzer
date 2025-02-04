#pragma once

#include <cstdlib>
#include <functional>
#include <string>
#include <unordered_map>

class AppSettings {
public:
  bool m_help = false;
  bool m_verbose = false;

  bool m_toJson = false;

  bool m_startSprint = false;

  std::string m_outfile = "scount";

  int m_port = 8060;
  std::string m_sprintPath = "./sprint";

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

      {"-j", [this]() { setToJsonToTrue(); }},
      {"--to-json", [this]() { setToJsonToTrue(); }},

      {"-S", [this]() { setStartSprintToTrue(); }},
      {"--start-sprint", [this]() { setStartSprintToTrue(); }},
  };

  const std::unordered_map<std::string, OneArgHandle> OneArgs{
      {"-o", [this](std::string v) { setOutfile(v); }},
      {"--outfile", [this](std::string v) { setOutfile(v); }},

      {"-p", [this](std::string v) { setPort(std::atoi(v.c_str())); }},
      {"--port", [this](std::string v) { setPort(std::atoi(v.c_str())); }},

      {"-s",
       [this](std::string v) {
         setStartSprintToTrue();
         setSprintPath(v);
       }},
      {"--sprint-path",
       [this](std::string v) {
         setStartSprintToTrue();
         setSprintPath(v);
       }},
  };

private:
  void setHelpToTrue();
  void setVerboseToTrue();
  void setToJsonToTrue();
  void setStartSprintToTrue();
  void setOutfile(std::string);
  void setPort(int);
  void setSprintPath(std::string);
};
