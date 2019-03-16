#include "szhy_content.h"
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
	//ptmp = (char*)strstr(response,"application/json");
	ptmp = (char*)strstr(response,"text/html");
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
*rspMsg:用来存储rspMsg
*
*/
int rspmsg_parse_json(char *json_str,char *rspMsg,char *rspType)
{
	printf("json_str = %s\n",json_str);
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
		item = cJSON_GetObjectItem(root, "rspMsg");
		char *tmp = cJSON_Print(item);
		tmp[strlen(tmp)-1] = 0;
		strcpy(rspMsg,tmp+1);
		free(tmp);
		item = cJSON_GetObjectItem(root, "rspType");
		char *tmp1 = cJSON_Print(item);
		tmp1[strlen(tmp1)-1] = 0;
		strcpy(rspType,tmp1+1);
		free(tmp1);
		return 0;
	}
}

/**
*功能:发起http请求，获取响应数据
*参数:
*url:http请求地址
*rspMsg:用来存储rspMsg
*
*/
int get_content_server(char* url,char *rsp)
{
    int socket_fd;//socket打开文件符
    struct sockaddr_in socaddr;
    struct hostent *host2ip = NULL;

    //分离主机中的主机地址和相对路径
	char *pHost = 0;
	char url_tmp[HTTP_COM_BUFSIZE];
    memset(url_tmp,0,HTTP_COM_BUFSIZE);
	strcpy(url_tmp,url);
	//pHost获取相对路径首地址
	for(pHost = url_tmp;*pHost != '/' && *pHost != '\0';++pHost);	
	
	char host[HTTP_COM_BUFSIZE];
	char GET[HTTP_COM_BUFSIZE];
	//获取相对路径保存到GET中
	memset(GET,0,HTTP_COM_BUFSIZE);
    if((int)(pHost-url_tmp) == strlen(url_tmp))
		strcpy(GET,"/");//即url中没有给出相对路径，需要自己手动的在url尾部加上/
    else
        strcpy(GET,pHost);//地址段pHost到strlen(url_tmp)保存的是相对路径
	
	//将主机信息保存到host中	
    memset(host,0,HTTP_COM_BUFSIZE);
    
    //此处将它置零，即它所指向的内容里面已经分离出了相对路径，剩下的为host信息
    *pHost = '\0';
    strcpy(host,url_tmp);

	//创建URL request信息
	char request[HTTP_COM_BUFSIZE];
    memset(request,0,HTTP_COM_BUFSIZE);
    strcat(request,"GET ");
    strcat(request,GET);
    strcat(request," HTTP/1.1\r\n");//至此为http请求行的信息
    strcat(request,"HOST: ");
    strcat(request,host);
    strcat(request,"\r\n");
    strcat(request,"Cache-Control: no-cache\r\n\r\n");
	
	//printf("request = %s\n",request);
	
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
	socaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	//socaddr.sin_addr.s_addr = inet_addr("47.107.25.1");
//	socaddr.sin_addr.s_addr = *((unsigned long*)host2ip->h_addr_list[0]);
    socaddr.sin_port = htons(SERVER_PORT);
	
	DEBUG("SERVER_IP:%s SERVER_PORT:%d \n",SERVER_IP,SERVER_PORT);
	
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
	if(do_szhy_connect(socket_fd, (struct sockaddr *)&socaddr, &addrlen) == false)
    {
		printf("%s : connect to server failed \n",__func__);
        close(socket_fd);
		return -1;
    }
 
 
	//4.向服务器发送url请求的request	
	int sendlen = do_szhy_send(socket_fd, request);
	if(sendlen < 0)
	{
		printf("%s : send to server failed \n",__func__);
        close(socket_fd);
		return -1;
	}

	//5.客户端接收服务器的返回信息
	static char response[CONTENT_RESP_BUFSIZE];
    memset(response,0,CONTENT_RESP_BUFSIZE);
	int recvlen = do_szhy_recv(socket_fd, response, CONTENT_RESP_BUFSIZE) ;
	if(recvlen < 0)
	{
		printf("%s : recv from server failed \n",__func__);
        close(socket_fd);
		return -1;
	}

	printf("[%s] [%d] response is : \n %s\n",__func__,__LINE__,response);
	
	int response_flg = 0;
	response_flg = address_get_http_result(response);
		
	if(response_flg != 0)
	{
		printf("%s : http response failed !\n",__func__);
		return -1;//服务器返回失败
	}
	else
	{
		char *json_start = NULL;
		char *json_end = NULL;
		json_start = strchr(response,'{');
		if(json_start != NULL)
		{
			json_end = strrchr(response,'}');
			if(json_end != NULL)
			{
				*(json_end+1) = '\0';
				strcpy(rsp,json_start);
			}
			else
			return -1;
		}
		else
			return -1;
		
	}
	
    close(socket_fd);
	return 0;
}