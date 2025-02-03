#pragma once

#include "IReader.h"
#include "IWriter.h"

#include <memory>
class App {
public:
  App(std::unique_ptr<sp::IReader>, std::unique_ptr<sp::IWriter>);

  int Start();

  void SetReader(std::unique_ptr<sp::IReader>);
  void SetWriter(std::unique_ptr<sp::IWriter>);

private:
  std::shared_ptr<sp::IReader> m_reader;
  std::shared_ptr<sp::IWriter> m_writer;

private:
};
