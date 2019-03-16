/***************************************************************************
 * 
 * 百度语音识别、语音合成、天气预报相关
 * 
 **************************************************************************/
#ifndef  __BD_ASR_H_
#define  __BD_ASR_H_
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

#include <iconv.h>   


#include <locale.h>
#include <stdbool.h>

#include "cJSON.h"
#include "base64.h" 

//http访问默认端口
#define DEFAULT_PORT 80

//百度asr
#define ASR_RESP_BUFSIZE 1024
#define ASR_REQUEST_BUFSIZE 1024*500
#define ASR_SMALL_BUFSIZE 128

/**
*功能:语音识别http请求
*参数:
*host_data:地址字符串，不带http:// ,例如vop.baidu.com
*wav_path:wav文件路径(需要进行语音识别的文件)
*format_data:语音压缩的格式,不区分大小写,pcm（不压缩）、wav、opus、speex、amr
*token_data:开放平台获取到的开发者 access_token
*cuid:用户唯一标识，用来区分用户，填写机器 MAC 地址或 IMEI 码，长度为60以内
*result:用来存储result的值
*error_msg:用来存储err_msg的内容
*error_no:用来存储err_no的内容
*
*/
int analysis_http_post(char *host_data);

