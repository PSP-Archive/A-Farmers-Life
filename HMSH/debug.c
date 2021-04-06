#include "inc/main.h"

//#define FL_DEBUG

int debug_printf(int waitkeypress, char *szText, ...)
{
	#ifdef FL_DEBUG
	va_list ap;
	char szBuf[2048];

	va_start(ap, szText);
	vsprintf(szBuf, szText, ap);
	va_end(ap);
	
	clearScreen(0);
  printTextf(1, 1, RGB(255, 255, 255), "%s%s", waitkeypress==2?"Fatal error: ":"", szBuf);
  flipScreen();
  sceDisplayWaitVblankStart();
      	
      	
  if(waitkeypress==2)
  {
  	new_pad = 0;
    while(!new_pad) {readpad();}
    exit_callback();	
  }    	
  
  if(waitkeypress==1)
  {
    new_pad = 0;
    while(!new_pad) {readpad();}
  }
  #endif
  return 1;
}


int error_callback(char *szText, ...)
{
	va_list ap;
	char szBuf[2048];

	va_start(ap, szText);
	vsprintf(szBuf, szText, ap);
	va_end(ap);
	
	clearScreen(0);
  printTextf(1, 1, RGB(255, 75, 75), "Error: %s\n\nPress any key to quit.", szBuf);
  flipScreen();
  sceDisplayWaitVblankStart();
      	
  new_pad = 0;
  while(!new_pad) {readpad();}
  exit_callback();	
    
  return 1;
}
