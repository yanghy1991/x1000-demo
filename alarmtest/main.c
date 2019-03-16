#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv)
{
	
	if(argc < 3)
	{
		printf("usage: cmd id topic\n");
		return 0;
	}
	
//	printf("argv0 = %s \t argv1 = %s \t argv2 = %s \n",argv[0],argv[1],argv[2]);
	char cronStr[1024];
	memset(cronStr,0,1024);
	strcat(cronStr,"echo \"");
	strcat(cronStr,argv[0]);
	strcat(cronStr," ");
	strcat(cronStr,argv[1]);
	strcat(cronStr," ");
	strcat(cronStr,argv[2]);
	strcat(cronStr,"\" >> /home/others/x1000_my_demo/alarmtest/test.txt");
//	strcat(cronStr,"\" >> /tmp/test1.txt");
	system(cronStr);
	
//	system("echo \"test\" >> /home/others/x1000_my_demo/alarmtest/test.txt");
	
	return 0;
}