objects = cJSON.o base64.o bd_asr.o http_server.o http_get.o
http_server:$(objects)
	g++ -o http_server $(objects)
$(objects): cJSON.h base64.h bd_asr.h
.PHONY:clean
clean:
	rm -rf http_server $(objects)
