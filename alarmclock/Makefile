#可执行文件
target = set_clock
#源文件
SRCS = cJSON.c set_clock.c
#依赖目标
objects = $(SRCS:.c=.o)
#指令编译器
CC = mipsel-openwrt-linux-gcc
$(target):$(objects)
	mipsel-openwrt-linux-g++ -o $(target) $(objects)
$(objects): cJSON.h
	$(CC) -c $(SRCS)
.PHONY:clean
clean:
	rm -rf $(target) $(objects)