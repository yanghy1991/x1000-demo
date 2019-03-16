/*clent.c*/
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/select.h> 
#include <sys/times.h> 

#define MAX_CONNECT_TIMES 5

/**
 * 非阻塞连接
 */
bool do_bdasr_connect(int sock_fd, struct sockaddr* pser_addr, int* paddrlen)
{
    if(connect(sock_fd, pser_addr, *paddrlen) < 0)
    {
        if(errno == EISCONN)
        {
			printf("%s:%d, connect socket completed\n", __FILE__, __LINE__);
			return true;
        }
        if(errno != EINPROGRESS && errno != EALREADY && errno != EWOULDBLOCK)
        {
            printf("%s:%d, connect socket failed\n", __FILE__, __LINE__);
            return false;
        }
		else
        {
            printf("%s:%d, connect socket does not completed\n", __FILE__, __LINE__);
        }
    }
    else
    {
        printf("%s:%d, connect socket completed\n", __FILE__, __LINE__);
		return true;
    }
	
    fd_set fds_red, fds_write;
    struct timeval tval;
    int selret = 0;
    tval.tv_sec = 3;
    tval.tv_usec = 0;
    int ntrytimes = 0;
	
    while(1 && ntrytimes < MAX_CONNECT_TIMES)
    {
	    FD_ZERO(&fds_red);
	    FD_SET(sock_fd, &fds_red);
	    
	    FD_ZERO(&fds_write);
	    FD_SET(sock_fd, &fds_write);
//	    printf("%s:%d, before select\n", __FILE__, __LINE__);
	    
	    selret = select(sock_fd + 1, &fds_red, &fds_write, NULL, &tval);
	    
//	    printf("%s:%d, after select\n", __FILE__, __LINE__);
	    if(selret < 0)
	    {
			if(errno == EINTR)
			{
				ntrytimes++;
				continue;
			}
			else
			{
				printf("%s:%d, select failed,errno = %d\n", __FILE__, __LINE__,errno);
		        return false;
			}
	    }
	    else if(selret == 0)
        {
	        printf("%s:%d, connect socket timeout\n", __FILE__, __LINE__);
	        ntrytimes++;
			continue;
	    }
	    else
	    {
//	        printf("%s:%d, select default\n", __FILE__, __LINE__);
	    	 
	        if(FD_ISSET(sock_fd, &fds_red) || FD_ISSET(sock_fd, &fds_write))
	        {
				int error = 0;
	            int len = sizeof(error);
	            int rc = getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, (void *) &error, &len);
	            if(rc == -1)
	            {
	                printf("%s:%d, connection is closed\n", __FILE__, __LINE__);
	                return false;
	            }
	            else if(error)
	            {
	                printf("%s:%d, connection is closed\n", __FILE__, __LINE__);
	                return false;
	            }
	            else
	            {
	                printf("%s:%d, connection is ok\n", __FILE__, __LINE__);
					return true;
	            }
	            
	        }
	        else
	        {
	            printf("%s:%d, no descriptor is ready\n", __FILE__, __LINE__);
				ntrytimes++;
				continue;
	        }
	    }
    }
	
    return false;	
}

/**
 * 非阻塞接收
 */
//return value, -1 means Recv happs error; 0 means timeout or be interupted; > 0 means ok
int do_bdasr_recv(int sock_fd, char * recvbuf, int recvbuflen)
{
    fd_set fds_red;
    struct timeval tval;
    int selret = 0;
    tval.tv_sec = 3;
    tval.tv_usec = 0;
	int ntrytimes = 0;
	
    while(1 && ntrytimes < MAX_CONNECT_TIMES)
    {
       //we must clear fds for every loop, otherwise can not check the change of descriptor
		FD_ZERO(&fds_red);
		FD_SET(sock_fd, &fds_red);
//		printf("%s:%d, before select\n", __FILE__, __LINE__);
	
		selret = select(sock_fd + 1, &fds_red, NULL, NULL, &tval);
	   
//		printf("%s:%d, after select\n", __FILE__, __LINE__);
		if(selret < 0)
		{
			if(errno == EINTR)
			{
				ntrytimes++;
				continue;
			}
			else
			{
				printf("%s:%d, select failed\n", __FILE__, __LINE__);
                return -1;
			}
		}
		else if(selret == 0)
		{
			printf("%s:%d, select timeout, no descriptors can be read or written\n", __FILE__, __LINE__);
			ntrytimes++;
			continue;
		}
		else
		{
//			printf("%s:%d, select default\n", __FILE__, __LINE__);
   		 
            if(FD_ISSET(sock_fd, &fds_red))
            {
//				printf("%s:%d, receive data\n", __FILE__, __LINE__);
   	            bool brecvres = true;
                    //receive data
				int recvlen = 0;
				int retlen = 0;
				char *ptr = recvbuf;
				int leftlen = recvbuflen -1;
				int recv_times = 0;
				do
				{
//					printf("%s:%d, before  recv\n", __FILE__, __LINE__);
					retlen = recv(sock_fd, ptr, leftlen, 0) ;
//					printf("%s:%d, after recv, and retlen is %d, errno is %d\n", __FILE__, __LINE__, retlen, errno);
					if(retlen < 0)
					{
						if(errno == EAGAIN || errno == EWOULDBLOCK)
						{
							printf("%s:%d : errno is EAGAIN or EWOULDBLOCK \n", __FILE__, __LINE__);
							usleep(10000);
							recv_times++;
							if(recv_times < 6)
							{
								continue;
							}
							else
							{
								break;
							}
							
						}
						else if(errno == EINTR )
						{
							printf("%s:%d : errno is EINTR \n", __FILE__, __LINE__);
							retlen = 0;
						}
						else
						{
							printf("%s:%d, recv data error is %d\n", __FILE__, __LINE__, errno);
							return -1;
						}
					}
					else if(retlen == 0)
					{
						printf("%s:%d, data read ok\n", __FILE__, __LINE__);
						break;
					}
					recvlen += retlen;
					leftlen -= retlen;
					ptr += retlen;
				}
				while(leftlen);
//				printf("%s:%d, reveive data is\n%s\n", __FILE__, __LINE__, recvbuf);
				return recvlen;
   
            }
			else
			{
				ntrytimes++;
				continue;
			}
		}
    }
	
	
}

/**
 * 非阻塞发送
 */
int do_bdasr_send(int sock_fd, char * sendbuf)
{
	int sendlen = strlen(sendbuf) +1;
	int retlen = 0;
	int leftlen = sendlen;
	char *ptr = sendbuf;
	fd_set fds_write;
	struct timeval tval;
	int selret = 0;
	tval.tv_sec = 3;
	tval.tv_usec = 0;
	FD_ZERO(&fds_write);
	FD_SET(sock_fd, &fds_write);
	int ntrytimes = 0;
	while(1 && ntrytimes < MAX_CONNECT_TIMES)
	{
		FD_ZERO(&fds_write);
	    FD_SET(sock_fd, &fds_write);
			
		selret = select(sock_fd + 1, NULL, &fds_write, NULL, &tval);
		if(selret < 0)
		{
			if(errno == EINTR)
			{
				ntrytimes++;
				continue;
			}
			else
			{
				printf("%s:%d, select failed\n", __FILE__, __LINE__);
	            return -1;
			}
		}
		else if(selret == 0)
		{
			printf("%s:%d, select timeout, no descriptors can be read or written\n", __FILE__, __LINE__);
			ntrytimes++;
			continue;
		}
		else
		{
			if(FD_ISSET(sock_fd, &fds_write) )
			{
//				printf("%s:%d, send data\n", __FILE__, __LINE__);
				do
				{
					retlen = send(sock_fd, ptr, sendlen, 0);
					if(retlen < 0)
					{
						if(errno == EAGAIN || errno == EWOULDBLOCK)
						{
							printf("%s:%d : errno is EAGAIN or EWOULDBLOCK \n", __FILE__, __LINE__);
							break;
						}
						else  if(errno == EINTR)
						{
							printf("%s:%d : errno is EINTR \n", __FILE__, __LINE__);
							retlen = 0;
						}
							
						else
						{
							printf("%s:%d : send data error is %d\n", __FILE__, __LINE__, errno);
							return -1;
						}
					}
					leftlen -= retlen;
					sendlen = leftlen;
					ptr += retlen;
//					printf("%s:%d, send data,retlen = %d,leftlen = %d\n", __FILE__, __LINE__,retlen,leftlen);
				}
				while(leftlen > 0);
				if(leftlen <= 0)
					break;
			}
			else
			{
				return -1;
			}
		}
	}
	
	return sendlen;
}
