#include "../inc/main.h"

void Init_Audio(Engine * engine)
{

}

void bgm_init(int channel)
{
	OGG_Init(channel);
}
void se_init(int channel)
{
	mm_init(channel);
}

void bgm_play(char *fname, ...)
{
	va_list ap;
	char filename[2048];

	va_start(ap, fname);
	vsprintf(filename, fname, ap);
	va_end(ap);
	
	if(filename[0]=='\0') return;
	
	if(OGG_IsPlaying()) bgm_stop();
	
	if(!OGG_Load(filename)) error_callback("Could not load: %s", filename);
		
		
	float volume = 0.0;
	while(volume<5.0)
	{
		volume+=0.01;
		OGG_SetVolume(volume);
		sceKernelDelayThread(100);
	}
	
		
	OGG_Play();
}
void se_play(char *fname, ...)
{
	va_list ap;
	char filename[2048];

	va_start(ap, fname);
	vsprintf(filename, fname, ap);
	va_end(ap);
	
	if(!mm_loadwav(filename)) error_callback("Could not load: %s", filename);
	if(!mm_playwav()) error_callback("Could not play: %s", filename);
}

void bgm_stop()
{
	float volume = 5.0;
	while(volume>0.0)
	{
		volume-=0.01;
		OGG_SetVolume(volume);
		sceKernelDelayThread(100);
	}
	OGG_Stop();
	OGG_FreeTune();
}
void se_stop()
{
	//mm_stopwav();
}

void bgm_pause()
{
	OGG_Pause();
}
void se_pause()
{
	//mm_pause();
}

void bgm_end()
{
	OGG_End();
}
void se_end()
{
	mm_exit();
}
