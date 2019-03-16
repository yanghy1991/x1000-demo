objects = cJSON.o bd_token.o get_bd_token.o
get_bd_token:$(objects)
	g++ -o get_bd_token $(objects)
$(objects): cJSON.h bd_token.h
.PHONY:clean
clean:
	rm -rf get_bd_token $(objects)
