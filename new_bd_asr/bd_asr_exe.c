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

//语音播放今天天气
int play_today_weather()
{
	//播放新浪天气
	if (access("data/sina_weather0.mp3",F_OK) == 0)
	{
		system("mpg123 data/sina_weather0.mp3");
		return 0;
	}
	//播放新浪天气
	if (access("data/sina_weather0_bk.mp3",F_OK) == 0)
	{
		system("mpg123 data/sina_weather0_bk.mp3");
		return 0;
	}
	
	//播放百度天气
	if (access("data/bd_weather0.mp3",F_OK) == 0)
	{
		system("mpg123 data/bd_weather0.mp3");
		return 0;
	}
	//播放百度天气
	if (access("data/bd_weather0_bk.mp3",F_OK) == 0)
	{
		system("mpg123 data/bd_weather0_bk.mp3");
		return 0;
	}
	
	//播放气象台天气
	if (access("data/bureau_weather0.mp3",F_OK) == 0)
	{
		system("mpg123 data/bureau_weather0.mp3");
		return 0;
	}
	//播放气象台天气
	if (access("data/bureau_weather0_bk.mp3",F_OK) == 0)
	{
		system("mpg123 data/bureau_weather0_bk.mp3");
		return 0;
	}
	
	return -1;
}

//语音播放明天天气
int play_tomorroe_weather()
{
	//播放新浪天气
	if (access("data/sina_weather1.mp3",F_OK) == 0)
	{
		system("mpg123 data/sina_weather1.mp3");
		return 0;
	}
	//播放新浪天气
	if (access("data/sina_weather1_bk.mp3",F_OK) == 0)
	{
		system("mpg123 data/sina_weather1_bk.mp3");
		return 0;
	}
	
	//播放百度天气
	if (access("data/bd_weather1.mp3",F_OK) == 0)
	{
		system("mpg123 data/bd_weather1.mp3");
		return 0;
	}
	//播放百度天气
	if (access("data/bd_weather1_bk.mp3",F_OK) == 0)
	{
		system("mpg123 data/bd_weather1_bk.mp3");
		return 0;
	}
	
	return -1;
}

//语音播放后天天气
int play_afterday_weather()
{
	//播放新浪天气
	if (access("data/sina_weather2.mp3",F_OK) == 0)
	{
		system("mpg123 data/sina_weather2.mp3");
		return 0;
	}
	//播放新浪天气
	if (access("data/sina_weather2_bk.mp3",F_OK) == 0)
	{
		system("mpg123 data/sina_weather2_bk.mp3");
		return 0;
	}
	
	//播放百度天气
	if (access("data/bd_weather2.mp3",F_OK) == 0)
	{
		system("mpg123 data/bd_weather2.mp3");
		return 0;
	}
	//播放百度天气
	if (access("data/bd_weather2_bk.mp3",F_OK) == 0)
	{
		system("mpg123 data/bd_weather2_bk.mp3");
		return 0;
	}
	
	return -1;
}

int main(int argc , char **argv)
{
	//获取access_token
	char access_token[ASR_SMALL_BUFSIZE];
	memset(access_token,0,ASR_SMALL_BUFSIZE);
	if(-1 == get_access_token(access_token))
	{
		printf("get access token error.\n");
		goto end;
	}
	
	//录音
	//system("arecord -c 1 -r 8000 -f S16_LE -d 4 data/tq.wav");
	//system("arecord -c 1 -r 8000 -f S16_LE -D plughw:1,0 -d 4 data/tq.wav");
	
	//asr
	char result[ASR_RESP_BUFSIZE];
	memset(result,0,ASR_RESP_BUFSIZE);
	char err_msg[ASR_SMALL_BUFSIZE];
	memset(err_msg,0,ASR_SMALL_BUFSIZE);
	int asr_err_no = 1;
	int *error_no = &asr_err_no;
	if(-1 == asr_http_post("vop.baidu.com","data/tq.wav","wav",access_token,"00:0c:29:fb:d7:52",result,err_msg,error_no))
	{
		printf("asr from baidu error.\n");
		//goto end;
	}
	printf("result : \n%s\n",result);
	printf("err_msg : \n%s\n",err_msg);
	printf("asr_err_no : %d\n",asr_err_no);
	
	
	
/*	
	char *tmp_weather = strstr(result,"天气");
	char *tmp_clock = strstr(result,"闹钟");

	if(tmp_weather != NULL)
	{
		char *tmp2 = strstr(result,"今天");
		char *tmp3 = strstr(result,"明天");
		char *tmp4 = strstr(result,"后天");
		if(tmp2 != NULL)
		{
			if(-1 == play_today_weather())
			{
				printf("play today weather error.\n");
				goto end;
			}	
		}
		else if(tmp3 != NULL)
		{
			if(-1 == play_tomorroe_weather())
			{
				printf("play tomorrow weather error.\n");
				goto end;
			}
		}
		else if(tmp4 != NULL)
		{
			if(-1 == play_afterday_weather())
			{
				printf("play afterday weather error.\n");
				goto end;
			}
		}
	}
	else if(tmp_clock != NULL)
	{
		char *tmp1 = strstr(result,"设置");
		char *tmp2 = strstr(result,"删除");
		
		if(tmp1 != NULL)
		{
			char set_clock_str[ASR_SMALL_BUFSIZE];
			memset(set_clock_str,0,ASR_SMALL_BUFSIZE); 
			strcat(set_clock_str,"/tmp/my/set_clock ");
			strcat(set_clock_str,result);
			system(set_clock_str);
		}
		if(tmp2 != NULL)
		{
				
		}
	}
	else
		printf("no use information.\n");
*/	
	end:
	return 0;
}