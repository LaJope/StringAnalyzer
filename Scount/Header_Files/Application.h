#pragma once

#include <condition_variable>
#include <future>
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

private:
  void processThread(std::promise<int> error);
  void writeThread(std::promise<int> error);
};

// template <class type> class App {
//   using Reader = sc::IReader<type>;
//   using Process = sc::IProcess<type>;
//   using Writer = sc::IWriter<type>;
//
// public:
//   App<type>(std::unique_ptr<Reader>, std::unique_ptr<Process>,
//             std::unique_ptr<Writer>);
//
//   void setReader(std::unique_ptr<Reader>);
//   void setProcess(std::unique_ptr<Process>);
//   void setWriter(std::unique_ptr<Writer>);
//
//   int Start();
//
// private:
//   std::shared_ptr<Reader> m_reader;
//   std::shared_ptr<Process> m_process;
//   std::shared_ptr<Writer> m_writer;
//
//   std::map<type, int> m_buffer;
//
//   std::mutex m_buffer_lock;
//
// private:
//   void ProcessThread();
//   void WriteThread();
// };

