#include "city_ipinfo.h"

int bkup_file()
{
	char city_buf[IPCITY_COM_BUFSIZE];
	memset(city_buf,0,IPCITY_COM_BUFSIZE);
	//检测文件是否存在
	if (access("data/ipinfo_city.txt",F_OK) == -1)
	{
		printf("data/ipinfo_city.txt not exist.\n");
		return -1;
	}
	else
	{
		//从文件中读取当前城市名
		int fd = -1;
		int ret = 0;
		//open file
		fd = open("data/ipinfo_city.txt", O_RDONLY);
		if (-1 == fd)
		{
			printf("open ipinfo_city.txt error.\n");
			return -1;
		}
		//read file
		ret = read(fd, city_buf, IPCITY_COM_BUFSIZE-1);
		if(ret < 0)
		{
			printf("read city name error.\n");
			return -1;
		}
		else
		{
			if((strlen(city_buf) <= 0) || (strstr(city_buf,"nodata") != NULL))
			{
				printf("ipinfo_city.txt data error.\n");
				return -1;
			}
			else
			{
				printf("city name is %s\n",city_buf);
				system("cp data/ipinfo_city.txt data/ipinfo_city_bk.txt");
			}
		}			
	}
	
	return 0;
}

int main()
{
	if(-1 == bkup_file())
		printf("backup file error.\n");
	
	
	char *url = "ipinfo.io/";
	char city[IPCITY_COM_BUFSIZE];
	memset(city,0,IPCITY_COM_BUFSIZE);
	
	if(-1 == ipinfo_get_city(url,city))
	{
		printf("city http request error.\n");
		goto end;
	}
	printf("city = %s\n",city);
	
	if(strlen(city) <= 0)
	{
		printf("get city error.\n");
		strcpy(city,"nodata");
	}
	
	//DIR *dirp = NULL;
	if(opendir("data") == NULL)  
	{  
		if(mkdir("data",0777) == -1)
		{
			printf("mkdir -data- error.\n");
			goto end;
		}
	}
		
	int fd = -1;
	int ret = 0;
	//open file
	fd = open("data/ipinfo_city.txt", O_RDWR|O_CREAT|O_TRUNC);
	if (-1 == fd)
	{
		printf("open ipinfo_city.txt error.\n");
		goto end;
	}
	//write file
	ret = write(fd, city, strlen(city));
	if(-1 == ret)
	{
		printf("write ipinfo_city.txt error.\n");
		goto end;
	}
	close(fd);
	
	end:
	return 0;
}