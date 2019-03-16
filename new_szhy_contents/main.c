#include "szhy_content.h"

int main(int argc , char **argv)
{
	char result[CONTENT_RESP_BUFSIZE];
	memset(result,0,CONTENT_RESP_BUFSIZE);
	stpcpy(result,"青蛙王子的故事");
	//char *url = "localhost:8080/bingo/cmd?deviceId=szhy001000001&cmd=\"今天天气怎么样\"";
	//char *url = "39.108.71.229:8080/bingo/cmd?deviceId=szhy001000001&cmd=\"今天天气怎么样\"";
	char url[CONTENT_RESP_BUFSIZE];
	memset(url,0,CONTENT_RESP_BUFSIZE);	
	char rspMsg[CONTENT_RESP_BUFSIZE];
	memset(rspMsg,0,CONTENT_RESP_BUFSIZE);

	strcat(url,"39.108.71.229:8080/bingo/cmd?");
	strcat(url,"deviceId=szhy001000001");
	strcat(url,"&cmd=");
	strcat(url,result);
	
	//把结果存到文件
	if(-1 == get_content_server(url,rspMsg))
	{
		printf("rspMsg http request error.\n");
		goto end;
	}
	printf("rspMsg = %s\n",rspMsg);

end:
	return 0;
}