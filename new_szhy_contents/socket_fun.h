#ifndef  __SOCKET_FUN_H_
#define  __SOCKET_FUN_H_
#endif

/**
 * ����������
 */
bool do_szhy_connect(int sock_fd, struct sockaddr* pser_addr, int* paddrlen);

/**
 * ����������
 */
int do_szhy_recv(int sock_fd, char * recvbuf, int recvbuflen);

/**
 * ����������
 */
int do_szhy_send(int sock_fd, char * sendbuf);