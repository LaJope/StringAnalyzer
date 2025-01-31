#include <future>
#include <mutex>
#include <thread>

#include "Logger.h"
#include "Application.h"

// Application public

App::App(std::unique_ptr<sc::IReader> reader,
         std::unique_ptr<sc::IProcess> process,
         std::unique_ptr<sc::IWriter> writer)
    : m_reader(std::move(reader)), m_process(std::move(process)),
      m_writer(std::move(writer)) {}

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
  std::promise<int> processPromise, writePromise;
  auto processFuture = processPromise.get_future();
  auto writeFuture = writePromise.get_future();

  std::thread processThread(&App::processThread, this,
                            std::move(processPromise));
  std::thread writeThread(&App::writeThread, this, std::move(writePromise));

  if (int processError = processFuture.get(); processError != 0) {
    Logger::GetInstance().Error("Error with processing class");
    return processError;
  }
  if (int writeError = writeFuture.get(); writeError != 0) {
    Logger::GetInstance().Error("Error with writing class");
    return writeError;
  }

  processThread.join();
  writeThread.join();

  return 0;
}

// Application private

void App::processThread(std::promise<int> promise) {
  int err = 0;

  while (true) {
    std::string input = m_reader->Read();
    std::lock_guard<std::mutex> lock(m_buffer_lock);
    err = m_process->Process(input, m_buffer);
    if (err != 0) {
      m_buffer.clear();
      break;
    }
    m_buffer_full.notify_one();
  }
  promise.set_value(err);
}

void App::writeThread(std::promise<int> promise) {
  while (true) {
    std::unique_lock<std::mutex> lock(m_buffer_lock);
    m_buffer_full.wait(lock, [this]() { return !m_buffer.empty(); });

    m_writer->Write(m_buffer);
    m_buffer.clear();
  }
}

// template <typename type>
// App<type>::App(std::unique_ptr<Reader> reader, std::unique_ptr<Process>
// process,
//                std::unique_ptr<Writer> writer)
//     : m_reader(std::move(reader)), m_process(std::move(process)),
//       m_writer(std::move(writer)) {}
//
// template <typename type>
// void App<type>::setReader(std::unique_ptr<Reader> reader) {
//   m_reader = std::move(reader);
// }
//
// template <typename type>
// void App<type>::setProcess(std::unique_ptr<Process> process) {
//   m_process = std::move(process);
// }
//
// template <typename type>
// void App<type>::setWriter(std::unique_ptr<Writer> writer) {
//   m_writer = std::move(writer);
// }
//
// template <typename type> int App<type>::Start() {
//   std::thread processThread(&App::ProcessThread, this);
//   std::thread writeThread(&App::WriteThread, this);
//   processThread.join();
// }
//
// // Application private
//
// template <typename type> void App<type>::ProcessThread() {
//   while (true) {
//     type input = m_reader->Read();
//     std::lock_guard<std::mutex> lock(m_buffer_lock);
//     m_process->Process(m_buffer);
//   }
// }
// template <typename type> void App<type>::WriteThread() {
//   while (true) {
//     int err = m_writer->Write(m_buffer);
//   }
// }
