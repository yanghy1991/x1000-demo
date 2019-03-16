#可执行文件
target = http_server
#源文件
SRCS = cJSON.c base64.c bd_asr.c http_server.c http_get.c
#依赖目标
objects = $(SRCS:.c=.o)
#指令编译器
CC = mipsel-linux-gcc
$(target):$(objects)
	mipsel-linux-g++ -o $(target) $(objects)
$(objects): cJSON.h base64.h bd_asr.h
	$(CC) -c $(SRCS)
.PHONY:clean
clean:
	rm -rf $(target) $(objects)
