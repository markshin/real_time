#ifndef PALS__PALS_SOCKET_H
#define PALS__PALS_SOCKET_H

int pals_open_tx_socket();
int pals_open_rx_socket(char *ipaddr, int port);

int pals_socket_sendto(int sock, void *buf, int bufferlen, char* str_addr, int port);
int pals_socket_recvfrom(int sock, void *buf, int bufferlen);

#endif /* PALS__PALS_SOCKET_H */
