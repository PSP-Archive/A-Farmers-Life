#include "inc/main.h"

//Read controller information
void readpad()
{
	static int n=0;

	sceCtrlReadBufferPositive(&paddata, 1);

	now_pad = paddata.Buttons;
	new_pad = now_pad & ~old_pad;
	if(old_pad==now_pad){
		n++;
		if(n>=15){
			new_pad=now_pad;
			n = 10;
		}
	}else{
		n=0;
		old_pad = now_pad;
	}
}

void readanalog(unsigned int x, unsigned int y)
{
	if(paddata.Lx>200) *(int*)x+= 2;
	if(paddata.Ly>200) *(int*)y+= 2;
	if(paddata.Lx<55) *(int*)x-= 2;
	if(paddata.Ly<55) *(int*)y-= 2;
}
