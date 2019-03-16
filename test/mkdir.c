#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int makeDir(char* path)
{
	printf("mmmmmmmmmmmmmmmmmm\n");
    int beginCmpPath;
    int endCmpPath;
    int fullPathLen;
    int pathLen;
    char currentPath[128] = {0};
    char fullPath[256] = {0};

	if(path == NULL)
	{
		printf("path is null\n");
		return -1;
	}
	
	if(pathLen = strlen(path) <= 0)
	{
		printf("path length is 0\n");
		return -1;
	}
	
    printf("path = %s\n", path);
    //���·��
    if('/' != path[0])
    {   
        //��ȡ��ǰ·��
        getcwd(currentPath, sizeof(currentPath));
        strcat(currentPath, "/");
        printf("currentPath = %s\n", currentPath);
        beginCmpPath = strlen(currentPath);
        strcat(currentPath, path);
        if(path[pathLen] != '/')
        {
            strcat(currentPath, "/");
        }
        endCmpPath = strlen(currentPath);
        
    }
    else
    {
        //����·��
        int pathLen = strlen(path);
        strcpy(currentPath, path);
        if(path[pathLen] != '/')
        {
            strcat(currentPath, "/");
        }
        beginCmpPath = 1;
        endCmpPath = strlen(currentPath);
    }
    //��������Ŀ¼
	int i = 0;
    for(i = beginCmpPath; i < endCmpPath ; i++ )
    {
        if('/' == currentPath[i])
        {
            currentPath[i] = '\0';
			
			if(opendir(currentPath) == NULL)
			{
				if(mkdir(currentPath, 0755) == -1)
                {
                    printf("currentPath = %s\n", currentPath);
                    perror("mkdir error %s\n");
                    return -1;
                }
			}
            currentPath[i] = '/';
        }
    }
    return 0;
}

int main()
{
	char *path = NULL; 
    makeDir(path);
    return 0;
}