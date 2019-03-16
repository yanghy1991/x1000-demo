#ifndef  __SZHY_CONTENT_H_
#define  __SZHY_CONTENT_H_
#endif

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include<dirent.h>

#include <locale.h>
#include <stdbool.h>

#include "cJSON.h"
#include "socket_fun.h"

//如果是正式服务器 注释该行
#define __DEBUG__

#ifdef __DEBUG__
	#define DEBUG(format, ...) printf("[%s : %s : %d] ",__FILE__,__func__,__LINE__); printf(format, ##__VA_ARGS__);
#else
	#define DEBUG(format, ...)
#endif

//正式服务器
#define SERVER_PORT 80
#define SERVER_IP "47.107.25.1"

//测试服务器
//#define SERVER_PORT 8082
//#define SERVER_IP "47.106.157.206"

#define CONTENT_RESP_BUFSIZE 2048
#define HTTP_COM_BUFSIZE 512

/**
*功能:发起http请求，获取响应数据
*参数:
*url:http请求地址
*rspMsg:用来存储rspMsg
*
*/
extern int get_content_server(char* url,char *rsp);