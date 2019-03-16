#include <time.h>//C语言的头文件
#include <stdio.h>//C语言的I/O
#include <string.h>
int main(int argc,char **argv)
{
	time_t now;//实例化time_t结构
	struct tm *timenow;//实例化tm结构指针
	time(&now);
	//time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
	timenow   =   localtime(&now);
	//localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)
	printf("Local time is:%s/n",asctime(timenow));
	//上句中asctime函数把时间转换成字符，通过printf()函数输出
	
	char current[128];
	memset(current,0,128);
    sprintf(current, "%04d%02d%02d%02d%02d%02d", timenow->tm_year + 1900, timenow->tm_mon+1, timenow->tm_mday, 
												   timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
	printf("current = %s\n",current);
	return 0;
}