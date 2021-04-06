#include "inc/main.h"

#define printf pspDebugScreenPrintf
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y)) 

PSP_MODULE_INFO("A Farmer\'s life", 0, 1, 1);


/* Exit callback */
int exit_callback() {
	bgm_end();
	se_end();
	
	CloseZip();
	
 	sceKernelExitGame();
	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
  int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0) {
  	sceKernelStartThread(thid, 0, 0);
	}
	return thid;
}


int main(int argc, char **argv)
{
	char *p;
	strcpy(EBOOTPath, *argv);
	p = strrchr(EBOOTPath, '/');
	*++p = 0;
	
	//scePowerSetClockFrequency(333, 333, 166);

	pspDebugScreenInit();
  SetupCallbacks();
  pspAudioInit();
  
  se_init(1);
  bgm_init(0);
  
 	initGraphics();
 	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

  OpenZip(DATA_ZIP);

  Engine * engine = malloc(sizeof(Engine));
  Init_Engine(engine);
  
  engine->CurrentMap = 0;
  
  while(1)
  {
    switch(engine->CurrentMap)
    {
  	  case -2:
  		  exit_callback();
  	  break;
  	  case 0:
  		  Titlescreen_Main(engine);
  	  break;
  	  default:
  		  Map_Main(engine);
  	  break;
    }
  }

 	exit_callback();
 	return 0;
} 
