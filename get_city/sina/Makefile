#可执行文件
target = get_city_from_sina
#源文件
SRCS = cJSON.c city_sina.c get_city_from_sina.c
#依赖目标
objects = $(SRCS:.c=.o)
#指令编译器
CC = mipsel-openwrt-linux-gcc
$(target):$(objects)
	mipsel-openwrt-linux-g++ -o $(target) $(objects)
$(objects): cJSON.h city_sina.h
	$(CC) -c $(SRCS)
.PHONY:clean
clean:
	rm -rf $(target) $(objects)