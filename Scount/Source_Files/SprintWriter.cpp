#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "Logger.h"
#include "SprintWriter.h"

namespace sc {

// SprintWriter public

SprintWriter::SprintWriter(int port, std::string sprintPath)
    : m_port(port), m_sprintPath(sprintPath) {
  startServer();
  startSprint();
  connectClient();
}
SprintWriter::~SprintWriter() { close(m_serverSocket); }

int SprintWriter::Write(std::map<char, uint8_t> &buffer) {
  if (!checkConnection()) {
    Logger::GetInstance().Warning(
        "Client socket was disconnected. Retrying...");
    startSprint();
    connectClient();
  }
  uint8_t alphabetSize = 26 * 2;
  int messageSize = alphabetSize * 2;
  uint8_t message[messageSize];
  int ind = 0;
  for (auto it = buffer.begin(); it != buffer.end(); it++) {
    if (ind + 2 >= buffer.size()) {
      Logger::GetInstance().Error("Buffer size too big");
      return 1;
    }
    message[ind++] = it->first;
    message[ind++] = it->second;
  }

  int bytesSent = send(m_clientSocket, message, sizeof(message), 0);
  if (bytesSent == -1) {
    Logger::GetInstance().Error("Could not send data to sprint");
    return 1;
  }
  if (bytesSent != messageSize) {
    Logger::GetInstance().Error("Lost " +
                                std::to_string(messageSize - bytesSent) +
                                " bytes out of " + std::to_string(messageSize));
    return 1;
  }
  Logger::GetInstance().Log("Sent message to sprint at port " +
                            std::to_string(m_port));
  return 0;
}

// SprintWriter private

int SprintWriter::startServer() {
  if (m_serverSocket)
    close(m_serverSocket);
  m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  m_serverAddress.sin_family = AF_INET;
  m_serverAddress.sin_port = htons(m_port);
  m_serverAddress.sin_addr.s_addr = INADDR_ANY;

  int err = bind(m_serverSocket, (struct sockaddr *)&m_serverAddress,
                 sizeof(m_serverAddress));
  if (!err) {
    Logger::GetInstance().Error("Could not bind socket at port " +
                                std::to_string(m_port) + ": " +
                                std::string(strerror(err)));
    return err;
  }
  Logger::GetInstance().Log("Binded server to port " + std::to_string(m_port));

  err = listen(m_serverSocket, 1);
  if (!err) {
    Logger::GetInstance().Error("Could not listen to port " +
                                std::to_string(m_port) + ": " +
                                std::string(strerror(err)));
    return err;
  }
  Logger::GetInstance().Log("Listening for clients at port " +
                            std::to_string(m_port));

  return 0;
}

int SprintWriter::connectClient() {
  // int err = listen(m_serverSocket, 1);
  // if (!err) {
  //   Logger::GetInstance().Error("Could not listen to port " +
  //                               std::to_string(m_port));
  //   return err;
  // }

  m_clientSocket = accept(m_serverSocket, nullptr, nullptr);

  Logger::GetInstance().Log("Client connected");

  return 0;
}

int SprintWriter::startSprint() {
  char *sprintCall;
  std::sprintf(sprintCall, "%s -p %d", m_sprintPath.c_str(), m_port);
  int err = system(sprintCall);
  if (!err) {
    Logger::GetInstance().Error("Could not start sprint program: " +
                                std::string(strerror(err)));
    return err;
  }
  Logger::GetInstance().Log("Started sprint program");

  return 0;
}

bool SprintWriter::checkConnection() {
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
  Logger::GetInstance().Log("Sprint program is still connected");

  return true;
}

} // namespace sc
