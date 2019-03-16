#include "bd_analysis_words.h" 

//utf-8转gbk
int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen,  
        char *outbuf, size_t outlen) {  
    iconv_t cd;  
    char **pin = &inbuf;  
    char **pout = &outbuf;  
  
    cd = iconv_open(to_charset, from_charset);  
    if (cd == 0)  
        return -1;  
    memset(outbuf, 0, outlen);  
    if (iconv(cd, pin, &inlen, pout, &outlen) == -1)  
        return -1;  
    iconv_close(cd);  
    *pout = '\0';  
  
    return 0;  
}  
  
int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {  
    return code_convert("utf-8", "gbk", inbuf, inlen, outbuf, outlen);  
}

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
char * ananlysis_make_json(char* text)
{	
	cJSON * pJsonRoot = NULL;
	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
		printf("%s : create cJSON failed!\n",__func__);
		return NULL;
	}
	cJSON_AddStringToObject(pJsonRoot, "text", text);
	
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
			return -1;//获取result失败
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
int analysis_http_post(char *host_data)
{
	
	//构建HTTP访问
	char content_page[] = "POST /rpc/2.0/nlp/v1/lexer?access_token=24.c392e93276ab23f77cc71902c190d476.2592000.1499588372.282335-9747262 HTTP/1.1\r\n";
	char content_host[] = "HOST: aip.baidubce.com\r\n";  
	char content_type[] = "Content-Type: application/json\r\n";
	char content_len[ASR_SMALL_BUFSIZE];
	memset(content_len,0,ASR_SMALL_BUFSIZE);
	//构建json参数
	char *json_str = ananlysis_make_json("百度是一家高科技公司");
	
	char *s = "百度是一家高科技公司";  
    char buf[128];  
    u2g(json_str, strlen(json_str), buf, sizeof(buf)); 
	int i = 0;
	for(i=0;i<sizeof(buf);i++)
	{
		printf("0x%x",buf[i]);
	}
	
	//char *json_str = "{\"text\": \"百度是一家高科技公司\"}";
	sprintf(content_len,"Content-Length: %d\r\n\r\n",strlen(buf));
	//创建访问体
	char content[ASR_REQUEST_BUFSIZE];
	memset(content,0,ASR_REQUEST_BUFSIZE);
	sprintf(content,"%s%s%s%s%s",content_page,content_host,content_type,content_len,buf);
		
	printf("\n%s\n",content);
	//构建socket访问
	//添加host
	char host[ASR_SMALL_BUFSIZE];
	memset(host,0,ASR_SMALL_BUFSIZE);
	strcpy(host,host_data);
	//创建socket
	int soc_fd;
	struct sockaddr_in sock_addr;
	soc_fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == soc_fd)
    {
        printf("%s : create socket failed of client.\n",__func__);
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
	
	//连接服务器
	int connect_flag;
	connect_flag = connect(soc_fd,(struct sockaddr*)&sock_addr,(socklen_t)sizeof(sock_addr));
    if(-1 == connect_flag)
	{
        printf("%s : connect failed of client!\n",__func__);
		close(soc_fd);
        return -1;//socket连接失败,返回-3
    }
	
	//向服务器发送url请求的request
    int send_flag;
	send_flag = send(soc_fd,content,strlen(content),0);
    if(-1 == send_flag)
    {
        printf("%s : send request to server failed.\n",__func__);
		close(soc_fd);
        return -1;//向服务器发送url请求失败,返回-4
    }
	
	
	//客户端接收服务器的返回信息
	static char recv_buffer[ASR_RESP_BUFSIZE];
    memset(recv_buffer,0,ASR_RESP_BUFSIZE);
    int recv_flag;
	recv_flag = recv(soc_fd,recv_buffer,ASR_RESP_BUFSIZE,0);
    if(-1 == recv_flag)
    {
        printf("%s : recieve response form server failed!\n",__func__);
		close(soc_fd);
        return -1;//接收服务器返回失败
    }
	
	printf("\n\n%s\n\n",recv_buffer);
	/*
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
	*/
    close(soc_fd);
	return 0;
}
























