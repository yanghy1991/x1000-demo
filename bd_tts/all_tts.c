#include <stdio.h>

int main(int argc,char **argv)
{
	//今天
	system("./bd_tts_exe data/sina_weather0.txt data/sina_weather0.mp3");
	system("./bd_tts_exe data/sina_weather0_bk.txt data/sina_weather0_bk.mp3");
	system("./bd_tts_exe data/bd_weather0.txt data/bd_weather0.mp3");
	system("./bd_tts_exe data/bd_weather0_bk.txt data/bd_weather0_bk.mp3");
	system("./bd_tts_exe data/bureau_weather.txt data/bureau_weather.mp3");
	system("./bd_tts_exe data/bureau_weather_bk.txt data/bureau_weather_bk.mp3");
	
	//明天
	system("./bd_tts_exe data/sina_weather1.txt data/sina_weather1.mp3");
	system("./bd_tts_exe data/sina_weather1_bk.txt data/sina_weather1_bk.mp3");
	system("./bd_tts_exe data/bd_weather1.txt data/bd_weather1.mp3");
	system("./bd_tts_exe data/bd_weather1_bk.txt data/bd_weather1_bk.mp3");
	
	//后天
	system("./bd_tts_exe data/sina_weather2.txt data/sina_weather2.mp3");
	system("./bd_tts_exe data/sina_weather2_bk.txt data/sina_weather2_bk.mp3");
	system("./bd_tts_exe data/bd_weather2.txt data/bd_weather2.mp3");
	system("./bd_tts_exe data/bd_weather2_bk.txt data/bd_weather2_bk.mp3");
	
	return 0;
}