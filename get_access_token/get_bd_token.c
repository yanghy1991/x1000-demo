#include "bd_token.h"

int bkup_file(char *src_path,char *dest_path)
{
	char city_buf[TOKEN_TOKEN_BUFSIZE];
	memset(city_buf,0,TOKEN_TOKEN_BUFSIZE);
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
		ret = read(fd, city_buf, TOKEN_TOKEN_BUFSIZE-1);
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
				char command[TOKEN_TOKEN_BUFSIZE];
				memset(command,0,TOKEN_TOKEN_BUFSIZE);
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


int main(int argc , char **argv)
{
	//备份acces_token文件
	if(-1 == bkup_file("data/bdtoken.txt","data/bdtoken_bk.txt"))
		printf("backup file error.\n");
	
	//get token
	char request_content[TOKEN_COMMON_BUFSIZE];
	memset(request_content,0,TOKEN_COMMON_BUFSIZE);
	if(-1 == token_create_parameters("openapi.baidu.com/oauth/2.0/token?","d5NTk2rGHZ3GTIVBTQ5ZyCN2","685267784b4f62e50beb0784441a6615",request_content))
	{
		printf("token create request error.\n");
		goto end;
	}	
	printf("token http : \n%s\n",request_content);
	
	char access_token[TOKEN_TOKEN_BUFSIZE];
	memset(access_token,0,TOKEN_TOKEN_BUFSIZE);
	if(-1 == bd_get_token(request_content,access_token))
	{
		printf("get token from baidu error.\n");
		goto end;
	}
	
	printf("access_token : \n%s\n",access_token);
	
	if(strlen(access_token) <= 0)
	{
		printf("get token error.\n");
		strcpy(access_token,"nodata");
	}
	
	//DIR *dirp = NULL;
	if(opendir("data") == NULL)  
	{  
		if(mkdir("data",0777) == -1)
		{
			printf("mkdir -data- error.\n");
			return -1;
		}
	}
		
	int fd = -1;
	int ret = 0;
	//open file
	fd = open("data/bdtoken.txt", O_RDWR|O_CREAT|O_TRUNC);
	if (-1 == fd)
	{
		printf("open bdtoken.txt error.\n");
		return -1;
	}
	//write file
	ret = write(fd, access_token, strlen(access_token));
	if(-1 == ret)
	{
		printf("write bdtoken.txt error.\n");
		return -1;
	}
	close(fd);
	
	
	end:
	return 0;
}