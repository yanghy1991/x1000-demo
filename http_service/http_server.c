#include "bd_asr.h"

//获取acess_token
int get_access_token(char *access_token)
{
	int is_token_ok = -1;
	int fd = -1;
	int ret = 0;
	char token_buf[ASR_SMALL_BUFSIZE];
	memset(token_buf,0,ASR_SMALL_BUFSIZE);
	
	//检测文件是否存在
	if (access("data/bdtoken.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/bdtoken.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, token_buf, ASR_SMALL_BUFSIZE-1)) >= 0)
			{
				if(strstr(token_buf,"nodata") == NULL)
				{
					strcpy(access_token,token_buf);
					is_token_ok = 0;
					close(fd);
					return is_token_ok;
				}
			}
		}
	}
	
	//检测文件是否存在
	if (access("data/bdtoken_bk.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/bdtoken_bk.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, token_buf, ASR_SMALL_BUFSIZE-1)) >= 0)
			{
				if(strstr(token_buf,"nodata") == NULL)
				{
					strcpy(access_token,token_buf);
					is_token_ok = 0;
					close(fd);
					return is_token_ok;
				}
			}
		}
	}
	
	return is_token_ok;
}

int main(int argc , char **argv)
{
	
	//获取access_token
	char access_token[ASR_SMALL_BUFSIZE];
	memset(access_token,0,ASR_SMALL_BUFSIZE);
	if(-1 == get_access_token(access_token))
	{
		printf("get access token error.\n");
		goto end;
	}
	
	//录音
	system("arecord -c 1 -r 8000 -f S16_LE -d 4 data/tq.wav");
	//system("arecord -c 1 -r 8000 -f S16_LE -D plughw:1,0 -d 4 data/tq.wav");
	
	printf("starting ......\n");
	
	//asr
	char result[ASR_RESP_BUFSIZE];
	memset(result,0,ASR_RESP_BUFSIZE);
	char err_msg[ASR_SMALL_BUFSIZE];
	memset(err_msg,0,ASR_SMALL_BUFSIZE);
	int asr_err_no = 1;
	int *error_no = &asr_err_no;
	if(-1 == asr_http_post("vop.baidu.com","data/tq.wav","wav",access_token,"00:0c:29:fb:d7:52",result,err_msg,error_no))
	{
		printf("asr from baidu error.\n");
		goto end;
	}
	printf("result : \n%s\n",result);
	printf("err_msg : \n%s\n",err_msg);
	printf("asr_err_no : %d\n",asr_err_no);
	
	//char *url = "localhost:8080/bingo/cmd?deviceId=szhy001000001&cmd=\"今天天气怎么样\"";
	char url[ASR_RESP_BUFSIZE];
	memset(url,0,ASR_RESP_BUFSIZE);	
	char rspMsg[ASR_RESP_BUFSIZE];
	memset(rspMsg,0,ASR_RESP_BUFSIZE);
	
	
	strcat(url,"192.168.64.1:8080/bingo/cmd?");
	strcat(url,"deviceId=szhy001000001");
	strcat(url,"&cmd=");
	strcat(url,result);
	//printf("url = %s\n",url);
	if(strlen(url) <= 0)
	{
		goto end;
	}

	if(-1 == get_msg_from_server(url,rspMsg))
	{
		printf("rspMsg http request error.\n");
		goto end;
	}
	printf("rspMsg = %s\n",rspMsg);
	

end:
	return 0;
}