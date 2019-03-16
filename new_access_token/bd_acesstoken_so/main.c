#include "bd_token.h"

int main(int argc , char **argv)
{
	char token_path[128] = "/home/others/x1000_my_demo/new_access_token/bd_acesstoken_so/test";
	if(get_bd_tokenaccess(token_path) != 0)
	{
		printf("access token failed \n");
		return -1;
	}
	
	return 0;
}