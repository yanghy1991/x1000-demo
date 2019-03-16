#include "bd_asr.h"

//获取acess_token
int get_access_token(char *access_token)
{
	int is_token_ok = -1;
	int fd = -1;
	int ret = 0;
	char token_buf[ASR_SMALL_BUFSIZE];
	memset(token_buf,0,ASR_SMALL_BUFSIZE);
	
	//检测文件是否存在
	if (access("data/bdtoken.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/bdtoken.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, token_buf, ASR_SMALL_BUFSIZE-1)) >= 0)
			{
				if(strstr(token_buf,"nodata") == NULL)
				{
					strcpy(access_token,token_buf);
					is_token_ok = 0;
					close(fd);
					return is_token_ok;
				}
			}
		}
	}
	
	//检测文件是否存在
	if (access("data/bdtoken_bk.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/bdtoken_bk.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, token_buf, ASR_SMALL_BUFSIZE-1)) >= 0)
			{
				if(strstr(token_buf,"nodata") == NULL)
				{
					strcpy(access_token,token_buf);
					is_token_ok = 0;
					close(fd);
					return is_token_ok;
				}
			}
		}
	}
	
	return is_token_ok;
}

int write_file(char *rspMsg)
{
	int fd = -1;
	int ret = 0;
	//open file
	fd = open("data/rspMsg.txt", O_RDWR|O_CREAT|O_TRUNC);
	if (-1 == fd)
	{
		printf("open rspMsg.txt error.\n");
		return -1;
	}
	//write file
	ret = write(fd, rspMsg, strlen(rspMsg));
	if(-1 == ret)
	{
		printf("write rspMsg.txt error.\n");
		return -1;
	}
	close(fd);
	usleep(1000);
	
	return 0;
}


//获取当前时间
int getNowTime(char *nowTime)
{
    time_t now;//实例化time_t结构
	struct tm *timenow;//实例化tm结构指针
	time(&now);
	//time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
	timenow   =   localtime(&now);
	//localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)
	//printf("Local time is:%s\n",asctime(timenow));
	//上句中asctime函数把时间转换成字符，通过printf()函数输出
	
	char current[128];
	memset(current,0,128);
    sprintf(current, "%04d%02d%02d%02d%02d%02d", timenow->tm_year + 1900, timenow->tm_mon+1, timenow->tm_mday, 
												   timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
	printf("current = %s\n",current);
	strcpy(nowTime,current);
}

int weather_info(char *rspMsg)
{
	//把返回的信息写入文件
	if(-1 == write_file(rspMsg))
	{
		printf("把返回的信息写入文件错误.");
		return -1;
	}
	//tts
	system("./bd_tts_exe data/rspMsg.txt data/rspMsg.mp3");
	system("mpg123 data/rspMsg.mp3");
	
	return 0;	
}

int type_all_func(char *rspMsg)
{
	//把返回的信息写入文件
	if(-1 == write_file(rspMsg))
	{
		printf("把返回的信息写入文件错误.");
		return -1;
	}
	//tts
	system("./bd_tts_exe data/rspMsg.txt data/rspMsg.mp3");
	system("mpg123 data/rspMsg.mp3");
	return 0;	
}

int alarm_func(char *alarm, char *topic)
{
	char minute[16];
	memset(minute,0,16);
	char hour[16];
	memset(hour,0,16);
	char day[16];
	memset(day,0,16);
	char month[16];
	memset(month,0,16);		
	int count = 0;		
	char buf[256];
	memset(buf,0,256);
	char *token;  
	strcpy(buf,alarm);  
	char *buff = alarm;
	
	while((token = strsep(&buff, " ")) != NULL){  
		count++; 
		if(count == 1)
			strcpy(minute,token);
		else if(count == 2)
			strcpy(hour,token);
		else if(count == 3)
			strcpy(day,token);
		else if(count == 4)
			strcpy(month,token);
	}
	char alarmStr[512];
	memset(alarmStr,0,512);
	if(strcmp(topic,"闹钟") == 0)
	{
		strcat(alarmStr,month);
		strcat(alarmStr,"月");
		strcat(alarmStr,day);
		strcat(alarmStr,"日");
		strcat(alarmStr,hour);
		strcat(alarmStr,"点");
		strcat(alarmStr,minute);
		strcat(alarmStr,"分的闹钟设置完毕");
	}
	else
	{
		strcat(alarmStr,"我会在");
		strcat(alarmStr,month);
		strcat(alarmStr,"月");
		strcat(alarmStr,day);
		strcat(alarmStr,"日");
		strcat(alarmStr,hour);
		strcat(alarmStr,"点");
		strcat(alarmStr,minute);
		strcat(alarmStr,"分");
		strcat(alarmStr,",按时提醒你");
		strcat(alarmStr,topic);
	}
	
	printf("alarmStr = %s \n",alarmStr);
	
	//把返回的信息写入文件
	if(-1 == write_file(alarmStr))
	{
		printf("把返回的信息写入文件错误.");
		return -1;
	}
	//tts
	system("./bd_tts_exe data/rspMsg.txt data/rspMsg.mp3");
	
	
	
	///设置闹钟(crontab)
	
	//获取当前时间
	char currentTime[256];
	memset(currentTime,0,256);
	getNowTime(currentTime);
	
	printf("currentTime = %s\n",currentTime);
	
	//把返回的信息写入文件
	char tmp[256];
	memset(tmp,0,256);
	strcat(tmp,topic);
	strcat(tmp,"时间到了");
	printf("tmp = %s\n",tmp);
	if(-1 == write_file(tmp))
	{
		printf("把返回的信息写入文件错误.");
		return -1;
	}
	//生成闹钟到提示音
	char cmdStr[1024];
	memset(cmdStr,0,1024);
	strcat(cmdStr,"./bd_tts_exe data/rspMsg.txt data/");
	strcat(cmdStr,currentTime);
	strcat(cmdStr,".mp3");
	system(cmdStr);
	
	printf("buf = %s\n",buf);
	
	//写入crontab
	char cronStr[1024];
	memset(cronStr,0,1024);
	strcat(cronStr,"echo \"");
	strcat(cronStr,buf);
	strcat(cronStr," cd /home/others/x1000_my_demo/x1000-http_server_update/data && nohup /usr/bin/mpg123 /home/others/x1000_my_demo/x1000-http_server_update/data/");
	strcat(cronStr,currentTime);
	strcat(cronStr,".mp3 && nohup /usr/bin/mpg123 /home/others/x1000_my_demo/x1000-http_server_update/data/alarm.mp3");	
	strcat(cronStr,"\" >> /var/spool/cron/crontabs/root");
	printf("cronStr = %s\n",cronStr);
	system(cronStr);
	
	//播放
	system("mpg123 data/rspMsg.mp3");
	
	return 0;
}

int delete_alarm_func(char *rspMsg)
{
	//把返回的信息写入文件
	if(-1 == write_file(rspMsg))
	{
		printf("把返回的信息写入文件错误.");
		return -1;
	}
	//tts
	system("./bd_tts_exe data/rspMsg.txt data/rspMsg.mp3");
	system("echo "" > /var/spool/cron/crontabs/root");	
	system("mpg123 data/rspMsg.mp3");
	return 0;
}

int main(int argc , char **argv)
{
#if 0	
	//获取access_token
	char access_token[ASR_SMALL_BUFSIZE];
	memset(access_token,0,ASR_SMALL_BUFSIZE);
	if(-1 == get_access_token(access_token))
	{
		printf("get access token error.\n");
		goto end;
	}
	/*
	printf("arecord starting ......\n");
	//录音
	system("arecord -c 1 -r 8000 -f S16_LE -d 5 data/tq.wav");
	//system("arecord -c 1 -r 8000 -f S16_LE -D plughw:1,0 -d 4 data/tq.wav");
	printf("are end ......\n");
	*/
	//asr
	char result[ASR_RESP_BUFSIZE];
	memset(result,0,ASR_RESP_BUFSIZE);
	char err_msg[ASR_SMALL_BUFSIZE];
	memset(err_msg,0,ASR_SMALL_BUFSIZE);
	int asr_err_no = 1;
	int *error_no = &asr_err_no;
	if(-1 == asr_http_post("vop.baidu.com","out.pcm","pcm",access_token,"00:0c:29:fb:d7:52",result,err_msg,error_no))
	{
		printf("asr from baidu error.\n");
		goto end;
	}
	printf("result : \n%s\n",result);
	printf("err_msg : \n%s\n",err_msg);
	printf("asr_err_no : %d\n",asr_err_no);
#endif

	
	//char *url = "localhost:8080/bingo/cmd?deviceId=szhy001000001&cmd=\"今天天气怎么样\"";
	
	char url[ASR_RESP_BUFSIZE];
	memset(url,0,ASR_RESP_BUFSIZE);	
	char rspMsg[ASR_RESP_BUFSIZE];
	memset(rspMsg,0,ASR_RESP_BUFSIZE);
	char rspType[ASR_RESP_BUFSIZE];
	memset(rspType,0,ASR_RESP_BUFSIZE);


	char strTmp[ASR_RESP_BUFSIZE];
	memset(strTmp,0,ASR_RESP_BUFSIZE);
	
//	printf("请输入搜索语句:");
//	scanf("%s",strTmp);	
	
	strcpy(strTmp,"我要听白雪公主的故事");
	
	strcat(url,"39.108.71.229:8080/bingo/cmd?");
	strcat(url,"deviceId=szhy001000001");
	strcat(url,"&cmd=");
	
	
	strcat(url,strTmp);
	
//	strcat(url,result);
	printf("url = %s\n",url);
	
	//把结果存到文件
	if(-1 == get_msg_from_server(url,rspMsg,rspType))
	{
		printf("rspMsg http request error.\n");
		goto end;
	}
	printf("rspMsg = %s\n",rspMsg);
	printf("rspType = %s\n",rspType);
	
/*	
	if(strcmp(rspType,"weather") == 0)
	{
		if(-1 == weather_info(rspMsg))
		{
			printf("获取天气预报信息失败.");
			goto end;
		}
	}
	else if(strcmp(rspType,"all") == 0)
	{
		if(-1 == type_all_func(rspMsg))
		{
			printf("没有获取到需要的信息.");
			goto end;
		}
	}
	else if(strcmp(rspType,"oncealarm") == 0)
	{
		char alarmInfo[16];
		memset(alarmInfo,0,16);	
		char topic[HTTP_SMALL_BUFSIZE];
		memset(topic,0,HTTP_SMALL_BUFSIZE);
		int count = 0;		
		char *buf;  
		char *token;  
		buf = rspMsg;  
		while((token = strsep(&buf, "-")) != NULL){  
			count++; 
			if(count == 1)
				strcpy(alarmInfo,token);
			else if(count == 2)
				strcpy(topic,token);
		}
		printf("alarmInfo = %s\n", alarmInfo); 
		printf("topic = %s\n", topic); 
		
		if(-1 == alarm_func(alarmInfo,topic))
		{
			printf("闹钟设置操作错误.");
			goto end;
		}
	}
	else if(strcmp(rspType,"alarm") == 0)
	{
		char alarmInfo[16];
		memset(alarmInfo,0,16);	
		char topic[HTTP_SMALL_BUFSIZE];
		memset(topic,0,HTTP_SMALL_BUFSIZE);
		int count = 0;		
		char *buf;  
		char *token;  
		buf = rspMsg;  
		while((token = strsep(&buf, "-")) != NULL){  
			count++; 
			if(count == 1)
				strcpy(alarmInfo,token);
			else if(count == 2)
				strcpy(topic,token);
		}
		printf("alarmInfo = %s\n", alarmInfo); 
		printf("topic = %s\n", topic); 
		
		if( -1 == alarm_func(alarmInfo,topic))
		{
			printf("闹钟设置操作错误.");
			goto end;
		}
	}
	else if(strcmp(rspType,"deletealarm") == 0)
	{
		if(-1 == delete_alarm_func(rspMsg))
		{
			printf("闹钟删除操作错误.");
			goto end;
		}
	}
	else
	{
		printf("rspMsg ... = %s\n",rspMsg);
	}
*/	
	
	
end:
	return 0;
}