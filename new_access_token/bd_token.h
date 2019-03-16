/***************************************************************************
 * 
 * 百度语音识别、语音合成、天气预报相关
 * 
 **************************************************************************/
#ifndef  __BD_TOKEN_H_
#define  __BD_TOKEN_H_
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
//http访问默认端口
#define DEFAULT_PORT 80

//百度get token
#define TOKEN_RESP_BUFSIZE 2048
#define TOKEN_COMMON_BUFSIZE 256
#define TOKEN_TOKEN_BUFSIZE 128

/**
*功能:创建http访问字符串
*参数:
*url_data:绝对url,比如openapi.baidu.com/oauth/2.0/token?
*client_id_data:百度应用账号
*client_secret_data:百度应用安全账号
*request_content:存储http访问字符串
*
*/
int token_create_parameters(char *url_data,char *client_id_data,char *client_secret_data,char *request_content);

/**
*功能:请求http获取access_token
*参数:
*url:http请求字符串
*access_token_data:用来存储access_token
*
*/
int bd_get_token(char* url,char *access_token);


















