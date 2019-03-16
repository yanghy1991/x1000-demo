/***************************************************************************
 * 
 * 百度语音识别、语音合成、天气预报相关
 * 
 **************************************************************************/
#ifndef  __BD_TTS_H_
#define  __BD_TTS_H_
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

//百度tts
#define TTS_REQ_BUFSIZE 1024
#define TTS_SMALL_BUFSIZE 128
#define TTS_RECV_BUFSIZE 1024

/**
*功能http访问参数构建:
*必填参数:
*tex:合成的文本，使用UTF-8编码，请注意文本长度必须小于1024字节
*lan:语言选择,填写zh
*tok:开放平台获取到的开发者 access_token
*ctp:客户端类型选择，web端填写1
*cuid:用户唯一标识，用来区分用户，填写机器 MAC 地址或 IMEI 码，长度为60以内
*选填参数:
*spd:语速，取值0-9，默认为5中语速
*pit:音调，取值0-9，默认为5中语调
*vol:音量，取值0-9，默认为5中音量
*per:发音人选择, 0为女声，1为男声，3为情感合成-度逍遥，4为情感合成-度丫丫，默认为普通女声
*
*parameters:存储构建好的字符串
*
*/
int tts_create_parameters(char *tex_data,char *lan_data,char *tok_data,char *ctp_data,char *cuid_data,
						char *spd_data,char *pit_data,char *vol_data,char *per_data,char *parameters);
						
/**
*功能:语音合成http请求
*参数:
*mp3_path:返回的音频文件存储路径
*parameters:请求参数字符串
*err_no:存储错误码
*
*/
int tts_http_post(char *mp3_path,char *parameters,int *err_no);
