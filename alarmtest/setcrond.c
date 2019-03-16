#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv)
{
	char crontab[1024];
	memset(crontab,0,1024);
	strcat(crontab,"echo \"");

	strcat(crontab,"");
	strcat(crontab," ");
	strcat(crontab,reminder->hours);
	strcat(crontab," ");
	strcat(crontab,reminder->days);
	strcat(crontab," ");
	strcat(crontab,reminder->months);
	strcat(crontab," ");
	char week[20];
	memset(week,0,20);
	if(reminder->mon == 1){
		strcat(week,"1,");
	}
	if(reminder->tue == 1){
		strcat(week,"2,");
	}
	if(reminder->wed == 1){
		strcat(week,"3,");
	}
	if(reminder->thu == 1){
		strcat(week,"4,");
	}
	if(reminder->fri == 1){
		strcat(week,"5,");
	}
	if(reminder->sat == 1){
		strcat(week,"6,");
	}
	if(reminder->sun == 1){
		strcat(week,"0,");
	}
	if((reminder->mon == 0) && (reminder->tue == 0) && (reminder->wed == 0) && (reminder->thu == 0) && (reminder->fri == 0) && (reminder->sat == 0) && (reminder->sun == 0)){
		strcat(week,"*");
	}
	int lastw = strlen(week)-1;
	if(week[lastw] == ','){
		week[lastw] = '\0';
	}
	
	strcat(crontab,week);
	strcat(crontab," ");
	strcat(crontab,"/usr/fs/usr/share/vr/res/alarm.sh");
//	strcat(crontab,reminder->alarmId);
//	strcat(crontab," ");
//	strcat(crontab,reminder->topic);
	
//	strcat(crontab,"meet");
//	strcat(crontab,"\r\n");
	strcat(crontab,"\" >> /var/spool/cron/crontabs/root");
	printf("crontab = %s\n",crontab);
	system(crontab);
	
	return 0;
}