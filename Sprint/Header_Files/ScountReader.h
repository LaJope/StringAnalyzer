#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "IReader.h"

namespace sp {

class ScountReader : public IReader {
public:
  ScountReader(int);
  ~ScountReader();

public:
  int Read(std::vector<std::pair<char, uint8_t>> &) override;

private:
  int m_port;
  int m_clientSocket;

  const int m_maxReconectAttempts = 5;

private:
  int connectToServer();
  int reconnectToServer();

  bool checkConnection();
};

} // namespace sp
