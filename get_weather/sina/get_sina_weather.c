#include "sina_weather.h"

int bkup_file(char *src_path,char *dest_path)
{
	char city_buf[SINAWEA_SMALL_BUFSIZE];
	memset(city_buf,0,SINAWEA_SMALL_BUFSIZE);
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
		ret = read(fd, city_buf, SINAWEA_SMALL_BUFSIZE-1);
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
				char command[SINAWEA_SMALL_BUFSIZE];
				memset(command,0,SINAWEA_SMALL_BUFSIZE);
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

//获取城市代码
int get_city_gb(char *city_gb)
{
	int is_city_gb_ok = -1;
	int fd = -1;
	int ret = 0;
	char city_buf[SINAWEA_SMALL_BUFSIZE];
	memset(city_buf,0,SINAWEA_SMALL_BUFSIZE);
	
	//检测文件是否存在
	if (access("data/sina_city.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/sina_city.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, city_buf, SINAWEA_SMALL_BUFSIZE-1)) >= 0)
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
	if (access("data/sina_city_bk.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/sina_city_bk.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, city_buf, SINAWEA_SMALL_BUFSIZE-1)) >= 0)
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
	if (access("data/sohu_city_gb.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/sohu_city_gb.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, city_buf, SINAWEA_SMALL_BUFSIZE-1)) >= 0)
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
	if (access("data/sohu_city_gb_bk.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/sohu_city_gb_bk.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, city_buf, SINAWEA_SMALL_BUFSIZE-1)) >= 0)
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
	if(-1 == bkup_file("data/sina_weather0.txt","data/sina_weather0_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/sina_weather1.txt","data/sina_weather1_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/sina_weather2.txt","data/sina_weather2_bk.txt"))
		printf("backup file error.\n");
	
	if(-1 == bkup_file("data/sina_show_weather0.txt","data/sina_show_weather0_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/sina_show_weather1.txt","data/sina_show_weather1_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/sina_show_weather2.txt","data/sina_show_weather2_bk.txt"))
		printf("backup file error.\n");
	
	
	char city_buf[SINAWEA_SMALL_BUFSIZE];
	memset(city_buf,0,SINAWEA_SMALL_BUFSIZE);
	
	//检测文件是否存在,以及获取城市代码
	if(-1 == get_city_gb(city_buf))
	{
		printf("can't get city gb2312 code.\n");
		goto end;
	}
	
	
	
	printf("city name length is %d\n",strlen(city_buf));
	city_buf[strlen(city_buf)] = '\0';
	char sina_request_content[SINAWEA_COM_BUFSIZE];
	memset(sina_request_content,0,SINAWEA_COM_BUFSIZE);
	
	int i = 0;
	for(i=0;i<3;i++)
	{
		char sina_weather_path[100];
		memset(sina_weather_path,0,100);
		char sina_show_weather_path[100];
		memset(sina_show_weather_path,0,100);
		char day[2];
		memset(day,0,2);
		if(i==0)
		{
			day[0] = '0';
			strcpy(sina_weather_path,"data/sina_weather0.txt");
			strcpy(sina_show_weather_path,"data/sina_show_weather0.txt");
		}
		else if(i==1)
		{
			day[0] = '1';
			strcpy(sina_weather_path,"data/sina_weather1.txt");
			strcpy(sina_show_weather_path,"data/sina_show_weather1.txt");
		}	
		else if(i==2)
		{
			day[0] = '2';
			strcpy(sina_weather_path,"data/sina_weather2.txt");
			strcpy(sina_show_weather_path,"data/sina_show_weather2.txt");
		}
			
		if(-1 == sina_weather_create_parameters("php.weather.sina.com.cn/xml.php?",city_buf,"DJOYnieT8234jlsK",day,sina_request_content))
		{
			printf("sina weather create request error.\n");
			goto end;
		}
		printf("sina http : \n%s\n",sina_request_content);
		
		char sina_weather[SINAWEA_COM_BUFSIZE];
		memset(sina_weather,0,SINAWEA_COM_BUFSIZE);
		char sina_show_weather[SINAWEA_COM_BUFSIZE];
		memset(sina_show_weather,0,SINAWEA_COM_BUFSIZE);
		if(-1 == get_sina_weather(sina_request_content,sina_weather,sina_show_weather,day))
		{
			printf("get sina weather error.\n");
			goto end;
		}
		printf("sina weather : \n%s\n",sina_weather);
		printf("sina show weather : \n%s\n",sina_show_weather);
		//把天气写入文件
		if(strlen(sina_weather) <= 0)
		{
			printf("sina weather is NULL.\n");
			strcpy(sina_weather,"nodata");
		}
		int fd = -1;
		int ret = 0;
		//open file
		fd = open(sina_weather_path, O_RDWR|O_CREAT|O_TRUNC);
		if (-1 == fd)
		{
			printf("open weather file error.\n");
			goto end;
		}
		//write file
		ret = write(fd, sina_weather, strlen(sina_weather));
		if(-1 == ret)
		{
			printf("write weather file error.\n");
			goto end;
		}
		close(fd);
		
		
		//需要显示的天气写进文件
		if(strlen(sina_show_weather) <= 0)
		{
			printf("sina show weather is NULL.\n");
			strcpy(sina_show_weather,"nodata");
		}
		
		//open file
		fd = open(sina_show_weather_path, O_RDWR|O_CREAT|O_TRUNC);
		if (-1 == fd)
		{
			printf("open weather file error.\n");
			goto end;
		}
		//write file
		ret = write(fd, sina_show_weather, strlen(sina_show_weather));
		if(-1 == ret)
		{
			printf("write weather file error.\n");
			goto end;
		}
		close(fd);
		
		memset(sina_request_content,0,SINAWEA_COM_BUFSIZE);
		
	}

	end:
	return 0;
}