/***************************************************************************
 * 
 * 从新浪获取天气预报信息
 * 
 **************************************************************************/
#ifndef  __SINA_WEATHER_H_
#define  __SINA_WEATHER_H_
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

//新浪天气预报
#define SINAWEA_COM_BUFSIZE 256
#define SINAWEA_SMALL_BUFSIZE 128
#define SINAWEA_RESP_BUFSIZE 4096
#define SINAWEA_ITEM_BUFSIZE 128
#define SINAWEA_READ_BUFSUZE 1024

/**
*功能:http访问请求构建
*url_data:请求url,例如php.weather.sina.com.cn/xml.php?
*city_data:城市名的GB2312码，例如,深圳%C9%EE%DB%DA
*password_data:DJOYnieT8234jlsK(固定值)
*day_data:请求哪天的天气预报,0(今天),1(明天),2(后天)
*request_content:存储构建好的请求字符串
*
*/
int sina_weather_create_parameters(char *url_data,char *city_data,char *password_data,char *day_data,char *request_content);

/**
*功能:从新浪获取天气预报信息
*url:请求url
*weather:用来存储天气预报内容
*day_data:请求哪天的天气预报,0(今天),1(明天),2(后天)
*
*/
int get_sina_weather(char* url,char *weather,char *show_weather,char *day_data);




























