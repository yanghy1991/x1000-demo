#include "bd_weather.h"

/**
*功能:判断http返回是否正确
*参数:
*response:http响应的字符串
*
*/
int bd_weather_get_http_result(char *response)  
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
*error:用来存储error错误码
*status:用来存储请求天气的状态标志
*date:用来存储天气预报返回日期
*weather1:用来存储今天天气预报内容
*weather2:用来存储明天天气预报内容
*weather3:用来存储后天天气预报内容
*
*/
int bd_weater_json_parse(char *weater_json_str,int *error,char *status,char *date,char *weather1,char *show_weather1,char *weather2,char *show_weather2,char *weather3,char *show_weather3)
{
	//根级
	cJSON *root;
	//第一级
	cJSON *_error,*_status,*_date,*_results;
	//第二级
	cJSON *currentCity,*pm25,*weather_data;
	//第三级
	cJSON *__weather,*__wind,*__temperature;
	
	//
	cJSON *results_arrar_item0,*result_item;
	
	//
	cJSON *weather_item,*weather_sub_item;

	//生成根对象
	root = cJSON_Parse(weater_json_str);
	if (!root) 
    {
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	//从root中获取错误代码
	_error = cJSON_GetObjectItem(root, "error");
	if(NULL == _error)
	{
		printf("%s : get _error failed!\n",__func__);
		return -1;
	}
	*error = _error->valueint;
	//从root中获取状态
	_status = cJSON_GetObjectItem(root, "status");
	if(NULL == _status)
	{
		printf("%s : get _status failed!\n",__func__);
		return -1;
	}
	char *tmpp = cJSON_Print(_status);
	tmpp[strlen(tmpp)-1] = 0;
	strcpy(status,tmpp+1);
				
	//从root中获取日期
	_date = cJSON_GetObjectItem(root, "date");
	if(NULL == _date)
	{
		printf("%s : get _date failed!\n",__func__);
		return -1;
	}
	tmpp = cJSON_Print(_date);
	tmpp[strlen(tmpp)-1] = 0;
	strcpy(date,tmpp+1);
	free(tmpp);
	
	//从root中获取结果
	_results = cJSON_GetObjectItem(root, "results");
	if(NULL == _results)
	{
		printf("%s : get _results failed!\n",__func__);
		return -1;
	}
	//results是数组，查看里面有多少元素
	int results_arrar_size = -1;
	results_arrar_size = cJSON_GetArraySize(_results);
	printf("results_arrar_size = %d \n",results_arrar_size);
	if(results_arrar_size > 0)
	{
		//获取results数组里的第一个元素
		results_arrar_item0 = cJSON_GetArrayItem(_results,0);
		//把第一个元素转成字符串
		char *item0_str = cJSON_Print(results_arrar_item0);
		//通过字符串生成cJSON对象
		result_item = cJSON_Parse(item0_str);
		
		//获取元素
		currentCity = cJSON_GetObjectItem(result_item, "currentCity");
		if(NULL == currentCity)
		{
			printf("%s : get currentCity failed!\n",__func__);
			return -1;
		}
		pm25 = cJSON_GetObjectItem(result_item, "pm25");
		if(NULL == pm25)
		{
			printf("%s : get pm25 failed!\n",__func__);
			return -1;
		}
		weather_data = cJSON_GetObjectItem(result_item, "weather_data");
		if(NULL == weather_data)
		{
			printf("%s : get weather_data failed!\n",__func__);
			return -1;
		}
		
		//weather_data是数组
		int weather_data_arrar_size = -1;
		weather_data_arrar_size = cJSON_GetArraySize(weather_data);
		printf("weather_data_arrar_size = %d\n",weather_data_arrar_size);
		
		int i = 0;
		char *weather_item_str;
		for(i=0;i<weather_data_arrar_size;i++)
		{
			weather_item = cJSON_GetArrayItem(weather_data,i);
			if(weather_item)
			{
				weather_item_str = cJSON_Print(weather_item);
				weather_sub_item = cJSON_Parse(weather_item_str);
				if(NULL == weather_sub_item)
				{
					printf("%s : get weather_sub_item failed!\n",__func__);
					return -1;
				}
				
				__weather = cJSON_GetObjectItem(weather_sub_item,"weather");
				if(NULL == __weather)
				{
					printf("%s : get __weather failed!\n",__func__);
					return -1;
				}
				__wind = cJSON_GetObjectItem(weather_sub_item,"wind");
				if(NULL == __wind)
				{
					printf("%s : get __wind failed!\n",__func__);
					return -1;
				}
				__temperature = cJSON_GetObjectItem(weather_sub_item,"temperature");
				if(NULL == __temperature)
				{
					printf("%s : get __temperature failed!\n",__func__);
					return -1;
				}
				//今天天气预报
				if(i == 0)
				{
					char *tmp = NULL;
					/*
					tmp = cJSON_Print(currentCity);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather1,tmp+1);
					*/
					strcat(weather1,"今天PM2.5是");
					tmp = cJSON_Print(pm25);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather1,tmp+1);
					strcat(weather1,",");
					
					strcat(weather1,"今天");
					tmp = cJSON_Print(__weather);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather1,tmp+1);
					strcat(weather1,",");
					strcat(show_weather1,"0:");
					strcat(show_weather1,tmp+1);
					
					
					
					tmp = cJSON_Print(__wind);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather1,tmp+1);
					strcat(weather1,",");
					
					strcat(weather1,"气温是");
					tmp = cJSON_Print(__temperature);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather1,tmp+1);
					strcat(weather1,".");
					strcat(show_weather1,":");
					strcat(show_weather1,tmp+1);
					free(tmp);
				}
				//明天天气预报
				if(i == 1)
				{
					char *tmp = NULL;
					/*
					tmp = cJSON_Print(currentCity);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather2,tmp+1);
					*/
					
					strcat(weather2,"明天");
					tmp = cJSON_Print(__weather);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather2,tmp+1);
					strcat(weather2,",");
					strcat(show_weather2,"1:");
					strcat(show_weather2,tmp+1);
					
					tmp = cJSON_Print(__wind);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather2,tmp+1);
					strcat(weather2,",");
					
					strcat(weather2,"气温是");
					tmp = cJSON_Print(__temperature);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather2,tmp+1);
					strcat(weather2,".");
					strcat(show_weather2,":");
					strcat(show_weather2,tmp+1);
					free(tmp);					
				}	
				//后天天气预报
				if(i == 2)
				{
					char *tmp = NULL;
					/*
					tmp = cJSON_Print(currentCity);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather3,tmp+1);
					*/
					
					strcat(weather3,"后天");
					tmp = cJSON_Print(__weather);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather3,tmp+1);
					strcat(weather3,",");
					strcat(show_weather3,"2:");
					strcat(show_weather3,tmp+1);
					
					tmp = cJSON_Print(__wind);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather3,tmp+1);
					strcat(weather3,",");
					
					strcat(weather3,"气温是");
					tmp = cJSON_Print(__temperature);
					tmp[strlen(tmp)-1] = 0;
					strcat(weather3,tmp+1);
					strcat(weather3,".");
					strcat(show_weather3,":");
					strcat(show_weather3,tmp+1);
					free(tmp);					
				}
			}			
		}
	}
	else
	{
		printf("%s : no weather data.\n",__func__);
		return -1;
	}	
	
	return 0;
}

/**
*功能:http访问请求构建
*url_data:请求url,例如api.map.baidu.com/telematics/v3/weather?
*location_data:城市名或拼音
*output_data:返回信息格式设置json或xml
*ak_data:百度应用的ak
*request_content:存储构建好的请求字符串
*
*/
int bd_weater_create_parameters(char *url_data,char *location_data,char *output_data,char *ak_data,char *request_content)
{
	if(url_data == NULL || location_data == NULL || output_data == NULL || ak_data == NULL || request_content == NULL)
	{
		printf("%s : some parameter error.\n",__func__);
		return -1;
	}
	
	strcat(request_content,url_data);
	strcat(request_content,"location=");
	strcat(request_content,location_data);
	strcat(request_content,"&output=");
	strcat(request_content,output_data);
	strcat(request_content,"&ak=");
	strcat(request_content,ak_data);
	
	return 0;
}

/**
*功能:从百度获取天气预报信息
*url:请求url
*error:用来存储error错误码
*status:用来存储请求天气的状态标志
*date:用来存储天气预报返回日期
*weather1:用来存储今天天气预报内容
*weather2:用来存储明天天气预报内容
*weather3:用来存储后天天气预报内容
*
*/
int get_bd_weather(char* url,int *error,char *status,char *date,char *weather1,char *show_weather1,char *weather2,char *show_weather2,char *weather3,char *show_weather3)
{
    int socket_fd;//socket打开文件符
    struct sockaddr_in socaddr;
    struct hostent *host2ip = NULL;

    //分离主机中的主机地址和相对路径
	
	char *pHost = 0;
	char url_tmp[BDWEA_COM_BUFSIZE];
    memset(url_tmp,0,BDWEA_COM_BUFSIZE);
	strcpy(url_tmp,url);
	//pHost获取相对路径首地址
	for(pHost = url_tmp;*pHost != '/' && *pHost != '\0';++pHost);	
	
	char host[BDWEA_SMALL_BUFSIZE];
	char GET[BDWEA_COM_BUFSIZE];
	//获取相对路径保存到GET中
	memset(GET,0,BDWEA_COM_BUFSIZE);
    if((int)(pHost-url_tmp) == strlen(url_tmp))
		strcpy(GET,"/");//即url中没有给出相对路径，需要自己手动的在url尾部加上/
    else
        strcpy(GET,pHost);//地址段pHost到strlen(url_tmp)保存的是相对路径
	
	//将主机信息保存到host中	
    memset(host,0,BDWEA_SMALL_BUFSIZE);
    
    //此处将它置零，即它所指向的内容里面已经分离出了相对路径，剩下的为host信息
    *pHost = '\0';
    strcpy(host,url_tmp);

	//创建URL request信息
	char request[BDWEA_COM_BUFSIZE];
    memset(request,0,BDWEA_COM_BUFSIZE);
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
	static char response[BDWEA_RESP_BUFSIZE];
    memset(response,0,BDWEA_RESP_BUFSIZE);
    int nCount;
	
	char bd_tmp[BDWEA_READ_BUFSUZE];
	memset(bd_tmp,0,BDWEA_READ_BUFSUZE);
	while((nCount = recv(socket_fd, bd_tmp, (BDWEA_READ_BUFSUZE-1), 0)) > 0)
	{
		//printf("\n%s\n",bd_tmp);
		strcat(response,bd_tmp);
		memset(bd_tmp,0,BDWEA_READ_BUFSUZE);
	}
	
	
	//nCount = recv(socket_fd,response,BDWEA_RESP_BUFSIZE,0);
	nCount = strlen(response);
	
    if(nCount < BDWEA_READ_BUFSUZE)
    {
        printf("%s : recieve failed!\n",__func__);
        return -1;//服务器信息接收失败
    }
    else
    {
		printf("\n%s \n",response);
		int response_flg = 0;
		response_flg = bd_weather_get_http_result(response);
		
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
			
			int parse_flag = bd_weater_json_parse(weather_json_str,error,status,date,weather1,show_weather1,weather2,show_weather2,weather3,show_weather3);
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

































