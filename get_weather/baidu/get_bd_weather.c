#include "bd_weather.h"

int bkup_file(char *src_path,char *dest_path)
{
	char city_buf[BDWEA_SMALL_BUFSIZE];
	memset(city_buf,0,BDWEA_SMALL_BUFSIZE);
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
		fd = open(src_path, O_RDONLY);
		if (-1 == fd)
		{
			printf("open %s error.\n",src_path);
			return -1;
		}
		//read file
		ret = read(fd, city_buf, BDWEA_SMALL_BUFSIZE-1);
		if(ret < 0)
		{
			printf("read city name error.\n");
			return -1;
		}
		else
		{
			if((strlen(city_buf) <= 0) || (strstr(city_buf,"nodata") != NULL))
			{
				printf("%s data error.\n",src_path);
				return -1;
			}
			else
			{
				//printf("city name is %s\n",city_buf);
				char command[BDWEA_SMALL_BUFSIZE];
				memset(command,0,BDWEA_SMALL_BUFSIZE);
				strcat(command,"cp ");
				strcat(command,src_path);
				strcat(command," ");
				strcat(command,dest_path);
				system(command);
			}
		}			
	}
	
	return 0;
}

//获取acess_token
int get_access_token(char *access_token)
{
	int is_token_ok = -1;
	int fd = -1;
	int ret = 0;
	char token_buf[BDWEA_SMALL_BUFSIZE];
	memset(token_buf,0,BDWEA_SMALL_BUFSIZE);
	
	//检测文件是否存在
	if (access("data/bdtoken.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/bdtoken.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, token_buf, BDWEA_SMALL_BUFSIZE-1)) >= 0)
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
			if((ret = read(fd, token_buf, BDWEA_SMALL_BUFSIZE-1)) >= 0)
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

//获取城市代码
int get_city_name(char *city_gb)
{
	int is_city_gb_ok = -1;
	int fd = -1;
	int ret = 0;
	char city_buf[BDWEA_SMALL_BUFSIZE];
	memset(city_buf,0,BDWEA_SMALL_BUFSIZE);
	
	//检测文件是否存在
	if (access("data/sohu_city.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/sohu_city.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, city_buf, BDWEA_SMALL_BUFSIZE-1)) >= 0)
			{
				if(strstr(city_buf,"nodata") == NULL)
				{
					strcpy(city_gb,city_buf);
					is_city_gb_ok = 0;
					close(fd);
					return is_city_gb_ok;
				}
			}
		}
	}
	
	//检测文件是否存在
	if (access("data/sohu_city_bk.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/sohu_city_bk.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, city_buf, BDWEA_SMALL_BUFSIZE-1)) >= 0)
			{
				if(strstr(city_buf,"nodata") == NULL)
				{
					strcpy(city_gb,city_buf);
					is_city_gb_ok = 0;
					close(fd);
					return is_city_gb_ok;
				}
			}
		}
	}
	
	//检测文件是否存在
	if (access("data/ipinfo_city.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/ipinfo_city.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, city_buf, BDWEA_SMALL_BUFSIZE-1)) >= 0)
			{
				if(strstr(city_buf,"nodata") == NULL)
				{
					strcpy(city_gb,city_buf);
					is_city_gb_ok = 0;
					close(fd);
					return is_city_gb_ok;
				}
			}
		}
	}
	
	//检测文件是否存在
	if (access("data/ipinfo_city_bk.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/ipinfo_city_bk.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, city_buf, BDWEA_SMALL_BUFSIZE-1)) >= 0)
			{
				if(strstr(city_buf,"nodata") == NULL)
				{
					strcpy(city_gb,city_buf);
					is_city_gb_ok = 0;
					close(fd);
					return is_city_gb_ok;
				}
			}
		}
	}
	
	return is_city_gb_ok;
}

int main(int argc , char **argv)
{
	//备份文件
	if(-1 == bkup_file("data/bd_weather0.txt","data/bd_weather0_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/bd_weather1.txt","data/bd_weather1_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/bd_weather2.txt","data/bd_weather2_bk.txt"))
		printf("backup file error.\n");
	
	if(-1 == bkup_file("data/bd_show_weather0.txt","data/bd_show_weather0_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/bd_show_weather1.txt","data/bd_show_weather1_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/bd_show_weather2.txt","data/bd_show_weather2_bk.txt"))
		printf("backup file error.\n");
	
	
	
	char city_buf[BDWEA_SMALL_BUFSIZE];
	memset(city_buf,0,BDWEA_SMALL_BUFSIZE);
	char access_token[BDWEA_SMALL_BUFSIZE];
	memset(access_token,0,BDWEA_SMALL_BUFSIZE);
	//检测文件是否存在,以及获取城市名
	if(-1 == get_city_name(city_buf))
	{
		printf("can't get city name.\n");
		goto end;
	}
	
	//检测文件是否存在,以及获取access token
	if(-1 == get_access_token(access_token))
	{
		printf("can't get access token.\n");
		goto end;
	}	
	
	char bd_request_content[BDWEA_COM_BUFSIZE];
	memset(bd_request_content,0,BDWEA_COM_BUFSIZE);
	if(-1 == bd_weater_create_parameters("api.map.baidu.com/telematics/v3/weather?",city_buf,"json","IREczXEtVceyQHETFB5rUPrFS9kIGAVe",bd_request_content))
	{
		printf("sina weather create request error.\n");
		goto end;
	}
	printf("baidu http : \n%s\n",bd_request_content);
	
	int bd_error_data = -1;
	int *bd_error;
	bd_error = &bd_error_data;
	char bd_status[20];
	memset(bd_status,0,20);
	char bd_date[20];
	memset(bd_date,0,20);
	char bd_weather1[BDWEA_SMALL_BUFSIZE];
	memset(bd_weather1,0,BDWEA_SMALL_BUFSIZE);
	char bd_weather2[BDWEA_SMALL_BUFSIZE];
	memset(bd_weather2,0,BDWEA_SMALL_BUFSIZE);
	char bd_weather3[BDWEA_SMALL_BUFSIZE];
	memset(bd_weather3,0,BDWEA_SMALL_BUFSIZE);
	
	char bd_show_weather1[BDWEA_SMALL_BUFSIZE];
	memset(bd_show_weather1,0,BDWEA_SMALL_BUFSIZE);
	char bd_show_weather2[BDWEA_SMALL_BUFSIZE];
	memset(bd_show_weather2,0,BDWEA_SMALL_BUFSIZE);
	char bd_show_weather3[BDWEA_SMALL_BUFSIZE];
	memset(bd_show_weather3,0,BDWEA_SMALL_BUFSIZE);
	
	if(-1 == get_bd_weather(bd_request_content,bd_error,bd_status,bd_date,bd_weather1,bd_show_weather1,bd_weather2,bd_show_weather2,bd_weather3,bd_show_weather3))
	{
		printf("baidu weather create request error.\n");
		goto end;
	}
	printf("baidu bd_weather1 : \n%s\n",bd_weather1);
	printf("baidu bd_weather2 : \n%s\n",bd_weather2);
	printf("baidu bd_weather3 : \n%s\n",bd_weather3);
	printf("baidu bd_show_weather1 : \n%s\n",bd_show_weather1);
	printf("baidu bd_show_weather2 : \n%s\n",bd_show_weather2);
	printf("baidu bd_show_weather3 : \n%s\n",bd_show_weather3);
/*	
	if(strlen(bd_weather1)<=0 || strlen(bd_weather2)<=0 || strlen(bd_weather3)<=0 || strlen(bd_show_weather1)<=0 || strlen(bd_show_weather2)<=0 || strlen(bd_show_weather3)<=0)
	{
		printf("bd_weather1,bd_weather2,bd_weather3 and so on is NULL.\n");
		goto end;
	}
*/	
	int i = 0;
	char file_name[BDWEA_SMALL_BUFSIZE];
	memset(file_name,0,BDWEA_SMALL_BUFSIZE);
	char file_name_show[BDWEA_SMALL_BUFSIZE];
	memset(file_name_show,0,BDWEA_SMALL_BUFSIZE);
	
	char bd_weather[BDWEA_SMALL_BUFSIZE+1];
	memset(bd_weather,0,BDWEA_SMALL_BUFSIZE+1);
	char bd_show_weather[BDWEA_SMALL_BUFSIZE];
	memset(bd_show_weather,0,BDWEA_SMALL_BUFSIZE);
	for(i=0;i<3;i++)
	{
		if(i == 0)
		{
			strcpy(file_name,"data/bd_weather0.txt");
			strcpy(file_name_show,"data/bd_show_weather0.txt");
			strcpy(bd_weather,bd_weather1);
			strcpy(bd_show_weather,bd_show_weather1);
		}
		if(i == 1)
		{
			strcpy(file_name,"data/bd_weather1.txt");
			strcpy(file_name_show,"data/bd_show_weather1.txt");
			strcpy(bd_weather,bd_weather2);
			strcpy(bd_show_weather,bd_show_weather2);
		}
		if(i == 2)
		{
			strcpy(file_name,"data/bd_weather2.txt");
			strcpy(file_name_show,"data/bd_show_weather2.txt");
			strcpy(bd_weather,bd_weather3);
			strcpy(bd_show_weather,bd_show_weather3);
		}
		
		int fd = -1;
		int ret = 0;
		//写语音播报天气预报文本
		
		if(strlen(bd_weather) <= 0)
		{
			printf("sina weather is NULL.\n");
			strcpy(bd_weather,"nodata");
		}
		//open file
		fd = open(file_name, O_RDWR|O_CREAT|O_TRUNC);
		if (-1 == fd)
		{
			printf("open weather file error.\n");
			goto end;
		}
		//write file
		ret = write(fd, bd_weather, strlen(bd_weather));
		if(-1 == ret)
		{
			printf("write weather file error.\n");
			goto end;
		}
		close(fd);
		
		//写屏幕显示天气预报文本
		if(strlen(bd_show_weather) <= 0)
		{
			printf("sina weather is NULL.\n");
			strcpy(bd_show_weather,"nodata");
		}
		//open file
		fd = open(file_name_show, O_RDWR|O_CREAT|O_TRUNC);
		if (-1 == fd)
		{
			printf("open weather file error.\n");
			goto end;
		}
		//write file
		ret = write(fd, bd_show_weather, strlen(bd_show_weather));
		if(-1 == ret)
		{
			printf("write weather file error.\n");
			goto end;
		}
		
		memset(file_name,0,BDWEA_SMALL_BUFSIZE);
		memset(file_name_show,0,BDWEA_SMALL_BUFSIZE);
		memset(bd_weather,0,BDWEA_SMALL_BUFSIZE);
		memset(bd_show_weather,0,BDWEA_SMALL_BUFSIZE);
		
	}
	
	
	end:
	return 0;
}