/***************************************************************************
 * 
 * 百度语音识别、语音合成、天气预报相关
 * 
 **************************************************************************/
#ifndef  __BD_WEATHER_H_
#define  __BD_WEATHER_H_
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

//百度天气预报
#define BDWEA_RESP_BUFSIZE 4096
#define BDWEA_COM_BUFSIZE 256
#define BDWEA_SMALL_BUFSIZE 128
#define BDWEA_READ_BUFSUZE 512

/**
*功能:http访问请求构建
*url_data:请求url,例如api.map.baidu.com/telematics/v3/weather?
*location_data:城市名或拼音
*output_data:返回信息格式设置json或xml
*ak_data:百度应用的ak
*request_content:存储构建好的请求字符串
*
*/
int bd_weater_create_parameters(char *url_data,char *location_data,char *output_data,char *ak_data,char *request_content);

/**
*功能:从百度获取天气预报信息
*url:请求url
*error:用来存储error错误码
*status:用来存储请求天气的状态标志
*date:用来存储天气预报返回日期
*weather1:用来存储今天天气预报内容
*weather2:用来存储明天天气预报内容
*
*/
int get_bd_weather(char* url,int *error,char *status,char *date,char *weather1,char *show_weather1,char *weather2,char *show_weather2,char *weather3,char *show_weather3);
