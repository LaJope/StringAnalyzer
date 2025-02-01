#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

#include "IWriter.h"

namespace sc {

class SprintWriter : public IWriter {
public:
  SprintWriter(int, std::string, int &);
  ~SprintWriter();

  int Write(std::map<char, uint8_t>) override;

private:
  int m_port;
  int m_serverSocket = 0, m_clientSocket = 0;
  sockaddr_in m_serverAddress, m_clientAddress;

  std::string m_sprintPath = "sprint";

private:
  static bool checkPath(std::string);
  int findSprintPath(std::string);

  int startServer();
  int connectClient();

  int startSprint();

  bool checkConnection();
};

} // namespace sc
