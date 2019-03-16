objects = cJSON.o bd_tts.o bd_tts_exe_str.o
bd_tts_exe_str:$(objects)
	g++ -o bd_tts_exe_str $(objects)
$(objects): cJSON.h bd_tts.h
.PHONY:clean
clean:
	rm -rf bd_tts_exe_str $(objects)
