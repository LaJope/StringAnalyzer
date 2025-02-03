#pragma once

#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>

#include "IProcess.h"
#include "IReader.h"
#include "IWriter.h"

class App {
public:
  App(std::unique_ptr<sc::IReader>, std::unique_ptr<sc::IProcess>,
      std::unique_ptr<sc::IWriter>);

  void SetReader(std::unique_ptr<sc::IReader>);
  void SetProcess(std::unique_ptr<sc::IProcess>);
  void SetWriter(std::unique_ptr<sc::IWriter>);

  int Start();

private:
  std::shared_ptr<sc::IReader> m_reader;
  std::shared_ptr<sc::IProcess> m_process;
  std::shared_ptr<sc::IWriter> m_writer;

  std::map<char, uint8_t> m_buffer;

  std::condition_variable m_buffer_full;
  std::mutex m_buffer_lock;

  bool quiting = false;

private:
  void processThread();
  void writeThread();
};
