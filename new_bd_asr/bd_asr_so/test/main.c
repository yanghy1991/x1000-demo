#include "bd_asr.h"

int main(int argc,char **argv)
{
	char *file_path = "/home/others/x1000_my_demo/new_bd_asr/bd_asr_so/test/bdtoken.txt";
	char *audio_path = "/home/others/x1000_my_demo/new_bd_asr/bd_asr_so/test/tq.wav";
	char *audio_format = "wav";
	char *cuid = "00:0c:29:fb:d7:52";
	char result[ASR_RESP_BUFSIZE];
	memset(result,0,ASR_RESP_BUFSIZE);
	char err_msg[ASR_SMALL_BUFSIZE];
	memset(err_msg,0,ASR_SMALL_BUFSIZE);
	int asr_err_no = 1;
	int *error_no = &asr_err_no;
	
	if(-1 == bd_asr(file_path,audio_path,audio_format,cuid,result,err_msg,error_no))
	{
		printf("baidu asr failed ......\n");
		return -1;
	}
	else
	{
		//验证失败,更新accesstoken,之后再重新asr
		if(asr_err_no == 3302)
		{
			
		}
		else
		{
			printf("error_no = %d\nerr_msg = %s\n",asr_err_no,err_msg);
			printf("result = %s\n",result);
		}		
	}
	return 0;
}