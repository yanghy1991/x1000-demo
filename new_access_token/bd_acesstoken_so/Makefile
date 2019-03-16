#可执行文件
target = libbdaccesstoken.so
#源文件
SRCS = cJSON.c bd_token.c mkdir.c socket_fun.c
#依赖目标
objects = $(SRCS:.c=.o)
#指令编译器
CC = mipsel-linux-gcc

$(target):$(objects)
	$(CC) -o $(target) $(objects) -lm -shared -fPIC

$(objects): cJSON.h bd_token.h mkdir.h socket_fun.h
	$(CC) -c $(SRCS) -fPIC
	
.PHONY:clean
clean:
	rm -rf $(target) $(objects)
