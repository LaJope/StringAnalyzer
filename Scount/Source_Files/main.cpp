#include <memory>

#include "AppSettings.h"
#include "Application.h"
#include "HelpMessage.h"
#include "IProcess.h"
#include "IReader.h"
#include "IWriter.h"
#include "JsonWriter.h"
#include "Logger.h"
#include "SprintWriter.h"
#include "StdinReader.h"
#include "StringProcess.h"

int main(int argc, char *argv[]) {
  AppSettings settings(argc, argv);

  Logger::GetInstance().SetProgramName("Scount");

  if (settings.m_help) {
    Logger::GetInstance().Write(HelpMessage);
    return 0;
  }
  Logger::GetInstance().SetVerbose(settings.m_verbose);

  std::unique_ptr<sc::IReader> reader;
  std::unique_ptr<sc::IProcess> process;
  std::unique_ptr<sc::IWriter> writer;

  reader = std::make_unique<sc::StdinReader>();
  process = std::make_unique<sc::StringProcess>();
  if (settings.m_toJson) {
    writer = std::make_unique<sc::JsonWriter>(settings.m_outfile);
  } else {
    int err = 0;
    writer = std::make_unique<sc::SprintWriter>(
        settings.m_port, settings.m_sprintPath, err, settings.m_startSprint);
    if (err) {
      return 1;
    }
  }

  App application(std::move(reader), std::move(process), std::move(writer));
  int err = application.Start();

  return err;
}
