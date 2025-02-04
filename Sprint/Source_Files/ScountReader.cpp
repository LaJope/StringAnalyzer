#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>

#include "Logger.h"
#include "ScountReader.h"

namespace sp {

ScountReader::ScountReader(int port) : m_port(port) {
  int err = connectToServer();
  if (err) {
    Logger::GetInstance().Error("Could not connect to server");
    reconnectToServer();
  }
}

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

  if (message[0] == 1) {
    Logger::GetInstance().Log("Recieved termination symbol. Exiting...");
    return 1;
  }

  if (message[0] == 0) {
    Logger::GetInstance().Error("Disconnected from server");
    int err = reconnectToServer();
    if (err)
      return -1;
  }

  Logger::GetInstance().Log("Recieved " + std::to_string(bytesRead) +
                            " bytes out of " + std::to_string(messageSize));

  for (int ind = 0; ind < bytesRead - int(bytesRead % 2); ind += 2) {
    if (message[ind] == 0)
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
    return 1;
  }

  Logger::GetInstance().Log("Connected");

  return 0;
}

int ScountReader::reconnectToServer() {
  Logger::GetInstance().Log("Trying to reconnect");
  uint32_t reconnectSeconds = 10;

  for (int atempt = 0; atempt < m_maxReconectAttempts; atempt++) {
    Logger::GetInstance().Error(
        "Retrying in " + std::to_string(reconnectSeconds) + " seconds...");
    std::this_thread::sleep_for(std::chrono::seconds(reconnectSeconds));
    int err = connectToServer();
    if (!err)
      return 0;
  }
  Logger::GetInstance().Error("Server is unreachable. Exiting...");
  return 1;
}

bool ScountReader::checkConnection() {
  int error = 0;
  socklen_t len = sizeof(error);
  int retval = getsockopt(m_clientSocket, SOL_SOCKET, SO_ERROR, &error, &len);
  if (retval != 0) {
    Logger::GetInstance().Error("Could not get socket error code: " +
                                std::string(strerror(retval)));
    return false;
  }

  if (error != 0) {
    Logger::GetInstance().Error("Socket error: " +
                                std::string(strerror(error)));
    return false;
  }
  Logger::GetInstance().Log("Still connected to server");

  return true;
}

} // namespace sp
