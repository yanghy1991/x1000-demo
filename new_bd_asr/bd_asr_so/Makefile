#可执行文件
target = libbaiduasr.so
#源文件
SRCS = cJSON.c base64.c mkdir.c socket_fun.c bd_asr.c
#依赖目标
objects = $(SRCS:.c=.o)
#指令编译器
CC = mipsel-linux-gcc

$(target):$(objects)
	$(CC) -o $(target) $(objects) -lm -shared -fPIC

$(objects): cJSON.h bd_asr.h mkdir.h socket_fun.h base64.h
	$(CC) -c $(SRCS) -fPIC
	
.PHONY:clean
clean:
	rm -rf $(target) $(objects)
