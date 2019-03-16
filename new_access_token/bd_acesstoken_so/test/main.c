#include "bd_token.h"

int main(int argc , char **argv)
{
	char token_path[128] = "/tmp/data";
	if(get_bd_tokenaccess(token_path) != 0)
	{
		printf("access token failed \n");
		return -1;
	}
	
	printf("get bd access token success\n");
	
	return 0;
}