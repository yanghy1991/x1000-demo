/***************************************************************************
 * 
 * 百度语音识别、语音合成、天气预报相关
 * 
 **************************************************************************/
#ifndef  __BUREAU_WEATHER_H_
#define  __BUREAU_WEATHER_H_
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

#define BURWEA_COM_BUFSIZE 256
#define BURWEA_SMALL_BUFSIZE 128
#define BURWEA_RESP_BUFSIZE 2048

//气象台天气预报
/**
*功能:http访问请求构建www.weather.com.cn/data/sk/101010100.html
*url_data:请求url,例如www.weather.com.cn/data/
*city_code_data:中国气象城市名代码
*request_content:存储构建好的请求字符串
*
*/
int bureau_weater_create_parameters(char *url_data,char *city_code_data,char *request_content);

/**
*功能:从百度获取天气预报信息
*url:请求url
*weather1:用来存储一小时天气预报内容
*
*/
int get_bureau_weather(char* url,char *weather,char *show_weather);



























