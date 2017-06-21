#include "pals_socket.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

int pals_open_tx_socket() {
  
  int sockfd;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  return sockfd;
}
//char *ipaddr unused
int pals_open_rx_socket(char *ipaddr, int port) {

  int sockfd;
  struct sockaddr_in socketaddr;
  int flag;
  unsigned int optval = 1;

  bzero(&socketaddr, sizeof(struct sockaddr_in));

  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  //non blocking socket
  flag = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flag | O_NONBLOCK);

  socketaddr.sin_family = AF_INET;
  socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  socketaddr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr *)&socketaddr, sizeof(struct sockaddr_in)) < 0) {
    // bind error occurred
    return -1;
  }

  return sockfd;
}

// TODO: use this to implement pals_send and pals_recv
int pals_socket_sendto(int sock, void *buf, int bufferlen, char* str_addr, int port) {
  
  struct sockaddr_in socketaddr;
  
  bzero(&socketaddr, sizeof(struct sockaddr_in));
  
  socketaddr.sin_family = AF_INET;
  socketaddr.sin_addr.s_addr = inet_addr(str_addr); 
  socketaddr.sin_port = htons(port);
  
  return sendto(sock, buf, bufferlen, 0, (struct sockaddr *)&socketaddr, sizeof(socketaddr));
}

int pals_socket_recvfrom(int sock, void *buf, int bufferlen) {
  //sockaddr_in is redundant
  struct sockaddr_in addr;

  socklen_t socketlen;
  socketlen = sizeof(struct sockaddr_in);

  return recvfrom(sock, buf, bufferlen, 0, (struct sockaddr *)&addr, &socketlen);
}
