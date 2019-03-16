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

#define TIME_JSON_BUFSIZE 2048
#define ORGIN_LEN 128
#define TIME_LEN 32

//主函数
int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("useage: command clockstring.\n");
		goto end;
	}
	
	char *src_path = "/etc/crontab";
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

