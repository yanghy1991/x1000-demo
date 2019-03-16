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

#include "cJSON.h"

#define TIME_JSON_BUFSIZE 2048
#define ORGIN_LEN 128
#define TIME_LEN 32

//解析josn字符串
int time_parse_json(char *timeStr,int *timeInt)
{
	printf("timeStr = %s\n",timeStr);
	char *buf = "{\"零\":0,\"一\":1,\"二\":2,\"三\":3,\"四\":4,\"五\":5,\"六\":6,\"七\":7,\"八\":8,\"九\":9,\"十\":10,\"十一\":11,\"十二\":12,\"十三\":13,\"十四\":14,\"十五\":15,\"十六\":16,\"十七\":17,\"十八\":18,\"十九\":19,\"二十\":20,\"二十一\":21,\"二十二\":22,\"二十三\":23,\"二十四\":24,\"二十五\":25,\"二十六\":26,\"二十七\":27,\"二十八\":28,\"二十九\":29,\"三十\":30,\"三十一\":31,\"三十二\":32,\"三十三\":33,\"三十四\":34,\"三十五\":35,\"三十六\":36,\"三十七\":37,\"三十八\":38,\"三十九\":39,\"四十\":40,\"四十一\":41,\"四十二\":42,\"四十三\":43,\"四十四\":\"44\",\"四十五\":45,\"四十六\":46,\"四十七\":47,\"四十八\":48,\"四十九\":49,\"五十\":50,\"五十一\":51,\"五十二\":52,\"五十三\":53,\"五十四\":54,\"五十五\":55,\"五十六\":56,\"五十七\":57,\"五十八\":58,\"五十九\":59,\"两\":2,\"半\":30}";
	cJSON * root = NULL;
    cJSON * item = NULL;
	root = cJSON_Parse(buf);
	if (!root) 
    {
        printf("%s : create json object return.\n",__func__);
		return -1;
    }
	
	item = cJSON_GetObjectItem(root, timeStr);
	if(NULL == item)
	{
		printf("%s : get time failed!\n",__func__);
		return -1;
	}
	
	*timeInt = item->valueint;
	
	return 0;	
} 

//去掉字符串中的逗号
int change_str(char *orginStr,char *newStr)
{
	char nextStr[ORGIN_LEN];
	memset(nextStr,0,ORGIN_LEN);
	
	strcpy(nextStr,orginStr);
	printf("nextStr = %s\n",nextStr);
	
	char *tmp = NULL;
	char tmpArray[ORGIN_LEN];
	memset(tmpArray,0,ORGIN_LEN);
	
	while((tmp = strstr(nextStr,"，")) != NULL)
	{
		printf("tmp = %s\n",tmp);
		strcpy(tmpArray,tmp+3);

		*tmp = '\0'; 
		strcat(newStr,nextStr);
		
		memset(nextStr,0,ORGIN_LEN);
		strcpy(nextStr,tmpArray);
		memset(tmpArray,0,ORGIN_LEN);
	}
	
	printf("newStr = %s\n",newStr);

	return 0;
}

//从字符串中获取小时数和分钟数
int get_h_m(char *clockStr,int *hour,int *minute)
{
	char hourStr[TIME_LEN];
	memset(hourStr,0,TIME_LEN);
	char minuteStr[TIME_LEN];
	memset(minuteStr,0,TIME_LEN);
	
	if(clockStr == NULL)
	{
		printf("%s : clock string is NULL.\n",__func__);
		return -1;
	}

	char clockStrTmp[ORGIN_LEN];
	memset(clockStrTmp,0,ORGIN_LEN);
	strcpy(clockStrTmp,clockStr);
	//把闹钟设置为早上七点三十分
	if(strstr(clockStrTmp,"闹钟") != NULL)
	{
		if(strstr(clockStrTmp,"设置") != NULL)
		{
			char *tmp1 = NULL;
			char *tmp2 = NULL;
			char *tmp3 = NULL;
			char *tmp4 = NULL;
			if(((tmp1 = strstr(clockStrTmp,"早上")) != NULL) || ((tmp1 = strstr(clockStrTmp,"上午")) != NULL))
			{
				printf("tmp1 = %s\n",tmp1);
				if((tmp2 = strstr(clockStrTmp,"点")) != NULL)
				{
					printf("tmp2 = %s\n",tmp2);
					tmp3 = tmp2+3;
					printf("tmp3 = %s\n",tmp3);
					
					//获取小时数
					*tmp2 = '\0';
					strcpy(hourStr,tmp1+6);
					printf("hourStr = %s\n",hourStr);
					//小时数转换成int型
					if(-1 == time_parse_json(hourStr,hour))
					{
						printf("hour String to Int error.\n");
						return -1;
					}
						
					//获取分钟数
					if(strlen(tmp3) == 0)
					{
						memset(minuteStr,0,strlen(minuteStr));
					}
					else
					{
						if((tmp4 = strstr(tmp3,"分")) != NULL)
						{
							*tmp4 = '\0';
							strcpy(minuteStr,tmp3);
							printf("minuteStr = %s\n",minuteStr);
						}
						else if((tmp4 = strstr(tmp3,"半")) != NULL)
						{
							strcpy(minuteStr,"三十");
						}
						else
						{
							strcpy(minuteStr,tmp3);
						}
						
						//分钟数转换成int型
						if(-1 == time_parse_json(minuteStr,minute))
						{
							printf("minute String to Int error.\n");
							return -1;
						}						
					}
				}
				else
				{
					printf("no dian . \n");
					return -1;
				}
			}
			else if(((tmp1 = strstr(clockStrTmp,"晚上")) != NULL) || ((tmp1 = strstr(clockStrTmp,"下午")) != NULL))
			{
				printf("tmp1 = %s\n",tmp1);
				if((tmp2 = strstr(clockStrTmp,"点")) != NULL)
				{
					printf("tmp2 = %s\n",tmp2);
					tmp3 = tmp2+3;
					printf("tmp3 = %s\n",tmp3);
					
					//获取小时数
					*tmp2 = '\0';
					strcpy(hourStr,tmp1+6);
					printf("hourStr = %s\n",hourStr);
					//小时数转换成int型
					if(-1 == time_parse_json(hourStr,hour))
					{
						printf("hour String to Int error.\n");
						return -1;
					}
					if(*hour == 12 || *hour == 0)
						*hour = 0;	
					else
						*hour = (*hour+12);
					//获取分钟数
					if(strlen(tmp3) == 0)
					{
						memset(minuteStr,0,strlen(minuteStr));
					}
					else
					{
						if((tmp4 = strstr(tmp3,"分")) != NULL)
						{
							*tmp4 = '\0';
							strcpy(minuteStr,tmp3);
							printf("minuteStr = %s\n",minuteStr);
						}
						else if((tmp4 = strstr(tmp3,"半")) != NULL)
						{
							strcpy(minuteStr,"三十");
						}
						else
						{
							strcpy(minuteStr,tmp3);
						}
						
						//分钟数转换成int型
						if(-1 == time_parse_json(minuteStr,minute))
						{
							printf("minute String to Int error.\n");
							return -1;
						}						
					}
				}
				else
				{
					printf("no dian . \n");
					return -1;
				}
			}
			else if((tmp1 = strstr(clockStrTmp,"中午")) != NULL)
			{
				printf("tmp1 = %s\n",tmp1);
				if((tmp2 = strstr(clockStrTmp,"点")) != NULL)
				{
					printf("tmp2 = %s\n",tmp2);
					tmp3 = tmp2+3;
					printf("tmp3 = %s\n",tmp3);
					
					//获取小时数
					*tmp2 = '\0';
					strcpy(hourStr,tmp1+6);
					printf("hourStr = %s\n",hourStr);
					//小时数转换成int型
					if(-1 == time_parse_json(hourStr,hour))
					{
						printf("hour String to Int error.\n");
						return -1;
					}
					if(*hour == 1 || *hour == 2)
						*hour = (*hour+12);
						
					//获取分钟数
					if(strlen(tmp3) == 0)
					{
						memset(minuteStr,0,strlen(minuteStr));
					}
					else
					{
						if((tmp4 = strstr(tmp3,"分")) != NULL)
						{
							*tmp4 = '\0';
							strcpy(minuteStr,tmp3);
							printf("minuteStr = %s\n",minuteStr);
						}
						else if((tmp4 = strstr(tmp3,"半")) != NULL)
						{
							strcpy(minuteStr,"三十");
						}
						else
						{
							strcpy(minuteStr,tmp3);
						}
						
						//分钟数转换成int型
						if(-1 == time_parse_json(minuteStr,minute))
						{
							printf("minute String to Int error.\n");
							return -1;
						}						
					}
				}
				else
				{
					printf("no dian . \n");
					return -1;
				}
				
				
			}
			else
			{
				if(((tmp1 = strstr(clockStrTmp,"成")) != NULL) || ((tmp1 = strstr(clockStrTmp,"为")) != NULL))
				{
					printf("tmp1 = %s\n",tmp1);
					if((tmp2 = strstr(clockStrTmp,"点")) != NULL)
					{
						printf("tmp2 = %s\n",tmp2);
						tmp3 = tmp2+3;
						printf("tmp3 = %s\n",tmp3);
						
						//获取小时数
						*tmp2 = '\0';
						strcpy(hourStr,tmp1+3);
						printf("hourStr = %s\n",hourStr);
						//小时数转换成int型
						if(-1 == time_parse_json(hourStr,hour))
						{
							printf("hour String to Int error.\n");
							return -1;
						}
								
						//获取分钟数
						if(strlen(tmp3) == 0)
						{
							memset(minuteStr,0,strlen(minuteStr));
						}
						else
						{
							if((tmp4 = strstr(tmp3,"分")) != NULL)
							{
								*tmp4 = '\0';
								strcpy(minuteStr,tmp3);
								printf("minuteStr = %s\n",minuteStr);
							}
							else if((tmp4 = strstr(tmp3,"半")) != NULL)
							{
								strcpy(minuteStr,"三十");
							}
							else
							{
								strcpy(minuteStr,tmp3);
							}
							
							//分钟数转换成int型
							if(-1 == time_parse_json(minuteStr,minute))
							{
								printf("minute String to Int error.\n");
								return -1;
							}						
						}
					}
					else
					{
						printf("no dian . \n");
						return -1;
					}
				}
				else 
				{
					printf("can not get useful information.\n");
					return -1;
				}	
			}
		}
	}
	
	return 0;
}
//[把闹钟，设置为下午，两点半，]
//主函数
int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("useage: command clockstring.\n");
		goto end;
	}
	
	//char *orginStr = "把闹钟设置为,八点,五十";
	char newstr[ORGIN_LEN];
	memset(newstr,0,ORGIN_LEN);

	//去除数组中的逗号
	if(-1 == change_str(argv[1],newstr))
	{
		printf("change string error.\n");
		goto end;
	}

	printf("newstr = %s\n",newstr);

	int hour = -1;
	int minute = -1;
	int *hourPtr = &hour;
	int *minutePtr = &minute;
	
	if(-1 == get_h_m(newstr,hourPtr,minutePtr))
	{
		printf("get hour and minute error.\n");
		goto end;
	}

	printf("hour = %d,minute = %d\n",hour,minute);
	//  * * * * * root /home/others/alarmclock/alock_test.sh
	
	if(hour == -1)
		goto end;
	if(minute == -1)
		minute = 0;
	
	char out_buf[ORGIN_LEN];
	memset(out_buf,0,ORGIN_LEN);
	//sprintf(out_buf,"%d %d * * * root /home/others/alarmclock/alock_test.sh \n",minute,hour);
	sprintf(out_buf,"%d %d * * * root /tmp/my/alock_test.sh \n",minute,hour);
	printf("out_buf = %s \n",out_buf);	
	
	char *src_path = "/etc/crontabs/root";
	//检测文件是否存在
	if (access(src_path,F_OK) == -1)
	{
		printf("%s not exist.\n",src_path);
		return -1;
	}
	else
	{
		//从文件中读取内容
		int fd = -1;
		int ret = 0;
		//open file
		fd = open(src_path, O_RDWR|O_APPEND);
		if (-1 == fd)
		{
			printf("open %s error.\n",src_path);
			return -1;
		}
		
			
		ret = write(fd, out_buf, strlen(out_buf));
		if(-1 == ret)
		{
			printf("write %s error.\n",src_path);
			return -1;
		}
		close(fd);					
	}

end:
	return 0;
}

