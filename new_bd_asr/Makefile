objects = cJSON.o base64.o bd_asr.o bd_asr_exe.o socket_fun.o
bd_asr_exe:$(objects)
	g++ -o bd_asr_exe $(objects)
$(objects): cJSON.h base64.h bd_asr.h
.PHONY:clean
clean:
	rm -rf bd_asr_exe $(objects)
