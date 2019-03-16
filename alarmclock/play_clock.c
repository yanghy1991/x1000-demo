#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h> 
#include <signal.h>

#define MAX_PID_NUM 32
pthread_t clock_ptread_id;

//闹钟播放线程
void * play_mp3_ptread_func(void *arg)
{
	system("mpg123 alarm.mp3");
}


char *basename(const char *path)
{
	register const char *s;
	register const char *p;

	p = s = path;

	while (*s)
	{
		if (*s++ == '/')
		{
			p = s;
		}
	}

	return (char *) p;
}

/* 根据进程名称获取PID, 比较 base name of pid_name
 * pid_list: 获取PID列表
 * list_size: 获取PID列表长度
 * RETURN值说明:
 *              < 0:
 *              >=0: 发现多少PID, pid_list 将保存已发现的PID
 */
int get_pid_by_name(const char* process_name, pid_t pid_list[], int list_size)
{
#define  MAX_BUF_SIZE       256

	DIR *dir;
	struct dirent *next;
	int count=0;
	pid_t pid;
	FILE *fp;
	char *base_pname = NULL;
	char *base_fname = NULL;
	char cmdline[MAX_BUF_SIZE];
	char path[MAX_BUF_SIZE];

	if(process_name == NULL || pid_list == NULL)
		return -EINVAL;

	base_pname = basename(process_name);
	if(strlen(base_pname) <= 0)
		return -EINVAL;

	dir = opendir("/proc");
	if (!dir)
	{
		return -EIO;
	}
	while ((next = readdir(dir)) != NULL)
	{
		/* skip non-number */
		if (!isdigit(*next->d_name))
			continue;

		pid = strtol(next->d_name, NULL, 0);
		sprintf(path, "/proc/%u/cmdline", pid);
		fp = fopen(path, "r");
		if(fp == NULL)
			continue;

		memset(cmdline, 0, sizeof(cmdline));
		if(fread(cmdline, MAX_BUF_SIZE - 1, 1, fp) < 0)
		{
			fclose(fp);
			continue;
		}
		fclose(fp);
		base_fname = basename(cmdline);

		if (strcmp(base_fname, base_pname) == 0 )
		{
			if(count >= list_size)
			{
				break;
			}
			else
			{
				pid_list[count] = pid;
				count++;
			}
		}
	}
	closedir(dir) ;
	return count;
}

int main(int argc,char **argv)
{
	
	int isr_paly_music_err;
	int i = 0;
	pid_t pid[MAX_PID_NUM];
	int rec = 0;
	char* process;
	int flag = -1;
	
	for(i=0;i<5;i++)
	{
		isr_paly_music_err = pthread_create(&(clock_ptread_id),NULL,play_mp3_ptread_func,NULL);
		if(isr_paly_music_err != 0)
		{
			printf("create thread clock_ptread_id fail ...\n");
			return -1;
		}
	
		int j = 0;
		for(j=0;j<300;j++)
		{
			if(flag == 1)
			{
				process = "mpg123";
				rec = get_pid_by_name(process, pid, MAX_PID_NUM);
				printf("process '%s' is existed? (%d): %c\n", "arecord", rec, (rec > 0)?'y':'n');
				if(rec != 0)
				{
					kill(pid[0],SIGINT);
					goto end;
				}
			}
			sleep(1);
		}	
	}
	
end:
	printf("over.\n");
	return 0;
}