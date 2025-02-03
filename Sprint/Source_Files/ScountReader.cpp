#include <cstdio>
#include <cstring>
#include <string>

#include "Logger.h"
#include "ScountReader.h"

namespace sp {

ScountReader::ScountReader(int port) : m_port(port) { connectToServer(); }

ScountReader::~ScountReader() { close(m_clientSocket); }

int ScountReader::Read(std::vector<std::pair<char, uint8_t>> &buffer) {
  int alphabetSize = 26 * 2;
  int messageSize = alphabetSize * 2;
  uint8_t message[messageSize];

  std::memset(message, 0, messageSize);

  int bytesRead = recv(m_clientSocket, message, sizeof(message), 0);
  if (bytesRead == -1) {
    Logger::GetInstance().Error("Could not read from socket");
    return 1;
  }

  if (message[0] == '\0') {
    Logger::GetInstance().Log("No data recieved. Exiting...");
    return -1;
  }

  Logger::GetInstance().Log("Recieved " + std::to_string(bytesRead) +
                              " bytes out of " + std::to_string(messageSize));

  for (int ind = 0; ind < bytesRead - int(bytesRead % 2); ind += 2) {
    if (message[ind] == '\0')
      break;
    buffer.push_back({char(message[ind]), uint8_t(message[ind + 1])});
  }

  return 0;
}

int ScountReader::connectToServer() {
  m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(m_port);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  int err = connect(m_clientSocket, (struct sockaddr *)&serverAddress,
                    sizeof(serverAddress));
  if (err) {
    Logger::GetInstance().Error("Could not connect to server");
    return err;
  }

  Logger::GetInstance().Log("Connected");

  return 0;
}

} // namespace sp
