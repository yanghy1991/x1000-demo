#include "bd_tts.h"

/**
*功能:判断http返回是否正确
*参数:
*response:http响应的字符串
*
*/
int tts_get_http_result(char *response)  
{ 
    char *ptmp = NULL; 

	//not http
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

	//TTS转换出错，返回-3
	ptmp = (char*)strstr(response,"application/json");
	if(ptmp){  
		printf("%s : tts failed,but error msg return.\n",__func__);
        return 1;
    }
	//正确返回TTS语音数据流,返回-4
	ptmp = (char*)strstr(response,"audio/mp3");
	if(ptmp){
		printf("%s : tts success.\n",__func__);
        return 2;
    }
}  

/**
*功能:获取http响应头的长度
*参数:
*response:http响应的字符串
*
*/
int tts_parse_http_result(char *response) 
{
	char *ptmp = NULL; 
	ptmp = (char*)strstr(response,"\r\n\r\n");  
	if(!ptmp)
	{  
		printf("%s : get http request head length error.\n",__func__);  
		return -1;  
	}
	
	return (ptmp+4-response); 	
}

/**
*功能:解析json字符串,获取json字符串里的err_no的值
*参数:
*json_str:需要解析的json字符串
*返回:直接返回err_no的值
*/
int parse_json(char *json_str)
{
	cJSON * root = NULL;
    cJSON * item = NULL;
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	else
	{
		item = cJSON_GetObjectItem(root, "err_no");
		return atoi(cJSON_Print(item));
	}
}

/**
*功能http访问参数构建:
*必填参数:
*tex:合成的文本，使用UTF-8编码，请注意文本长度必须小于1024字节
*lan:语言选择,填写zh
*tok:开放平台获取到的开发者 access_token
*ctp:客户端类型选择，web端填写1
*cuid:用户唯一标识，用来区分用户，填写机器 MAC 地址或 IMEI 码，长度为60以内
*选填参数:
*spd:语速，取值0-9，默认为5中语速
*pit:音调，取值0-9，默认为5中语调
*vol:音量，取值0-9，默认为5中音量
*per:发音人选择, 0为女声，1为男声，3为情感合成-度逍遥，4为情感合成-度丫丫，默认为普通女声
*
*parameters:存储构建好的字符串
*
*/
int tts_create_parameters(char *tex_data,char *lan_data,char *tok_data,char *ctp_data,char *cuid_data,
						char *spd_data,char *pit_data,char *vol_data,char *per_data,char *parameters)
{
	if(tex_data == NULL || lan_data == NULL || tok_data == NULL || ctp_data == NULL || cuid_data == NULL || spd_data == NULL || pit_data == NULL || vol_data == NULL || per_data == NULL || parameters == NULL)
	{
		printf("%s : some parameter error.\n",__func__);
		return -1;
	}
	
	char *spd,*pit,*vol,*per;
	
	if(spd_data[0] >=48 && spd_data[0] <= 57)
		spd=spd_data;
	else
		spd="5";
	
	if(pit_data[0] >=48 && pit_data[0] <= 57)
		pit=pit_data;
	else
		pit="5";
	if(vol_data[0] >=48 && vol_data[0] <= 57)
		vol=vol_data;
	else
		vol="5";
	if(per_data[0] >=48 && per_data[0] <= 49)
		per=per_data;
	else
		per="0";
	/*
	printf("spd = %c\n",spd[0]);
	printf("pit = %c\n",pit[0]);
	printf("vol = %c\n",vol[0]);
	printf("per = %c\n",per[0]);
	*/
	strcat(parameters,"tex=");
	strcat(parameters,tex_data);
	strcat(parameters,"&lan=");
	strcat(parameters,lan_data);
	strcat(parameters,"&tok=");
	strcat(parameters,tok_data);
	strcat(parameters,"&ctp=");
	strcat(parameters,ctp_data);
	strcat(parameters,"&cuid=");
	strcat(parameters,cuid_data);
	strcat(parameters,"&spd=");
	strcat(parameters,spd);
	strcat(parameters,"&pit=");
	strcat(parameters,pit);
	strcat(parameters,"&vol=");
	strcat(parameters,vol);
	strcat(parameters,"&per=");
	strcat(parameters,per);
	
	return 0;
}

/**
*功能:语音合成http请求
*参数:
*mp3_path:返回的音频文件存储路径
*parameters:请求参数字符串
*err_no:存储错误码
*
*/
int tts_http_post(char *mp3_path,char *parameters,int *err_no)
{
	char host[TTS_SMALL_BUFSIZE];//存放网址
	struct hostent *host2ip = NULL;//网址解析成ip
	struct sockaddr_in socaddr;
	int socket_fd;//打开的socket连接
	
	//以二进制方式打开（创建）文件
	FILE *mp3_fp = fopen(mp3_path, "wb");  
    if(mp3_fp == NULL){
        printf("%s : Cannot open file, press any key to exit.\n",__func__);
        return -1;
    }
	
	//构建HTTP访问
	char content_page[] = "POST /text2audio HTTP/1.1\r\n";
	char content_host[] = "HOST: tsn.baidu.com\r\n";  
	char content_type[] = "Content-Type: application/json\r\n";
	char content_len[TTS_SMALL_BUFSIZE];
	//char *p = "tex=你叫什么名字&lan=zh&cuid=00:0c:29:fb:d7:52&ctp=1&per=1&tok=24.d8f5aaaf7956b48096ec85de760a62a1.2592000.1493435707.282335-9406072";
	memset(content_len,0,TTS_SMALL_BUFSIZE);
	sprintf(content_len,"Content-Length: %d\r\n\r\n",strlen(parameters));
	char content[TTS_REQ_BUFSIZE];
	sprintf(content,"%s%s%s%s%s",content_page,content_host,content_type,content_len,parameters);
	
	//socket创建和连接
	memset(host,0,TTS_SMALL_BUFSIZE);
	strcpy(host,"tsn.baidu.com");
	
	//创建socket
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == socket_fd)
    {
        printf("%s : create socket failed of client.\n",__func__);
        return -1;
    }
	
	//设置IP地址结构
	host2ip = gethostbyname(host);//将上面获得的主机信息通过域名解析函数获得域>名信息
	
	bzero(&socaddr,sizeof(struct sockaddr_in));
	socaddr.sin_family = AF_INET;
	socaddr.sin_addr.s_addr = *((unsigned long*)host2ip->h_addr_list[0]);
	socaddr.sin_port = htons(DEFAULT_PORT);
	
	//连接服务器
	int connect_flag;
	connect_flag = connect(socket_fd,(struct sockaddr*)&socaddr,(socklen_t)sizeof(socaddr));
    if(-1 == connect_flag)
	{
        printf("%s : connect failed of client.\n",__func__);
        return -1;
    }
	
	//向服务器发送url请求的request
	int send_flag;
	send_flag = send(socket_fd,content,strlen(content),0);
    if(-1 == send_flag)
    {
        printf("%s : send failed of client.\n",__func__);
        return -1;
    }
		
	//接收服务器的返回信息
	static char response[TTS_RECV_BUFSIZE];
    memset(response,0,TTS_RECV_BUFSIZE); 
	char *tts_json = NULL;
	int nCount = 0;
	int len = 0;
	int response_flg = 0;
	int http_result_flag = 0;//只对返回的第一帧数据进行http类型的检测
    while((nCount = recv(socket_fd, response, TTS_RECV_BUFSIZE, 0)) > 0)
	{
		if(http_result_flag == 0)
		{
			response_flg = tts_get_http_result(response);
			if(response_flg == -1)
			{
				printf("%s : get http result error.",__func__);
				fclose(mp3_fp);
				close(socket_fd);
				return -1;
			}
			else if(response_flg == 1)
			{
				len = tts_parse_http_result(response);
				tts_json = (char *)(response+len);
				//printf("%s\n",tts_json);
				//解析jison
				int baidu_err_nu = parse_json(tts_json);
				if(baidu_err_nu == 500)
					printf("bai du error no is %d :不支持输入\n",baidu_err_nu);
				else if(baidu_err_nu == 501)
					printf("bai du error no is %d :输入参数不正确\n",baidu_err_nu);
				else if(baidu_err_nu == 502)
					printf("bai du error no is %d :token验证失败\n",baidu_err_nu);
				else if(baidu_err_nu == 503)
					printf("bai du error no is %d :合成后端错误\n",baidu_err_nu);
				else
					printf("x error , 未知错误\n");
				
				fclose(mp3_fp);
				close(socket_fd);
				*err_no = baidu_err_nu;
				return -1;//返回百度错误码
			}
			else if(response_flg == 2)
			{
				char *tmp = strstr(response,"\r\n\r\n"); 
				
				//if(1 > fwrite((tmp+4), nCount, 1, mp3_fp))
				if(1 > fwrite(response, nCount, 1, mp3_fp))
				{
					printf("%s : first write mp3 file error.\n",__func__);
					fclose(mp3_fp);
					close(socket_fd);
					return -1;//写入文件错误返回
				}				
				http_result_flag = 1;	
			}
		}
		else
		{
			if(http_result_flag == 1)
			{
				
				if(1 > fwrite(response, nCount, 1, mp3_fp))
				{
					printf("%s : write mp3 file error.\n",__func__);
					fclose(mp3_fp);
					close(socket_fd);
					return -1;//写入文件错误返回
				}
			}
		}       		
    }
	fclose(mp3_fp);
	close(socket_fd);
	return 0;
}


	
































