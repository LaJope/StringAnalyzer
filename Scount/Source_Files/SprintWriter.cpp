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

SprintWriter::SprintWriter(int port, std::string sprintPath, int &err,
                           bool startSprint)
    : m_port(port), m_startSprint(startSprint) {
  if (m_startSprint) {
    Logger::GetInstance().Log("Searching for sprint");
    err = findSprintPath(sprintPath);
    if (err)
      return;
    Logger::GetInstance().Log("Starting sprint");
    startSprintProgram();
  }
  Logger::GetInstance().Log("Starting server");
  err = startServer();
  if (err)
    return;
}
SprintWriter::~SprintWriter() {
  if (m_startSprint) {
    int alphabetSize = 26 * 2;
    int messageSize = alphabetSize * 2;
    uint8_t message[messageSize];
    std::memset(message, 1, messageSize);

    int bytesSent = send(m_clientSocket, message, sizeof(message), 0);
    if (bytesSent == -1) {
      Logger::GetInstance().Error("Could not send data to sprint");
    }
  }

  close(m_serverSocket);
  close(m_clientSocket);
}

// int SprintWriter::Write(std::map<char, uint8_t> buffer) { return 0; }
int SprintWriter::Write(std::map<char, uint8_t> buffer) {
  Logger::GetInstance().Log("Sending buffer to client");
  if (!checkConnection()) {
    Logger::GetInstance().Warning("Client socket was disconnected");
    if (m_startSprint) {
      Logger::GetInstance().Warning("Restarting sprint");
      startSprintProgram();
    }
    connectClient();
  }
  int alphabetSize = 26 * 2;
  int messageSize = alphabetSize * 2;
  uint8_t message[messageSize];
  std::memset(message, 0, messageSize);
  int ind = 0;
  for (auto it = buffer.begin(); it != buffer.end(); it++) {
    if (ind + 1 >= messageSize) {
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

bool SprintWriter::checkPath(std::string path) {
  return access(path.c_str(), F_OK) != -1;
}

int SprintWriter::findSprintPath(std::string path) {
  if (path.find("/") != path.npos && checkPath(path)) {
    m_sprintPath = path;
    Logger::GetInstance().Log("Found sprint path: " + m_sprintPath);
    return 0;
  }
  if (checkPath("./" + m_sprintPath)) {
    m_sprintPath = "./" + m_sprintPath;
    Logger::GetInstance().Log("Found sprint relative path: " + m_sprintPath);
    return 0;
  }
  if (!system((m_sprintPath + " --help >/dev/null 2>/dev/null").c_str())) {
    Logger::GetInstance().Log("Found sprint in PATH");
    return 0;
  }
  Logger::GetInstance().Error("Could not find sprint in current directory/PATH "
                              "or at provided path. Aborting...");
  return 1;
}

int SprintWriter::startServer() {
  if (m_serverSocket) {
    close(m_serverSocket);
  }
  m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (m_serverSocket < 0) {
    Logger::GetInstance().Error("Could not open socket. Aborting...");
    return 1;
  }
  int opt = 1;
  int err =
      setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (err < 0) {
    Logger::GetInstance().Error("Could not set socket option. Aborting...");
    return 1;
  }

  m_serverAddress.sin_family = AF_INET;
  m_serverAddress.sin_port = htons(m_port);
  m_serverAddress.sin_addr.s_addr = INADDR_ANY;

  err = bind(m_serverSocket, (struct sockaddr *)&m_serverAddress,
             sizeof(m_serverAddress));
  if (err) {
    Logger::GetInstance().Error("Could not bind socket at port " +
                                std::to_string(m_port));
    return err;
  }
  Logger::GetInstance().Log("Binded server to port " + std::to_string(m_port));

  err = listen(m_serverSocket, 1);
  if (err) {
    Logger::GetInstance().Error("Could not listen to port " +
                                std::to_string(m_port));
    return err;
  }
  Logger::GetInstance().Log("Listening for clients at port " +
                            std::to_string(m_port));

  return connectClient();
}

int SprintWriter::connectClient() {
  Logger::GetInstance().Write("Waiting for client to connect");
  if (m_clientSocket)
    close(m_clientSocket);

  socklen_t clilen = sizeof(m_clientAddress);
  m_clientSocket =
      accept(m_serverSocket, (struct sockaddr *)&m_clientAddress, &clilen);

  if (m_clientSocket < 0) {
    Logger::GetInstance().Error("Could not connect to any client. Aborting...");
    return 1;
  }

  Logger::GetInstance().Write("Client connected");

  return 0;
}

int SprintWriter::startSprintProgram() {
  char sprintCall[256];
  std::sprintf(sprintCall, "%s --port %d %s &", m_sprintPath.c_str(), m_port,
               (Logger::GetInstance().GetVerbose() ? "--verbose" : ""));
  int err = system(sprintCall);
  if (err) {
    Logger::GetInstance().Error("Could not start sprint program: " +
                                std::string(strerror(err)));
    return err;
  }
  Logger::GetInstance().Log("Started sprint program");

  return 0;
}

bool SprintWriter::checkConnection() {
  char buffer[1];
  int retval =
      recv(m_clientSocket, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT);
  if (retval == 0) {
    Logger::GetInstance().Error("Client disconnected");
    return false;
  }
  if (retval < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      Logger::GetInstance().Log("Sprint program is still connected");
      return true;
    } else {
      Logger::GetInstance().Error("Socket error: " +
                                  std::string(strerror(errno)));
      Logger::GetInstance().Error("Client disconnected");
      return false;
    }
  }
  Logger::GetInstance().Log("Sprint program is still connected");
  return true;
}

} // namespace sc
