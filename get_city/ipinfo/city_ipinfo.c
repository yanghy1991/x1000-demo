﻿#include "city_ipinfo.h"

/**
*功能:判断http返回是否正确
*参数:
*response:http响应的字符串
*
*/
int address_get_http_result(char *response)  
{ 
    char *ptmp = NULL; 

	//不是
    ptmp = (char*)strstr(response,"HTTP/1.1");  
    if(!ptmp){  
        printf("%s : http/1.1 not find.\n",__func__);  
        return -1;    
    }
	//HTTP访问出错
    if(atoi(ptmp + 9)!=200){  
        printf("%s : http response data error.\n",__func__);
        return -1;  
    }

	//没有json
	ptmp = (char*)strstr(response,"application/json");
	if(!ptmp)
	{
		printf("%s : no json return.\n",__func__);
		return -1; 
	}
	
	return 0;
} 

/**
*功能:对http响应的json字符串进行解析,解析出参数city
*参数:
*json_str:json字符串
*city:用来存储city
*
*/
int city_parse_json(char *json_str,char *city)
{
	//printf("%s\n",json_str);
	cJSON * root = NULL;
    cJSON * item = NULL;
	root = cJSON_Parse(json_str);
	if (!root) 
    {
		printf("%s : create json object return.\n",__func__);
		return -1;
    }
	else
	{
		item = cJSON_GetObjectItem(root, "city");
		char *tmp = cJSON_Print(item);
		tmp[strlen(tmp)-1] = 0;
		strcpy(city,tmp+1);
		free(tmp);
		return 0;
	}
}

/**
*功能:发起http请求，获取响应数据
*参数:
*url:http请求地址
*city:用来存储city
*
*/
int ipinfo_get_city(char* url,char *city)
{
    int socket_fd;//socket打开文件符
    struct sockaddr_in socaddr;
    struct hostent *host2ip = NULL;

    //分离主机中的主机地址和相对路径
	
	char *pHost = 0;
	char url_tmp[IPCITY_COM_BUFSIZE];
    memset(url_tmp,0,IPCITY_COM_BUFSIZE);
	strcpy(url_tmp,url);
	//pHost获取相对路径首地址
	for(pHost = url_tmp;*pHost != '/' && *pHost != '\0';++pHost);	
	
	char host[IPCITY_COM_BUFSIZE];
	char GET[IPCITY_COM_BUFSIZE];
	//获取相对路径保存到GET中
	memset(GET,0,IPCITY_COM_BUFSIZE);
    if((int)(pHost-url_tmp) == strlen(url_tmp))
		strcpy(GET,"/");//即url中没有给出相对路径，需要自己手动的在url尾部加上/
    else
        strcpy(GET,pHost);//地址段pHost到strlen(url_tmp)保存的是相对路径
	
	//将主机信息保存到host中	
    memset(host,0,IPCITY_COM_BUFSIZE);
    
    //此处将它置零，即它所指向的内容里面已经分离出了相对路径，剩下的为host信息
    *pHost = '\0';
    strcpy(host,url_tmp);

	//创建URL request信息
	char request[IPCITY_COM_BUFSIZE];
    memset(request,0,IPCITY_COM_BUFSIZE);
    strcat(request,"GET ");
    strcat(request,GET);
    strcat(request," HTTP/1.1\r\n");//至此为http请求行的信息
    strcat(request,"HOST: ");
    strcat(request,host);
    strcat(request,"\r\n");
    strcat(request,"Cache-Control: no-cache\r\n\r\n");
	
    //创建socket连接
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == socket_fd)
    {
        printf("%s : create socket failed of client!\n",__func__);
        return -1;//socket创建失败
    }
 
    host2ip = gethostbyname(host);//将上面获得的主机信息通过域名解析函数获得域>名信息
 
    //设置IP地址结构
    bzero(&socaddr,sizeof(struct sockaddr_in));
    socaddr.sin_family = AF_INET;
    socaddr.sin_addr.s_addr = *((unsigned long*)host2ip->h_addr_list[0]);
    socaddr.sin_port = htons(DEFAULT_PORT);
		
    //连接服务器
	int connect_flag;
	connect_flag = connect(socket_fd,(struct sockaddr*)&socaddr,(socklen_t)sizeof(socaddr));
    if(-1 == connect_flag)
	{
        printf("%s : connect failed of client!\n",__func__);
        return -1;//socket连接失败
    }
 
    //向服务器发送url请求的request
    int send_flag;
	send_flag = send(socket_fd,request,strlen(request),0);
    if(-1 == send_flag)
    {
        printf("%s : send failed of client!\n",__func__);
        return -1;//向服务器发送失败
    }
 
    //客户端接收服务器的返回信息
	static char response[IPCITY_RESP_BUFSIZE];
    memset(response,0,IPCITY_RESP_BUFSIZE);
    int nCount;
	nCount = recv(socket_fd,response,IPCITY_RESP_BUFSIZE,0);
    if(-1 == nCount)
    {
        printf("%s : recieve failed!\n",__func__);
        return -1;//服务器信息接收失败
    }
    else
    {
		//printf("\n%s \n",response);
		int response_flg = 0;
		response_flg = address_get_http_result(response);
		
		if(response_flg != 0)
		{
			printf("%s : http response failed !\n",__func__);
			return -1;//服务器返回失败
		}
		else
		{
			char *token_json = NULL;
			token_json = strstr(response,"{");
			int re_num = city_parse_json(token_json,city);
			if(re_num == -1)
			{
				printf("%s : there is no city \n",__func__);
				return -1;//获取city失败
			}
		}
		
    }
	
    close(socket_fd);
	return 0;
}











