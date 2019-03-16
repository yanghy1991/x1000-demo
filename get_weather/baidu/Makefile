objects = cJSON.o bd_weather.o get_bd_weather.o
get_bd_weather:$(objects)
	g++ -o get_bd_weather $(objects)
$(objects): cJSON.h bd_weather.h
.PHONY:clean
clean:
	rm -rf get_bd_weather $(objects)
