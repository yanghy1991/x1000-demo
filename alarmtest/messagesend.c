#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <errno.h>

#define MAX_TEXT 512

struct msg_st
{
	long int msg_type;
	char text[MAX_TEXT];
};

int main(int argc,char **argv)
{	
	if(argc < 3)
	{
		printf("usage: cmd id topic\n");
		return 0;
	}
	printf("argv1 = %s,argv2 = %s,argv3 = %s\n", argv[0],argv[1],argv[2]);
	
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