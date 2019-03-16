#include "bd_tts.h"

//获取tts字符串
int get_tts_str(char *file_path,char *tts_str)
{
	int is_tts_str_ok = -1;
	int fd = -1;
	int ret = 0;
	char tts_str_buf[TTS_REQ_BUFSIZE];
	memset(tts_str_buf,0,TTS_REQ_BUFSIZE);
	
	//检测文件是否存在
	if (access(file_path,F_OK) == 0)
	{
		//open file
		fd = open(file_path, O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, tts_str_buf, TTS_REQ_BUFSIZE-1)) >= 0)
			{
				if(strstr(tts_str_buf,"nodata") == NULL)
				{
					strcpy(tts_str,tts_str_buf);
					is_tts_str_ok = 0;
					close(fd);
					return is_tts_str_ok;
				}
			}
		}
	}
	
	return is_tts_str_ok;
}

//获取acess_token
int get_access_token(char *access_token)
{
	int is_token_ok = -1;
	int fd = -1;
	int ret = 0;
	char token_buf[TTS_SMALL_BUFSIZE];
	memset(token_buf,0,TTS_SMALL_BUFSIZE);
	
	//检测文件是否存在
	if (access("data/bdtoken.txt",F_OK) == 0)
	{
		//open file
		fd = open("data/bdtoken.txt", O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, token_buf, TTS_SMALL_BUFSIZE-1)) >= 0)
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
			if((ret = read(fd, token_buf, TTS_SMALL_BUFSIZE-1)) >= 0)
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

int main(int argc,char **argv)
{
	if(argc < 3)
	{
		printf("usage:bd_tts_exe strfilepath mp3filepath\n");
		goto end;
	}
	
	//检测文件是否存在,以及获取access token
	char access_token[TTS_SMALL_BUFSIZE];
	memset(access_token,0,TTS_SMALL_BUFSIZE);
	if(-1 == get_access_token(access_token))
	{
		printf("can't get access token.\n");
		goto end;
	}
	
	//读取需要转换的文件内容
	char tts_str[TTS_REQ_BUFSIZE];
	memset(tts_str,0,TTS_REQ_BUFSIZE);
	/*
	if(-1 == get_tts_str(argv[1],tts_str))
	{
		printf("get tts str error.\n");
		goto end;
	}
	*/
	strcpy(tts_str,argv[1]);
	
	printf("tts_str = %s\n",tts_str);
	
	//tts
	char tts_parameters[TTS_REQ_BUFSIZE];
	memset(tts_parameters,0,TTS_REQ_BUFSIZE);
	int tts_err_no = 1;
	int *tts_error_no = &tts_err_no;
	if(-1 == tts_create_parameters(tts_str,"zh",access_token,"1","00:0c:29:fb:d7:52","5","5","5","0",tts_parameters))
	{
		printf("baidu tts create request error.\n");
		goto end;
	}
	printf("tts http : \n%s\n",tts_parameters);
	
	if(-1 == tts_http_post(argv[2],tts_parameters,tts_error_no))
	{
		printf("tts from baidu error.\n");
		goto end;
	}
	printf("tts err_no : \n%d\n",tts_err_no);
	
	end:
	return 0;
}