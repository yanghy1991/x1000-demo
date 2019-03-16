#include "sina_weather.h"

/**
*功能:判断http返回是否正确
*参数:
*response:http响应的字符串
*
*/
int sina_weather_get_http_result(char *response)  
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

	return 0;	
}

/**
*功能:http访问请求构建
*url_data:请求url,例如php.weather.sina.com.cn/xml.php?
*city_data:城市名的GB2312码，例如,深圳%C9%EE%DB%DA
*password_data:DJOYnieT8234jlsK(固定值)
*day_data:请求哪天的天气预报,0(今天),1(明天),2(后天)
*request_content:存储构建好的请求字符串
*
*/
int sina_weather_create_parameters(char *url_data,char *city_data,char *password_data,char *day_data,char *request_content)
{
	if(url_data == NULL || city_data == NULL || password_data == NULL || day_data == NULL || request_content == NULL)
	{
		printf("%s : some parameter error.\n",__func__);
		return -1;
	}
	strcat(request_content,url_data);
	strcat(request_content,"city=");
	strcat(request_content,city_data);
	strcat(request_content,"&password=");
	strcat(request_content,password_data);
	strcat(request_content,"&day=");
	strcat(request_content,day_data);
	
	return 0;
}

/**
*功能:获取返回的xml中某一项的值
*items:xml字符串
*item_str:存储item中的值
*item_name_head:item的头,例如<status1>
*item_name_tail:item的尾,例如</status1>
*
*/
int get_item_info(char *items,char *item_str,char *item_name_head,char *item_name_tail)
{
	//复制items
	int items_len = strlen(items)+1;	
	char items_tmp[items_len];
	memset(items_tmp,0,items_len);
	strcpy(items_tmp,items);	
	//printf("items_tmp = %s\n",items_tmp);
	
	char *tmp = strstr(items_tmp,item_name_head);
	if(tmp == NULL)
	{
		printf("%s : get %s error!\n",__func__,item_name_head);
		return -1;
	}
	else
	{
		char *item_start = tmp+strlen(item_name_head);		
		char *item_end = strstr(items_tmp,item_name_tail);
		if(item_end == NULL)
		{
			printf("%s : get %s error!\n",__func__,item_name_tail);
			return -1;
		}
		else
		{
			*(items_tmp+(int)(item_end-items_tmp)) = '\0';
			//printf("item_start = %s\n",item_start);
			strcpy(item_str,item_start);
		}
	}	
	return 0;
}

/**
*功能:从返回的xml中分离所需要的天气信息
*xml_str:xml字符串
*weather:存储天气预报信息字符串
*show_weather:用来存储天气状态和温度
*day_data:请求哪天的天气预报,0(今天),1(明天),2(后天)
*
*/
int get_sina_weather_info(char *xml_str,char *weather,char *show_weather,char *day_data)
{
	char *tmp = NULL;
	
	tmp = (char *)strstr(xml_str,"</Profiles>");
	if(!tmp)
	{
		printf("%s : http response Incomplete information.\n",__func__);
		return -1;
	}
	else
	{
		if(day_data[0] ==48)
		{
			strcat(weather,"今天");
			strcat(show_weather,"0:");
		}else if(day_data[0] ==49)
		{
			strcat(weather,"明天");
			strcat(show_weather,"1:");
		}else if(day_data[0] ==50)
		{
			strcat(weather,"后天");
			strcat(show_weather,"2:");
		}
		else
		{
			printf("%s : the parameter only 0,1,2.\n",__func__);
			return -1;//预报天数的参数是0,1,2即是只能预报今天，明天和后天
		}
		char item_str[SINAWEA_ITEM_BUFSIZE];
		int item_reflag = 1;
		
		//<status1> </status1>
		memset(item_str,0,SINAWEA_ITEM_BUFSIZE);
		item_reflag = get_item_info(xml_str,item_str,"<status1>","</status1>");
		if(item_reflag != 0)
		{
			printf("%s : get status1 error.\n",__func__);
			return -1;
		}
		strcat(weather,item_str);
		strcat(show_weather,item_str);
		strcat(show_weather,":");
		
		//<power1> </power1>
		memset(item_str,0,SINAWEA_ITEM_BUFSIZE);
		item_reflag = get_item_info(xml_str,item_str,"<power1>","</power1>");
		if(item_reflag != 0)
		{
			printf("%s : get power1 error.\n",__func__);
			return -1;
		}
		strcat(weather,",");
		strcat(weather,item_str);
		
		//<direction1></direction1>
		memset(item_str,0,SINAWEA_ITEM_BUFSIZE);
		item_reflag = get_item_info(xml_str,item_str,"<direction1>","</direction1>");
		if(item_reflag != 0)
		{
			printf("%s : get direction1 error.\n",__func__);
			return -1;
		}
		strcat(weather,"级");
		strcat(weather,item_str);
		
		//<temperature2></temperature2>
		memset(item_str,0,SINAWEA_ITEM_BUFSIZE);
		item_reflag = get_item_info(xml_str,item_str,"<temperature2>","</temperature2>");
		if(item_reflag != 0)
		{
			printf("%s : get temperature1 error.\n",__func__);
			return -1;
		}
		strcat(weather,",气温");
		strcat(weather,item_str);
		strcat(show_weather,item_str);
		strcat(show_weather,"~");
		
		//<temperature1></temperature1>
		memset(item_str,0,SINAWEA_ITEM_BUFSIZE);
		item_reflag = get_item_info(xml_str,item_str,"<temperature1>","</temperature1>");
		if(item_reflag != 0)
		{
			printf("%s : get temperature1 error.\n",__func__);
			return -1;
		}
		strcat(weather,"~");
		strcat(weather,item_str);
		strcat(weather,"摄氏度");		
		strcat(show_weather,item_str);		
	
		//<zwx_s></zwx_s>
		memset(item_str,0,SINAWEA_ITEM_BUFSIZE);
		item_reflag = get_item_info(xml_str,item_str,"<zwx_s>","</zwx_s>");
		if(item_reflag != 0)
		{
			printf("%s : get zwx_s error\n",__func__);
			return -1;
		}
		strcat(weather,",");
		strcat(weather,item_str);
		strcat(weather,".");	
	}
	
	return 0;
}

/**
*功能:从新浪获取天气预报信息
*url:请求url
*weather:用来存储天气预报内容
*show_weather:用来存储天气状态和温度
*day_data:请求哪天的天气预报,0(今天),1(明天),2(后天)
*
*/
int get_sina_weather(char* url,char *weather,char *show_weather,char *day_data)
{
    int socket_fd;//socket打开文件符
    struct sockaddr_in socaddr;
    struct hostent *host2ip = NULL;

    //分离主机中的主机地址和相对路径
	
	char *pHost = 0;
	char url_tmp[SINAWEA_COM_BUFSIZE];
    memset(url_tmp,0,SINAWEA_COM_BUFSIZE);
	strcpy(url_tmp,url);
	//pHost获取相对路径首地址
	for(pHost = url_tmp;*pHost != '/' && *pHost != '\0';++pHost);	
	
	char host[SINAWEA_SMALL_BUFSIZE];
	char GET[SINAWEA_COM_BUFSIZE];
	//获取相对路径保存到GET中
	memset(GET,0,SINAWEA_COM_BUFSIZE);
    if((int)(pHost-url_tmp) == strlen(url_tmp))
		strcpy(GET,"/");//即url中没有给出相对路径，需要自己手动的在url尾部加上/
    else
        strcpy(GET,pHost);//地址段pHost到strlen(url_tmp)保存的是相对路径
	
	//将主机信息保存到host中	
    memset(host,0,SINAWEA_SMALL_BUFSIZE);
    
    //此处将它置零，即它所指向的内容里面已经分离出了相对路径，剩下的为host信息
    *pHost = '\0';
    strcpy(host,url_tmp);

	//创建URL request信息
	char request[SINAWEA_COM_BUFSIZE];
    memset(request,0,SINAWEA_COM_BUFSIZE);
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
        printf("%s : create socket failed of client.\n",__func__);
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
        printf("%s : connect failed of client.\n",__func__);
        return -1;//socket连接失败
    }
 
    //向服务器发送url请求的request
    int send_flag;
	send_flag = send(socket_fd,request,strlen(request),0);
    if(-1 == send_flag)
    {
        printf("%s : send failed of client.\n",__func__);
        return -1;//向服务器发送失败
    }
 
    //客户端接收服务器的返回信息
	char response[SINAWEA_RESP_BUFSIZE];
    memset(response,0,SINAWEA_RESP_BUFSIZE);
    int nCount;
	char sina_tmp[SINAWEA_READ_BUFSUZE];
	memset(sina_tmp,0,SINAWEA_READ_BUFSUZE);
	while((nCount = recv(socket_fd, sina_tmp, (SINAWEA_READ_BUFSUZE-1), 0)) > 0)
	{
		//printf("\n%s\n",sina_tmp);
		strcat(response,sina_tmp);
		memset(sina_tmp,0,SINAWEA_READ_BUFSUZE);
	}
	
	nCount = strlen(response);
    if(nCount < SINAWEA_READ_BUFSUZE)
    {
        printf("%s : recieve failed.\n",__func__);
        return -1;//服务器信息接收失败
    }
    else
    {
		//printf("\n nCount = %d \n",nCount);
		//printf("\n%s \n",response);
		int response_flg = 0;
		response_flg = sina_weather_get_http_result(response);		
		if(response_flg != 0)
		{
			printf("%s : http response failed.\n",__func__);
			return -1;//服务器返回失败
		}
		else
		{ 
			int weather_flag = get_sina_weather_info(response,weather,show_weather,day_data);
			if(weather_flag != 0)
			{
				printf("%s : get weater info error.\n",__func__);
				return -1;
			}
		}		
    }
	
    close(socket_fd);
	return 0;
}






















