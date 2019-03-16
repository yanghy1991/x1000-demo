/***************************************************************************
 * 
 * 从新浪获取当前城市名的unicode码并且转成GB18030码进行存储
 * 
 **************************************************************************/
#ifndef  __CITY_SINA_H_
#define  __CITY_SINA_H_
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
#include<malloc.h>

#include <locale.h>
#include <stdbool.h>

#include "cJSON.h"

//http访问默认端口
#define DEFAULT_PORT 80


//从int.dpool.sina.com.cn/iplookup/iplookup.php?format=json获取当前城市名的unicode码
#define SINACITY_RESP_BUFSIZE 1024
#define SINACITY_COM_BUFSIZE 128
#define SINACITY_SMALL_BUFSIZE 16
#define SINACITY_LINE_BUFSIZE 1024
/**
*功能:发起http请求，获取响应数据
*参数:
*url:http请求地址
*city:用来存储city
*
*/
int sina_get_city(char* url,char *city);
