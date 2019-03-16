#include "bd_token.h"

/**
*功能:判断http返回是否正确
*参数:
*response:http响应的字符串
*
*/
int token_get_http_result(char *response)  
{ 
    char *ptmp = NULL; 

	//不是http
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
	//没有json数据
	ptmp = (char*)strstr(response,"application/json");
	if(!ptmp)
	{		
		printf("%s : http response data error.\n",__func__);
        return -1;  
	}
	
	return 0;	
} 

/**
*功能:创建http访问字符串
*参数:
*url_data:绝对url,比如openapi.baidu.com/oauth/2.0/token?
*client_id_data:百度应用账号
*client_secret_data:百度应用安全账号
*request_content:存储http访问字符串
*
*/
int token_create_parameters(char *url_data,char *client_id_data,char *client_secret_data,char *request_content)
{
	if(url_data ==NULL || client_id_data==NULL || client_secret_data==NULL || request_content==NULL)
	{
		printf("%s : parameters error.\n",__func__);
		return -1;
	}
	strcat(request_content,url_data);
	strcat(request_content,"grant_type=client_credentials");
	strcat(request_content,"&client_id=");
	strcat(request_content,client_id_data);
	strcat(request_content,"&client_secret=");
	strcat(request_content,client_secret_data);
	
	return 0;
}

/**
*功能:对http响应的json字符串进行解析,解析出参数access_token
*参数:
*json_str:json字符串
*access_token_data:用来存储access_token
*
*/
int token_parse_json(char *json_str,char *access_token_data)
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
		item = cJSON_GetObjectItem(root, "access_token");
		char *tmp = cJSON_Print(item);
		tmp[strlen(tmp)-1] = 0;
		strcpy(access_token_data,tmp+1);		
		return 0;
	}
}

/**
*功能:请求http获取access_token
*参数:
*url:http请求字符串
*access_token_data:用来存储access_token
*
*/
int bd_get_token(char* url,char *access_token)
{
    int socket_fd;//socket打开文件符
    struct sockaddr_in socaddr;
    struct hostent *host2ip = NULL;

    //分离主机中的主机地址和相对路径
	
	char *pHost = 0;
	char url_tmp[TOKEN_COMMON_BUFSIZE];
    memset(url_tmp,0,TOKEN_COMMON_BUFSIZE);
	strcpy(url_tmp,url);
	//pHost获取相对路径首地址
	for(pHost = url_tmp;*pHost != '/' && *pHost != '\0';++pHost);	
	
	char host[TOKEN_COMMON_BUFSIZE];
	char GET[TOKEN_COMMON_BUFSIZE];
	//获取相对路径保存到GET中
	memset(GET,0,TOKEN_COMMON_BUFSIZE);
    if((int)(pHost-url_tmp) == strlen(url_tmp))
		strcpy(GET,"/");//即url中没有给出相对路径，需要自己手动的在url尾部加上/
    else
        strcpy(GET,pHost);//地址段pHost到strlen(url_tmp)保存的是相对路径
	
	//将主机信息保存到host中	
    memset(host,0,TOKEN_COMMON_BUFSIZE);
    
    //此处将它置零，即它所指向的内容里面已经分离出了相对路径，剩下的为host信息
    *pHost = '\0';
    strcpy(host,url_tmp);

	//创建URL request信息
	char request[TOKEN_COMMON_BUFSIZE];
    memset(request,0,TOKEN_COMMON_BUFSIZE);
    strcat(request,"GET ");
    strcat(request,GET);
    strcat(request," HTTP/1.1\r\n");//至此为http请求行的信息
    strcat(request,"HOST: ");
    strcat(request,host);
    strcat(request,"\r\n");
    strcat(request,"Cache-Control: no-cache\r\n\r\n");
	
    //1.创建socket连接
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
	
	//2.把socket设置为非阻塞模式
    int fdflags = fcntl(socket_fd, F_GETFL, 0);
    if(fcntl(socket_fd, F_SETFL, fdflags | O_NONBLOCK) < 0)
    {
        printf("fcntl set nonblock failed \n");
		close(socket_fd);
        return -1;
    }
	
    //3.连接服务器
	int addrlen = sizeof(socaddr);
	if(do_connect(socket_fd, (struct sockaddr *)&socaddr, &addrlen) == false)
    {
		printf("%s : connect to server failed \n",__func__);
        close(socket_fd);
		return -1;
    }
 
    //4.向服务器发送url请求的request	
	int sendlen = do_send(socket_fd, request);
	if(sendlen < 0)
	{
		printf("%s : send to server failed \n",__func__);
        close(socket_fd);
		return -1;
	}
 
    //5.客户端接收服务器的返回信息
	static char response[TOKEN_RESP_BUFSIZE];
    memset(response,0,TOKEN_RESP_BUFSIZE);
	int recvlen = do_recv(socket_fd, response, TOKEN_RESP_BUFSIZE) ;
	if(recvlen < 0)
	{
		printf("%s : recv from server failed \n",__func__);
        close(socket_fd);
		return -1;
	}

	printf("response is : \n%s\n",response);
	int response_flg = 0;
	response_flg = token_get_http_result(response);		
	if(response_flg != 0)
	{
		printf("%s : http response failed !\n",__func__);
		return -1;//服务器返回失败
	}
	else
	{
		char *token_json = NULL;
		token_json = strstr(response,"{");
		int re_num = token_parse_json(token_json,access_token);
		if(re_num == -1)
		{
			printf("%s : there is no access_token \n",__func__);
			return -1;//获取acces_token失败
		}
	}
	
    close(socket_fd);
	return 0;
}




