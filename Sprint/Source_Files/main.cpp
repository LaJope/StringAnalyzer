#include <iostream>
#include <netinet/in.h>

#include "AppSettings.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  AppSettings settings(argc, argv);

  int sockfd, portno, n;
  sockaddr_in serv_addr;
  struct hostent *server;

  uint8_t alphabetSize = 26 * 2;
  int messageSize = alphabetSize * 2;
  uint8_t message[messageSize];
  std::memset(message, 0, messageSize);
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }
  portno = settings.m_port;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    std::cout << "ERROR opening socket\n";
  // server = gethostbyname(argv[1]);
  // if (server == NULL) {
  //   fprintf(stderr, "ERROR, no such host\n");
  //   exit(0);
  // }
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  // bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
  //       server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    std::cout << "ERROR connecting\n";
  printf("Please enter the message: ");
  n = write(sockfd, message, messageSize);
  if (n < 0)
    std::cout << "ERROR writing to socket\n";
  bzero(message, messageSize);
  n = read(sockfd, message, messageSize - 1);
  if (n < 0)
    std::cout << "ERROR reading from socket\n";
  int ind = 0;
  for (uint8_t &elem : message) {
    if (ind) {
      std::cout << char(elem) << " ";
      ind++;
    } else {
      std::cout << elem << "\n";
      ind--;
    }
  }
  printf("%s\n", message);
  close(sockfd);
  return 0;

  return 0;
}
