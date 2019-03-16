#include "city_sina.h"

int bkup_file()
{
	char city_buf[SINACITY_COM_BUFSIZE];
	memset(city_buf,0,SINACITY_COM_BUFSIZE);
	//检测文件是否存在
	if (access("data/sina_city.txt",F_OK) == -1)
	{
		printf("data/sina_city.txt not exist.\n");
		return -1;
	}
	else
	{
		//从文件中读取当前城市名
		int fd = -1;
		int ret = 0;
		//open file
		fd = open("data/sina_city.txt", O_RDONLY);
		if (-1 == fd)
		{
			printf("open sina_city.txt error.\n");
			return -1;
		}
		//read file
		ret = read(fd, city_buf, SINACITY_COM_BUFSIZE-1);
		if(ret < 0)
		{
			printf("read city name error.\n");
			return -1;
		}
		else
		{
			if((strlen(city_buf) <= 0) || (strstr(city_buf,"nodata") != NULL))
			{
				printf("sina_city.txt data error.\n");
				return -1;
			}
			else
			{
				printf("city name is %s\n",city_buf);
				system("cp data/sina_city.txt data/sina_city_bk.txt");
			}
		}			
	}
	
	return 0;
}

int sina_city_parse_json(char *json_str,char *city_name,char *city_code)
{
	//printf("json_str = %s\n",json_str);
	cJSON * root = NULL;
    cJSON * item = NULL;
	root = cJSON_Parse(json_str);
	if (!root) 
    {
		printf("%s : create json object return.\n",__func__);
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
    }
	else
	{
		item = cJSON_GetObjectItem(root, city_name);
		if(item ==NULL)
		{
			printf("there is no the item %s\n",city_name);
			return -1;
		}
		char *tmp = cJSON_Print(item);
		tmp[strlen(tmp)-1] = 0;
		strcpy(city_code,tmp+1);
		free(tmp);
		return 0;
	}
}


int get_file_line(char *pInputName, char *pOutputBuf, int cnt)  
{  
    FILE * fp;  
    int i=0;  
    char * line = NULL;  
    size_t len = 0;  
    ssize_t read;  
  
    fp = fopen(pInputName, "r");  
    if (fp == NULL)
	{
		printf("open city_json_use.txt error.\n");
		return -1;
	}		
    if(cnt < 0)
	{
		printf("line number must >= 0.\n");
		return -1;
	}		
           
           
    while ((read = getline(&line, &len, fp)) != -1) {  
        ++i;  
        if(i >= cnt)  
            break;  
    }  
  
    if (line)  
    {  
        memcpy(pOutputBuf,line,strlen(line));  
        free(line);  
        return 0;   
    }  
	printf("get line error.\n");
    return -1;  
}

int main()
{
	//备份城市文件
	if(-1 == bkup_file())
		printf("backup file error.\n");
	
	//从网络获取当前城市名
	char *url = "int.dpool.sina.com.cn/iplookup/iplookup.php?format=json";
	
	char city[SINACITY_COM_BUFSIZE];
	memset(city,0,SINACITY_COM_BUFSIZE);

	if(-1 == sina_get_city(url,city))
	{
		printf("city http request error.\n");
		goto end;
	}
	printf("city = %s\n",city);
	
	//查询城市国标码
	char city_gb[SINACITY_COM_BUFSIZE];
	memset(city_gb,0,SINACITY_COM_BUFSIZE);
	//检测文件是否存在和获取城市代码
	if (access("data/city_gb_json_use.txt",F_OK) == -1)
	{
		printf("data/city_gb_json_use.txt not exist.\n");
		goto end;
	}
	else
	{
		int i = 0;
		for(i=1;i<36;i++)
		{
			char line_buf[SINACITY_LINE_BUFSIZE];
			memset(line_buf,0,SINACITY_LINE_BUFSIZE);
			if(-1 == get_file_line("data/city_gb_json_use.txt",line_buf,i))
			{
				printf("get line from file error.\n");
				goto end;
			}
			//printf("line buf = %s\n",line_buf);
			if(-1 == sina_city_parse_json(line_buf,city,city_gb))
			{
				printf("get city_gb error.\n");				
			}
			else
			{
				if(strlen(city_gb) != 0)
				{
					printf("city_gb == %s\n",city_gb);
					break;
				}	
			}	
		}
	}
	
	if(strlen(city_gb) <= 0)
	{
		printf("get city_gb error.\n");
		strcpy(city_gb,"nodata");
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
	fd = open("data/sina_city.txt", O_RDWR|O_CREAT|O_TRUNC);
	if (-1 == fd)
	{
		printf("open sina_city.txt error.\n");
		goto end;
	}
	//write file
	ret = write(fd, city_gb, strlen(city_gb));
	if(-1 == ret)
	{
		printf("write sina_city.txt error.\n");
		goto end;
	}
	close(fd);
		
	end:
	return 0;
}