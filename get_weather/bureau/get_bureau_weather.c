#include "bureau_weather.h"

int bkup_file(char *src_path,char *dest_path)
{
	char city_buf[BURWEA_SMALL_BUFSIZE];
	memset(city_buf,0,BURWEA_SMALL_BUFSIZE);
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
		ret = read(fd, city_buf, BURWEA_SMALL_BUFSIZE-1);
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
				char command[BURWEA_SMALL_BUFSIZE];
				memset(command,0,BURWEA_SMALL_BUFSIZE);
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
int get_city_code(char *city_gb)
{
	int is_city_gb_ok = -1;
	int fd = -1;
	int ret = 0;
	char city_buf[BURWEA_SMALL_BUFSIZE];
	memset(city_buf,0,BURWEA_SMALL_BUFSIZE);
	
	//检测文件是否存在
	if (access("data/sohu_city_code.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/sohu_city_code.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, city_buf, BURWEA_SMALL_BUFSIZE-1)) >= 0)
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
	if (access("data/sohu_city_code_city_bk.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/sohu_city_code_city_bk.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, city_buf, BURWEA_SMALL_BUFSIZE-1)) >= 0)
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
	if(-1 == bkup_file("data/bureau_weather.txt","data/bureau_weather_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/bureau_show_weather.txt","data/bureau_show_weather_bk.txt"))
		printf("backup file error.\n");
	
	char city_code_buf[BURWEA_SMALL_BUFSIZE];
	memset(city_code_buf,0,BURWEA_SMALL_BUFSIZE);
	//检测文件是否存在,以及获取城市的天气预报代码
	if(-1 == get_city_code(city_code_buf))
	{
		printf("can't get city name.\n");
		goto end;
	}
		
	//构建http请求字符串
	char bureau_request_content[BURWEA_COM_BUFSIZE];
	memset(bureau_request_content,0,BURWEA_COM_BUFSIZE);
	if(-1 == bureau_weater_create_parameters("www.weather.com.cn/data/sk/",city_code_buf,bureau_request_content))
	{
		printf("bureau weather create request error.\n");
		goto end;
	}
	printf("bureau_request_content = %s\n",bureau_request_content);

	if(strlen(bureau_request_content) <= 0 )
	{
		printf("bureau_request_content is NULL.\n");
		goto end;
	}
	char bureau_weather[BURWEA_SMALL_BUFSIZE];
	memset(bureau_weather,0,BURWEA_SMALL_BUFSIZE);
	char bureau_show_weather[BURWEA_SMALL_BUFSIZE];
	memset(bureau_show_weather,0,BURWEA_SMALL_BUFSIZE);
	
	//请求服务器
	if(-1 == get_bureau_weather(bureau_request_content,bureau_weather,bureau_show_weather))
	{
		printf("bureau weather create request error.\n");
		goto end;
	}
	printf("bureau weather : \n%s\n",bureau_weather);
	printf("bureau bureau_show_weather : \n%s\n",bureau_show_weather);
	
	//把天气写入文件
	if(strlen(bureau_weather) <= 0)
	{
		printf("bureau weather is NULL.\n");
		strcpy(bureau_weather,"nodata");
	}
	int fd = -1;
	int ret = 0;
	//open file
	fd = open("data/bureau_weather.txt", O_RDWR|O_CREAT|O_TRUNC);
	if (-1 == fd)
	{
		printf("open weather file error.\n");
		goto end;
	}
	//write file
	ret = write(fd, bureau_weather, strlen(bureau_weather));
	if(-1 == ret)
	{
		printf("write weather file error.\n");
		goto end;
	}
	close(fd);
	
	//需要显示的天气写进文件
	if(strlen(bureau_show_weather) <= 0)
	{
		printf("sina show weather is NULL.\n");
		strcpy(bureau_show_weather,"nodata");
	}
		
	//open file
	fd = open("data/bureau_show_weather.txt", O_RDWR|O_CREAT|O_TRUNC);
	if (-1 == fd)
	{
		printf("open weather file error.\n");
		goto end;
	}
	//write file
	ret = write(fd, bureau_show_weather, strlen(bureau_show_weather));
	if(-1 == ret)
	{
		printf("write weather file error.\n");
		goto end;
	}
	close(fd);
	
	
	end:
	return 0;
}



