#ifndef  __SOCKET_FUN_H_
#define  __SOCKET_FUN_H_
#endif

/**
 * 非阻塞连接
 */
bool do_szhy_connect(int sock_fd, struct sockaddr* pser_addr, int* paddrlen);

/**
 * 非阻塞接收
 */
int do_szhy_recv(int sock_fd, char * recvbuf, int recvbuflen);

/**
 * 非阻塞发送
 */
int do_szhy_send(int sock_fd, char * sendbuf);