#include "inc/main.h"

int done = 0;
extern int _mm_errno;
extern BOOL _mm_critical;
extern char *_mm_errmsg[];
int mikModThreadID = -1;

static int active=0;
static int outputEnabled = 1;

//MikMod Error handler
void my_error_handler(void)
{
	printf("_mm_critical %d\n", MikMod_critical);
	printf("_mm_errno %d\n", MikMod_errno);
	printf("%s\n", MikMod_strerror(MikMod_errno));
	return;
}

//MikMod thread
static int AudioChannelThread(int args, void *argp)
{
  while (!done)
  {
    MikMod_Update();
    // We have to sleep here to allow other threads a chance to process.
    // with no sleep this thread will take over when the output is disabled via MikMod_DisableOutput()
    // co-operative threading sucks bigtime...
    sceKernelDelayThread(1);
  }
  return (0);
}

extern UWORD md_mode;
extern UBYTE md_reverb;
extern UBYTE md_pansep;

int maxchan = 128;
MODULE *mf = NULL; // for mod
SAMPLE *sf = NULL; // for wav
int voice = 0; 	   // for wav
int pan = 127;
int vol = 255;
int mf_vol = 64;
int freq =	48000;

//Initialize MikMod
void mm_init(int channel)
{
	MikMod_InitThreads();
	
	MikMod_RegisterErrorHandler(my_error_handler);
  /* register all the drivers */
  MikMod_RegisterAllDrivers();
  /* register all the module loaders */
  MikMod_RegisterAllLoaders();

    /* initialize the library */
	md_mode = DMODE_16BITS|DMODE_STEREO|DMODE_SOFT_SNDFX|DMODE_SOFT_MUSIC; 
	md_reverb = 0;
	md_pansep = 128;
  if(MikMod_Init("")){
  	mm_exit();
  	return;
  }

  MikMod_SetNumVoices(-1, 8);
  
  mikModThreadID = sceKernelCreateThread("MikMod" ,(void*)&AudioChannelThread,0x12,0x10000,0,NULL);
  
  if(mikModThreadID>0)
  	sceKernelStartThread(mikModThreadID, 0 , NULL);
  else{
  	mm_exit();
  	return;
  }
  
  active=1;
  
  return;
}

//Load a wave file
int mm_loadwav(char *filename)
{
	if(!active) return 0;
		
	if(sf) Sample_Free(sf);
	sf = Sample_Load(filename);
	if(!sf) return 0;
	return 1;
}

//Play the loaded wave file
int mm_playwav()
{
	if(!active) return 0;
		
	MikMod_EnableOutput();
  
	voice = Sample_Play(sf,0,0);
	Voice_SetPanning(voice, pan);
	Voice_SetVolume(voice, vol);
	sceKernelWakeupThread(mikModThreadID);
	return 1;
}

int mm_playmod(char *filename)
{
	mf = Player_Load(filename, maxchan, 0);
  if (NULL != mf)
  {
   	mf->wrap = 1;
   	mf->volume = mf_vol;
		Player_Start(mf);
  }else
  	return 0;
  return 1;
}

int mm_stopmod()
{
	if (NULL != mf)
  {
		Player_Stop();
		Player_Free(mf); // To stop the song for real, it needs to be freed. I know, weird...
    mf = NULL;
  }else
  	return 0;
  return 1;
}

void mm_exit()
{
	// allow audio thread to terminate cleanly
  done = 1;
  if (mikModThreadID > 0)
  {
    sceKernelWakeupThread(mikModThreadID);
    sceKernelWaitThreadEnd(mikModThreadID, NULL);
    // not 100% sure if this is necessary after a clean exit, but just to make sure any resources are freed:
    sceKernelDeleteThread(mikModThreadID);
  }
	Player_Stop();
	Player_Free(mf);
	MikMod_Exit();	
	active=0;
}
