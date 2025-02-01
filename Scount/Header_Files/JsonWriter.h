#pragma once

#include <string>

#include "IWriter.h"

namespace sc {

class JsonWriter : public IWriter {
public:
  JsonWriter(std::string);
  ~JsonWriter();

  int Write(std::map<char, uint8_t>) override;

  void SetFilename(std::string);

private:
  std::string m_filename;
};

} // namespace sc
