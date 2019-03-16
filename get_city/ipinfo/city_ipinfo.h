/***************************************************************************
 * 
 * 从ipinfo获取城市名的拼音
 * 
 **************************************************************************/
#ifndef  __IPINFO_H_
#define  __IPINFO_H_
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

//http访问默认端口
#define DEFAULT_PORT 80

//从ipinfo.io/获取当前城市名的拼音
#define IPCITY_RESP_BUFSIZE 1024
#define IPCITY_COM_BUFSIZE 64

/**
*功能:发起http请求，获取响应数据
*参数:
*url:http请求地址
*city:用来存储city
*
*/
int ipinfo_get_city(char* url,char *city);
