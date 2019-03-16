#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

/**
* 目录是否存在
*/

int is_dir_exist(char *dir_path)
{
	if(dir_path == NULL)
	{
		printf("dir path is null\n");
		return -1;
	}
	if(opendir(dir_path) == NULL)
	{
		printf("dir path is not exist\n");
		return -1;
	}
	return 0;
}


/**
* 创建多级目录(支持相对路径和据对路径)
*/
int create_dir(char* path)
{
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
    //相对路径
    if('/' != path[0])
    {   
        //获取当前路径
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
        //绝对路径
        int pathLen = strlen(path);
        strcpy(currentPath, path);
        if(path[pathLen] != '/')
        {
            strcat(currentPath, "/");
        }
        beginCmpPath = 1;
        endCmpPath = strlen(currentPath);
    }
    //创建各级目录
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










