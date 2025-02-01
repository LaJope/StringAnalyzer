#include <memory>
#include <mutex>
#include <thread>

#include "Application.h"
#include "Logger.h"
#include "SprintWriter.h"

// Application public

App::App(std::unique_ptr<sc::IReader> reader,
         std::unique_ptr<sc::IProcess> process,
         std::unique_ptr<sc::IWriter> writer)
    : m_reader(std::move(reader)), m_process(std::move(process)),
      m_writer(std::move(writer)) {}
App::~App() {}

void App::SetReader(std::unique_ptr<sc::IReader> reader) {
  m_reader = std::move(reader);
}

void App::SetProcess(std::unique_ptr<sc::IProcess> process) {
  m_process = std::move(process);
}

void App::SetWriter(std::unique_ptr<sc::IWriter> writer) {
  m_writer = std::move(writer);
}

int App::Start() {
  if (m_reader == nullptr || m_process == nullptr || m_writer == nullptr) {
    Logger::GetInstance().Error("Some of reader, process or writer is not "
                                "initialized corectly. Aborting");
    return 1;
  }

  std::thread processThread(&App::processThread, this);
  std::thread writeThread(&App::writeThread, this);

  processThread.join();
  writeThread.join();

  return 0;
}

// Application private

void App::processThread() {
  while (true) {
    std::string input = m_reader->Read(quiting);
    if (quiting)
      break;

    std::lock_guard<std::mutex> lock(m_buffer_lock);
    int err = m_process->Process(input, m_buffer);
    if (err != 0)
      break;
    
    m_buffer_full.notify_one();
  }
  quiting = true;
  m_buffer_full.notify_one();
  m_buffer.clear();
  Logger::GetInstance().Log("Exiting process thread...");
}

void App::writeThread() {
  while (true) {
    std::unique_lock<std::mutex> lock(m_buffer_lock);
    Logger::GetInstance().Log(
        "Waiting for process thread to release buffer...");
    m_buffer_full.wait(lock, [this]() { return !m_buffer.empty() || quiting; });
    if (quiting)
      break;
    
    Logger::GetInstance().Log("Got buffer to write");
    if (m_writer == nullptr)
      Logger::GetInstance().Log("Nullptr");

    m_writer->Write(m_buffer);
    m_buffer.clear();
  }
  Logger::GetInstance().Log("Exiting write thread...");
}
