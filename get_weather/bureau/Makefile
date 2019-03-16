#可执行文件
target = get_bureau_weather
#源文件
SRCS = cJSON.c bureau_weather.c get_bureau_weather.c
#依赖目标
objects = $(SRCS:.c=.o)
#指令编译器
CC = mipsel-openwrt-linux-gcc
$(target):$(objects)
	mipsel-openwrt-linux-g++ -o $(target) $(objects)
$(objects): cJSON.h bureau_weather.h
	$(CC) -c $(SRCS)
.PHONY:clean
clean:
	rm -rf $(target) $(objects)