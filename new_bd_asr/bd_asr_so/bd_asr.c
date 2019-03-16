#include "bd_asr.h" 

/**
*功能:使用cJSON生成json字符串
*参数:
*format_data:语音压缩的格式,不区分大小写,pcm（不压缩）、wav、opus、speex、amr
*token_data:开放平台获取到的开发者 access_token
*cuid:用户唯一标识，用来区分用户，填写机器 MAC 地址或 IMEI 码，长度为60以内
*base64_data:真实的语音数据 ，需要进行base64 编码
*data_len:原始语音长度，单位字节
*
*/
char * asr_make_json(char* format_data,char *token_data,char *cuid,char* base64_data, int data_len)
{
	cJSON * pJsonRoot = NULL;
	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
		printf("%s : create cJSON failed!\n",__func__);
		return NULL;
	}
	cJSON_AddStringToObject(pJsonRoot, "format", format_data);
	cJSON_AddStringToObject(pJsonRoot, "token", token_data);
	cJSON_AddStringToObject(pJsonRoot, "cuid", cuid);
	cJSON_AddNumberToObject(pJsonRoot, "rate", 16000);
	cJSON_AddNumberToObject(pJsonRoot, "channel", 1);
	cJSON_AddNumberToObject(pJsonRoot, "len", data_len);
	cJSON_AddStringToObject(pJsonRoot, "speech", base64_data);
	
	char * p = cJSON_Print(pJsonRoot);
	if(NULL == p)
	{
		cJSON_Delete(pJsonRoot);
		printf("%s : create cJSON string failed!\n",__func__);
		return NULL;
	}
	cJSON_Delete(pJsonRoot);
	
	return p;	
}

/**
*功能:判断http返回是否正确
*参数:
*response:http响应的字符串
*
*/
int asr_get_http_result(char *response)  
{ 
    char *ptmp = NULL; 

	//没有http响应头
    ptmp = (char*)strstr(response,"HTTP/1.1");  
    if(!ptmp){  
        printf("%s : http/1.1 not find.\n",__func__);  
        return -1;  
    }
	//HTTP访问出错
    if(atoi(ptmp + 9)!=200){  
        printf("%s : http response no OK(200).\n",__func__);  
        return -1;  
    }
	
	return 0;
} 

/**
*功能:对http响应的json字符串进行解析,解析出参数err_msg,err_no,result
*参数:
*json_str:json字符串
*error_msg:用来存储err_msg的内容
*result_data:用来存储result的值
*error_no:用来存储err_no的内容
*
*/
int asr_parse_json(char *json_str,char *error_msg,char *result_data,int *error_no)
{
	//printf("%s\n",json_str);
	cJSON * root = NULL;
    cJSON * item = NULL;
	cJSON * err_no = NULL;
	cJSON * err_msg = NULL;
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	else
	{
		err_no = cJSON_GetObjectItem(root, "err_no");
		if(NULL == err_no)
		{
			printf("%s : get err_no failed!\n",__func__);
			return -1;//获取err_no失败
		}
		//printf("err_no : %d\n", err_no->valueint);
		*error_no = err_no->valueint;
		
		err_msg = cJSON_GetObjectItem(root, "err_msg");
		if(NULL == err_msg)
		{
			printf("%s : get err_msg failed!\n",__func__);
			return -1;//获取err_msg失败
		}
		//printf("err_msg : %s\n", cJSON_Print(err_msg));
		strcpy(error_msg,cJSON_Print(err_msg));
				
		item = cJSON_GetObjectItem(root, "result");
		if(NULL == item)
		{
			printf("%s : get result failed!\n",__func__);
			strcpy(result_data,"");
			return 0;//获取result失败
		}
		strcpy(result_data,cJSON_Print(item));
		
		return 0;
	}
}

/**
*功能:语音识别http请求
*参数:
*host_data:地址字符串，不带http:// ,例如vop.baidu.com
*wav_path:wav文件路径(需要进行语音识别的文件)
*format_data:语音压缩的格式,不区分大小写,pcm（不压缩）、wav、opus、speex、amr
*token_data:开放平台获取到的开发者 access_token
*cuid:用户唯一标识，用来区分用户，填写机器 MAC 地址或 IMEI 码，长度为60以内
*result:用来存储result的值
*error_msg:用来存储err_msg的内容
*error_no:用来存储err_no的内容
*
*/
int asr_http_post(char *host_data,char *wav_path,char* format_data,char *token_data,char *cuid,char *result,char *err_msg,int *error_no)
{
	//读取音频文件
	FILE *wav_fp = NULL;
    wav_fp = fopen(wav_path, "r");
	//文件打开失败
    if (NULL == wav_fp)
	{
		printf("%s : open audio file failed.\n",__func__);  
		return -1;
	}
		
	//获取音频文件大小
	fseek(wav_fp, 0, SEEK_END);
	int wav_len = ftell(wav_fp);	
    fseek(wav_fp, 0, SEEK_SET);
	//把文件读到内存
	char *wav = (char *)malloc(wav_len);
    fread(wav, wav_len, sizeof(char), wav_fp);
	
	//把音频文件进行base64编码
	char base64data[wav_len*2]; 
	memset(base64data,0,wav_len*2);
	base64_encode((const unsigned char *)wav,base64data,wav_len);
	
	//构建HTTP访问
	char content_page[] = "POST /server_api HTTP/1.1\r\n";
	char content_host[] = "HOST: vop.baidu.com\r\n";  
	char content_type[] = "Content-Type: application/json\r\n";
	char content_len[ASR_SMALL_BUFSIZE];
	memset(content_len,0,ASR_SMALL_BUFSIZE);
	//构建json参数
	char *json_str = asr_make_json(format_data,token_data,cuid,base64data,wav_len);
	sprintf(content_len,"Content-Length: %d\r\n\r\n",strlen(json_str));
	//创建访问体
	char content[ASR_REQUEST_BUFSIZE];
	memset(content,0,ASR_REQUEST_BUFSIZE);
	sprintf(content,"%s%s%s%s%s",content_page,content_host,content_type,content_len,json_str);
		
	//构建socket访问
	//添加host
	char host[ASR_SMALL_BUFSIZE];
	memset(host,0,ASR_SMALL_BUFSIZE);
	strcpy(host,host_data);
	//1.创建socket
	int soc_fd;
	struct sockaddr_in sock_addr;
	soc_fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == soc_fd)
    {
        printf("%s : create socket failed of client.\n",__func__);
		fclose(wav_fp);
        return -1;//socket创建失败
    }
	
	struct hostent *host2ip = NULL;
	
	host2ip = gethostbyname(host);//将上面获得的主机信息通过域名解析函数获得域>名信息
	//设置IP地址结构
    bzero(&sock_addr,sizeof(struct sockaddr_in));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = *((unsigned long*)host2ip->h_addr_list[0]); 
	//sock_addr.sin_addr.s_addr = inet_addr("115.239.217.183");
	sock_addr.sin_port = htons(DEFAULT_PORT);
	
	//2.把socket设置为非阻塞模式
    int fdflags = fcntl(soc_fd, F_GETFL, 0);
    if(fcntl(soc_fd, F_SETFL, fdflags | O_NONBLOCK) < 0)
    {
        printf("fcntl set nonblock failed \n");
		close(soc_fd);
        return -1;
    }
	
	//3.连接服务器
	int addrlen = sizeof(sock_addr);
	if(do_bdasr_connect(soc_fd, (struct sockaddr *)&sock_addr, &addrlen) == false)
    {
		printf("%s : connect to server failed \n",__func__);
        close(soc_fd);
		fclose(wav_fp);
		return -1;
    }

//	printf("\n\ncontent = \n\n%s\n",content);
	//4.向服务器发送url请求的request	
	
	int sendlen = do_bdasr_send(soc_fd, content);
	if(sendlen < 0)
	{
		printf("%s : send to server failed \n",__func__);
        close(soc_fd);
		fclose(wav_fp);
		return -1;
	}

	//5.客户端接收服务器的返回信息
	static char recv_buffer[ASR_RESP_BUFSIZE];
    memset(recv_buffer,0,ASR_RESP_BUFSIZE);
	int recvlen = do_bdasr_recv(soc_fd, recv_buffer, ASR_RESP_BUFSIZE) ;
	if(recvlen < 0)
	{
		printf("%s : recv from server failed \n",__func__);
        close(soc_fd);
		fclose(wav_fp);
		return -1;
	}

//	printf("recv_buffer is : \n%s\n",recv_buffer);
	
	//json解析
	int response_flag = asr_get_http_result(recv_buffer);
	if(response_flag == 0)
	{
		char *result_json = NULL;
		result_json = strstr(recv_buffer,"{");
		int re_num = asr_parse_json(result_json,err_msg,result,error_no);
		if(re_num == -1)
		{
			printf("%s : parse json error.\n",__func__);
			fclose(wav_fp);
			close(soc_fd);
			return -1;//解析json失败
		}
	}
	else
	{
		printf("%s : the http response data error.\n",__func__);
		fclose(wav_fp);
		close(soc_fd);
		return -1;//数据返回出错
	}
	
	fclose(wav_fp);
    close(soc_fd);
	return 0;
}



/**
*功能:从文件获取百度accesstoken
*参数:
*host_data:地址字符串，不带http:// ,例如vop.baidu.com
*wav_path:wav文件路径(需要进行语音识别的文件)
*format_data:语音压缩的格式,不区分大小写,pcm（不压缩）、wav、opus、speex、amr
*token_data:开放平台获取到的开发者 access_token
*cuid:用户唯一标识，用来区分用户，填写机器 MAC 地址或 IMEI 码，长度为60以内
*result:用来存储result的值
*error_msg:用来存储err_msg的内容
*error_no:用来存储err_no的内容
*
*/
int get_access_token(char *file_path,char *access_token)
{
	int is_token_ok = -1;
	int fd = -1;
	int ret = 0;
	char token_buf[ASR_SMALL_BUFSIZE];
	memset(token_buf,0,ASR_SMALL_BUFSIZE);
	
	printf("file_path = %s\n",file_path);
	
	//检测文件是否存在
	if (access(file_path,F_OK) == 0)
	{
		//open file
		fd = open(file_path, O_RDONLY);
		if (-1 != fd)
		{
			//read file
			if((ret = read(fd, token_buf, ASR_SMALL_BUFSIZE-1)) >= 0)
			{
				strcpy(access_token,token_buf);
				is_token_ok = 0;
				close(fd);
			}
		}
	}
	
	return is_token_ok;
}


/**
*功能:百度语音识别接口
*参数:
*host_data:地址字符串，不带http:// ,例如vop.baidu.com
*wav_path:wav文件路径(需要进行语音识别的文件)
*format_data:语音压缩的格式,不区分大小写,pcm（不压缩）、wav、opus、speex、amr
*token_data:开放平台获取到的开发者 access_token
*cuid:用户唯一标识，用来区分用户，填写机器 MAC 地址或 IMEI 码，长度为60以内
*result:用来存储result的值
*error_msg:用来存储err_msg的内容
*error_no:用来存储err_no的内容
*
*/

int bd_asr(char *file_path,char *audio_path,char *audio_format,char *cuid,char *result,char *err_msg,int *error_no)
{
	//1.获取access_token
	char access_token[ASR_SMALL_BUFSIZE];
	memset(access_token,0,ASR_SMALL_BUFSIZE);
	if(-1 == get_access_token(file_path,access_token))
	{
		printf("get access token error.\n");
		return -1;
	}
	
	//2.语音识别
	if(-1 == asr_http_post("vop.baidu.com",audio_path,audio_format,access_token,cuid,result,err_msg,error_no))
	{
		printf("asr from baidu error.\n");
		return -2;
	}
	
	return 0;
}



















