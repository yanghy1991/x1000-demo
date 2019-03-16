objects = cJSON.o base64.o bd_analysis_words.o main.o
main:$(objects)
	g++ -o main $(objects)
$(objects): cJSON.h base64.h bd_analysis_words.h
.PHONY:clean
clean:
	rm -rf main $(objects)
