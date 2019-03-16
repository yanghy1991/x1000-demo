#include "bureau_weather.h"

/**
*功能:判断http返回是否正确
*参数:
*response:http响应的字符串
*
*/
int bureau_weather_get_http_result(char *response)  
{ 
    char *ptmp = NULL; 

	//不说http,返回-1
    ptmp = (char*)strstr(response,"HTTP/1.1");  
    if(!ptmp){  
        printf("%s : http/1.1 not find.\n",__func__);  
        return -1;  
    }
	//HTTP访问出错,返回-2
    if(atoi(ptmp + 9)!=200){  
        printf("%s : http response no OK(200).\n",__func__);
        return -1;  
    }

	return 0;	
} 

/**
*功能:对http响应的json字符串进行解析,解析出参数error,status,date,weather1，weather2
*参数:
*weater_json_str:含有天气信息的json字符串
*weather:用来存储一小时天气预报内容
*show_weather:用来存储用来显示的天气预报内容
*
*/
int bureau_weater_json_parse(char *weater_json_str,char *weather,char *show_weather)
{
	//根级
	cJSON *root;
	//第一级
	cJSON *weatherinfo,*weatherinfo_obj;
	//第二级
	cJSON *temp,*WD,*WS;
	
	//生成根对象
	root = cJSON_Parse(weater_json_str);
	if (!root) 
    {
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	//从root中获取weatherinfo
	weatherinfo = cJSON_GetObjectItem(root, "weatherinfo");
	if(NULL == weatherinfo)
	{
		printf("%s : get weatherinfo failed!\n",__func__);
		return -1;
	}
	
	//把第一个元素转成字符串
	char *weatherinfo_str = cJSON_Print(weatherinfo);
	//通过字符串生成cJSON对象
	weatherinfo_obj = cJSON_Parse(weatherinfo_str);
	if (!weatherinfo_obj) 
    {
		printf("%s : parse weatherinfo_obj json error.\n",__func__);  
		return -1;
    }
	
	//获取温度
	temp = cJSON_GetObjectItem(weatherinfo_obj, "temp");
	if(NULL == temp)
	{
		printf("%s : get temp failed!\n",__func__);
		return -1;
	}
	//获取风向
	WD = cJSON_GetObjectItem(weatherinfo_obj, "WD");
	if(NULL == WD)
	{
		printf("%s : get WD failed!\n",__func__);
		return -1;
	}
	//获取风力
	WS = cJSON_GetObjectItem(weatherinfo_obj, "WS");
	if(NULL == WS)
	{
		printf("%s : get WS failed!\n",__func__);
		return -1;
	}
	
	char *tmp = NULL;
	//添加温度
	strcat(weather,"今天的温度是");
	tmp = cJSON_Print(temp);
	tmp[strlen(tmp)-1] = 0;
	strcat(weather,tmp+1);
	strcat(weather,"摄氏度,");
	strcat(show_weather,tmp+1);
	
	//添加风力
	strcat(weather,"有");
	tmp = cJSON_Print(WS);
	tmp[strlen(tmp)-1] = 0;
	strcat(weather,tmp+1);
	//添加风向
	tmp = cJSON_Print(WD);
	tmp[strlen(tmp)-1] = 0;
	strcat(weather,tmp+1);
	strcat(weather,".");

	return 0;
}

/**
*功能:http访问请求构建www.weather.com.cn/data/sk/101010100.html
*url_data:请求url,例如www.weather.com.cn/data/sk/
*city_code_data:中国气象城市名代码
*request_content:存储构建好的请求字符串
*
*/
int bureau_weater_create_parameters(char *url_data,char *city_code_data,char *request_content)
{
	if(url_data == NULL || city_code_data == NULL || request_content == NULL)
	{
		printf("%s : some parameter error.\n",__func__);
		return -1;
	}
	
	strcat(request_content,url_data);
	strcat(request_content,city_code_data);
	strcat(request_content,".html");
	
	return 0;
}

/**
*功能:从气象台获取天气预报信息
*url:请求url
*weather1:用来存储一小时天气预报内容
*show_weather:用来存储用来显示的天气预报内容
*
*/
int get_bureau_weather(char* url,char *weather,char *show_weather)
{
    int socket_fd;//socket打开文件符
    struct sockaddr_in socaddr;
    struct hostent *host2ip = NULL;

    //分离主机中的主机地址和相对路径
	
	char *pHost = 0;
	char url_tmp[BURWEA_COM_BUFSIZE];
    memset(url_tmp,0,BURWEA_COM_BUFSIZE);
	strcpy(url_tmp,url);
	//pHost获取相对路径首地址
	for(pHost = url_tmp;*pHost != '/' && *pHost != '\0';++pHost);	
	
	char host[BURWEA_SMALL_BUFSIZE];
	char GET[BURWEA_COM_BUFSIZE];
	//获取相对路径保存到GET中
	memset(GET,0,BURWEA_COM_BUFSIZE);
    if((int)(pHost-url_tmp) == strlen(url_tmp))
		strcpy(GET,"/");//即url中没有给出相对路径，需要自己手动的在url尾部加上/
    else
        strcpy(GET,pHost);//地址段pHost到strlen(url_tmp)保存的是相对路径
	
	//将主机信息保存到host中	
    memset(host,0,BURWEA_SMALL_BUFSIZE);
    
    //此处将它置零，即它所指向的内容里面已经分离出了相对路径，剩下的为host信息
    *pHost = '\0';
    strcpy(host,url_tmp);

	//创建URL request信息
	char request[BURWEA_COM_BUFSIZE];
    memset(request,0,BURWEA_COM_BUFSIZE);
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
	static char response[BURWEA_RESP_BUFSIZE];
    memset(response,0,BURWEA_RESP_BUFSIZE);
    int nCount;
	nCount = recv(socket_fd,response,BURWEA_RESP_BUFSIZE,0);
    if(-1 == nCount)
    {
        printf("%s : recieve failed!\n",__func__);
        return -1;//服务器信息接收失败
    }
    else
    {
		printf("\n%s \n",response);
		int response_flg = 0;
		response_flg = bureau_weather_get_http_result(response);
		
		if(response_flg != 0)
		{
			printf("%s : http response failed !\n",__func__);
			return -1;//服务器返回失败
		}
		else
		{
			char *weather_json_str = NULL;
			weather_json_str = strstr(response,"{");
			//printf("weather_json_str = \n%s\n",weather_json_str);
			
			int parse_flag = bureau_weater_json_parse(weather_json_str,weather,show_weather);
			if(parse_flag == -1)
			{
				printf("%s : parse weather data failed \n",__func__);
				return -1;//获取city失败
			}
		}		
    }
	
    close(socket_fd);
	return 0;
}

































