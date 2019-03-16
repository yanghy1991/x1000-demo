#include "city_sina.h"

/**
*����:�ж�http�����Ƿ���ȷ
*����:
*response:http��Ӧ���ַ���
*
*/
int address_get_http_result(char *response)  
{ 
    char *ptmp = NULL; 

	//����
    ptmp = (char*)strstr(response,"HTTP/1.1");  
    if(!ptmp){  
        printf("%s : http/1.1 not find.\n",__func__);  
        return -1;    
    }
	//HTTP���ʳ���
    if(atoi(ptmp + 9)!=200){  
        printf("%s : http response data error.\n",__func__);
        return -1;  
    }

	//û��json
	ptmp = (char*)strstr(response,"application/json");
	if(!ptmp)
	{
		printf("%s : no json return.\n",__func__);
		return -1; 
	}
	
	return 0;
} 

/**
*����:��http��Ӧ��json�ַ������н���,����������city
*����:
*json_str:json�ַ���
*city:�����洢city
*
*/
int city_parse_json(char *json_str,char *city)
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
		item = cJSON_GetObjectItem(root, "city");
		char *tmp = cJSON_Print(item);
		tmp[strlen(tmp)-1] = 0;
		strcpy(city,tmp+1);
		free(tmp);
		return 0;
	}
}

/**
*����:����http���󣬻�ȡ��Ӧ����
*����:
*url:http�����ַ
*city:�����洢city
*
*/
int sina_get_city(char* url,char *city)
{
    int socket_fd;//socket���ļ���
    struct sockaddr_in socaddr;
    struct hostent *host2ip = NULL;

    //���������е�������ַ�����·��
	
	char *pHost = 0;
	char url_tmp[SINACITY_COM_BUFSIZE];
    memset(url_tmp,0,SINACITY_COM_BUFSIZE);
	strcpy(url_tmp,url);
	//pHost��ȡ���·���׵�ַ
	for(pHost = url_tmp;*pHost != '/' && *pHost != '\0';++pHost);	
	
	char host[SINACITY_COM_BUFSIZE];
	char GET[SINACITY_COM_BUFSIZE];
	//��ȡ���·�����浽GET��
	memset(GET,0,SINACITY_COM_BUFSIZE);
    if((int)(pHost-url_tmp) == strlen(url_tmp))
		strcpy(GET,"/");//��url��û�и������·������Ҫ�Լ��ֶ�����urlβ������/
    else
        strcpy(GET,pHost);//��ַ��pHost��strlen(url_tmp)����������·��
	
	//��������Ϣ���浽host��	
    memset(host,0,SINACITY_COM_BUFSIZE);
    
    //�˴��������㣬������ָ������������Ѿ�����������·����ʣ�µ�Ϊhost��Ϣ
    *pHost = '\0';
    strcpy(host,url_tmp);

	//����URL request��Ϣ
	char request[SINACITY_COM_BUFSIZE];
    memset(request,0,SINACITY_COM_BUFSIZE);
    strcat(request,"GET ");
    strcat(request,GET);
    strcat(request," HTTP/1.1\r\n");//����Ϊhttp�����е���Ϣ
    strcat(request,"HOST: ");
    strcat(request,host);
    strcat(request,"\r\n");
    strcat(request,"Cache-Control: no-cache\r\n\r\n");
	
    //����socket����
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == socket_fd)
    {
        printf("%s : create socket failed of client!\n",__func__);
        return -1;//socket����ʧ��
    }
 
    host2ip = gethostbyname(host);//�������õ�������Ϣͨ�������������������>����Ϣ
 
    //����IP��ַ�ṹ
    bzero(&socaddr,sizeof(struct sockaddr_in));
    socaddr.sin_family = AF_INET;
    socaddr.sin_addr.s_addr = *((unsigned long*)host2ip->h_addr_list[0]);
    socaddr.sin_port = htons(DEFAULT_PORT);
		
    //���ӷ�����
	int connect_flag;
	connect_flag = connect(socket_fd,(struct sockaddr*)&socaddr,(socklen_t)sizeof(socaddr));
    if(-1 == connect_flag)
	{
        printf("%s : connect failed of client!\n",__func__);
        return -1;//socket����ʧ��
    }
 
    //�����������url�����request
    int send_flag;
	send_flag = send(socket_fd,request,strlen(request),0);
    if(-1 == send_flag)
    {
        printf("%s : send failed of client!\n",__func__);
        return -1;//�����������ʧ��
    }
 
    //�ͻ��˽��շ������ķ�����Ϣ
	static char response[SINACITY_RESP_BUFSIZE];
    memset(response,0,SINACITY_RESP_BUFSIZE);
    int nCount;
	nCount = recv(socket_fd,response,SINACITY_RESP_BUFSIZE,0);
    if(-1 == nCount)
    {
        printf("%s : recieve failed!\n",__func__);
        return -1;//��������Ϣ����ʧ��
    }
    else
    {
		//printf("\n%s \n",response);
		int response_flg = 0;
		response_flg = address_get_http_result(response);
		
		if(response_flg != 0)
		{
			printf("%s : http response failed !\n",__func__);
			return -1;//����������ʧ��
		}
		else
		{
			char *token_json = NULL;
			token_json = strstr(response,"{");
			int re_num = city_parse_json(token_json,city);
			if(re_num == -1)
			{
				printf("%s : there is no city \n",__func__);
				return -1;//��ȡcityʧ��
			}
		}
		
    }
	
    close(socket_fd);
	return 0;
}











