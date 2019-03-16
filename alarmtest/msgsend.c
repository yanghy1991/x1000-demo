#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <errno.h>
#include <time.h>

#define MAX_TEXT 512

struct msg_st
{
	long int msg_type;
	char text[MAX_TEXT];
};


//获取当前时间
int getNowTime(char *nowTime)
{
    time_t now;//实例化time_t结构
	struct tm *timenow;//实例化tm结构指针
	time(&now);
	//time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
	timenow   =   localtime(&now);
	//localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)
	//printf("Local time is:%s\n",asctime(timenow));
	//上句中asctime函数把时间转换成字符，通过printf()函数输出
	
	char current[128];
	memset(current,0,128);
    sprintf(current, "%d-%d-%d-%d-%d-%d-%d", timenow->tm_year + 1900, timenow->tm_mon+1, timenow->tm_mday, 
												   timenow->tm_hour, timenow->tm_min, timenow->tm_sec, timenow->tm_wday);
	printf("current = %s\n",current);
	strcpy(nowTime,current);
}

int main(int argc,char **argv)
{	
	if(argc < 3)
	{
		printf("usage: cmd id topic\n");
		return 0;
	}
	printf("argv1 = %s,argv2 = %s,argv3 = %s\n", argv[0],argv[1],argv[2]);
	
	char currenttime[128];
	memset(currenttime,0,128);
	getNowTime(currenttime);
		
	struct msg_st alarm_data;
	char buffer[BUFSIZ];
	int msgid = -1;
	
	//建立消息队列
	msgid = msgget((key_t)4781, 0666 | IPC_CREAT);
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	//向消息队列中写消息
	strcpy(buffer,argv[2]);
	alarm_data.msg_type = 1;    //注意2
	strcpy(alarm_data.text, buffer);
		
	//向队列发送数据
	if(msgsnd(msgid, (void*)&alarm_data, MAX_TEXT, 0) == -1)
	{
		fprintf(stderr, "msgsnd failed\n");
		exit(EXIT_FAILURE);
	}
	printf("msgsend end ......\n");	
	exit(EXIT_SUCCESS);
	return 0;
}