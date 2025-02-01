
#include "JsonWriter.h"
#include "Logger.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <string>

namespace sc {

// JsonWriter public

JsonWriter::JsonWriter(std::string filename) : m_filename(filename) {
  std::ofstream jsonFile(m_filename + ".json");
  if (!jsonFile.is_open()) {
    Logger::GetInstance().Error("Could not open file with name " + m_filename);
  }
  jsonFile << "[\n";
  jsonFile.close();
}

JsonWriter::~JsonWriter() {
  std::ofstream jsonFile(m_filename + ".json", std::ios_base::app);
  if (!jsonFile.is_open()) {
    Logger::GetInstance().Error("Could not open file with name " + m_filename);
  }
  jsonFile << "]";
  jsonFile.close();
}

int JsonWriter::Write(std::map<char, uint8_t> buffer) {
  std::ofstream jsonFile(m_filename + ".json", std::ios_base::app);
  if (!jsonFile.is_open()) {
    Logger::GetInstance().Error("Could not open file with name " + m_filename);
    return 1;
  }
  jsonFile << "{\n";
  for (auto it = buffer.begin(); it != buffer.end(); it++) {
    char *jsonRow = (char *)malloc(sizeof(char) * 10);
    sprintf(jsonRow, "\"%c\": %d,", it->first, it->second);
    jsonFile << jsonRow << std::endl;
  }
  jsonFile << "},\n";
  jsonFile.close();
  return 0;
}

void JsonWriter::SetFilename(std::string filename) { m_filename = filename; }

} // namespace sc
