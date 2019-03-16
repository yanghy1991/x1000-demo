#include "city_sohu.h"

/**
*功能:对http响应的json字符串进行解析,解析出参数city
*参数:
*json_str:json字符串
*city:用来存储city
*
*/
int sohu_city_parse_json(char *json_str,char *city_name,char *city_code)
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

int bkup_file(char *src_path,char *dest_path)
{
	char city_buf[SOHU_COM_BUFSIZE];
	memset(city_buf,0,SOHU_COM_BUFSIZE);
	//检测文件是否存在
	if (access(src_path,F_OK) == -1)
	{
		printf("%s not exist.\n",src_path);
		return -1;
	}
	else
	{
		//从文件中读取当前城市名
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
		ret = read(fd, city_buf, SOHU_COM_BUFSIZE-1);
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
				printf("city name is %s\n",city_buf);
				char command[SOHU_COM_BUFSIZE];
				memset(command,0,SOHU_COM_BUFSIZE);
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

int main()
{
	//备份城市文件
	if(-1 == bkup_file("data/sohu_city_gb.txt","data/sohu_city_gb_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/sohu_city_code.txt","data/sohu_city_code_bk.txt"))
		printf("backup file error.\n");
	if(-1 == bkup_file("data/sohu_city.txt","data/sohu_city_bk.txt"))
		printf("backup file error.\n");
	
	int fd = -1;
	int ret = 0;
	
	//从网络获取当前城市名
	char *url = "pv.sohu.com/cityjson?ie=utf-8";
	char city[SOHU_COM_BUFSIZE];
	memset(city,0,SOHU_COM_BUFSIZE);	
	if(-1 == sohu_get_city(url,city))
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
	
	
	
	char city_code[SOHU_SMALL_BUFSIZE];
	memset(city_code,0,SOHU_SMALL_BUFSIZE);
	//检测文件是否存在和获取城市代码
	if (access("data/city_json_use.txt",F_OK) == -1)
	{
		printf("data/city_json_use.txt not exist.\n");
		goto end;
	}
	else
	{
		int i = 0;
		for(i=1;i<36;i++)
		{
			char line_buf[SOHU_LINE_BUFSIZE];
			memset(line_buf,0,SOHU_LINE_BUFSIZE);
			if(-1 == get_file_line("data/city_json_use.txt",line_buf,i))
			{
				printf("get line from file error.\n");
				goto end;
			}
			//printf("line buf = %s\n",line_buf);
			if(-1 == sohu_city_parse_json(line_buf,city,city_code))
			{
				printf("get city_code error.\n");				
			}
			else
			{
				if(strlen(city_code) != 0)
				{
					printf("city_code = %s\n",city_code);
					break;
				}	
			}	
		}
	}
	
	//存储天气预报城市代码
	if(strlen(city_code) <= 0)
	{
		printf("get city_code error.\n");
		strcpy(city_code,"nodata");
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
			
	
	//open file
	fd = open("data/sohu_city_code.txt", O_RDWR|O_CREAT|O_TRUNC);
	if (-1 == fd)
	{
		printf("open sohu_city_code.txt error.\n");
		goto end;
	}
	//write file
	ret = write(fd, city_code, strlen(city_code));
	if(-1 == ret)
	{
		printf("write sohu_city_code.txt error.\n");
		goto end;
	}
	close(fd);	
	
	//把城市名的gb2312码存入文件
	char city_gb[SOHU_COM_BUFSIZE];
	memset(city_gb,0,SOHU_COM_BUFSIZE);
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
			char line_buf[SOHU_LINE_BUFSIZE];
			memset(line_buf,0,SOHU_LINE_BUFSIZE);
			if(-1 == get_file_line("data/city_gb_json_use.txt",line_buf,i))
			{
				printf("get line from file error.\n");
				goto end;
			}
			//printf("line buf = %s\n",line_buf);
			if(-1 == sohu_city_parse_json(line_buf,city,city_gb))
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
		
	//存储城市名国标码
	if(strlen(city_gb) <= 0)
	{
		printf("get city_code error.\n");
		strcpy(city_gb,"nodata");
	}
			
	//open file
	fd = open("data/sohu_city_gb.txt", O_RDWR|O_CREAT|O_TRUNC);
	if (-1 == fd)
	{
		printf("open sohu_city_gb.txt error.\n");
		goto end;
	}
	//write file
	ret = write(fd, city_gb, strlen(city_gb));
	if(-1 == ret)
	{
		printf("write sohu_city_gb.txt error.\n");
		goto end;
	}
	close(fd);
	
	//存储城市名汉字
	//open file
	fd = open("data/sohu_city.txt", O_RDWR|O_CREAT|O_TRUNC);
	if (-1 == fd)
	{
		printf("open sohu_city.txt error.\n");
		goto end;
	}
	//write file
	ret = write(fd, city, strlen(city));
	if(-1 == ret)
	{
		printf("write sohu_city.txt error.\n");
		goto end;
	}
	close(fd);
	
	
	end:
	return 0;
}