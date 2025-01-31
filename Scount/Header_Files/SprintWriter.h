#include <netinet/in.h>
#include <sys/socket.h>

#include "IWriter.h"

namespace sc {

class SprintWriter : public IWriter {
public:
  SprintWriter(int, std::string);
  ~SprintWriter();

  int Write(std::map<char, uint8_t> &) override;

private:
  int m_port;
  int m_serverSocket, m_clientSocket;
  sockaddr_in m_serverAddress;

  std::string m_sprintPath;

private:
  int startServer();
  int connectClient();

  int startSprint();

  bool checkConnection();
};

} // namespace sc
