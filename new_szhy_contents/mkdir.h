#ifndef  __MKDIR_H_
#define  __MKDIR_H_
#endif

/**
* 目录是否存在
*/
int is_dir_exist(char *dir_path);

/**
* 创建多级目录(支持相对路径和据对路径)
*/
int create_dir(char* path);