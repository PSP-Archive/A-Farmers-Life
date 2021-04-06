#include "inc/main.h"


//Check for collision
int Collide(int fx, int fy, int fw, int fh, int sx, int sy, int sw, int sh)
{
	int ret=0;
	int x, px, y, py;
	int ifx=0, ify=0;

		for(px=0;px<fw;px++){
			for(x=0;x<sw;x++){
				if(fx+px == sx+x)
					ifx = 1;
			}
		}
		for(py=0;py<fh;py++){
			for(y=0;y<sh;y++){
				if(fy+py == sy+y)
					ify = 1;
			}
		}
	if(ify && ifx) ret=1;
	return ret;
}



void FadeInScreen(Engine * engine, int mod)
{
	int a = 255;

	FadeScreen(a);

	for(;;)
	{
	  sceDisplayWaitVblankStart();
	  
	  if(engine->CurrentMap>0) Blit_Map(engine);
	  else if(engine->CurrentMap==0) Blit_Titlescreen(engine);
	  
	  FadeScreen(a);
	  
	  flipScreen();
	  a-=mod;
	  sceKernelDelayThread(1);
	  if(a<=0) break;
	}
}
void FadeOutScreen(Engine * engine, int mod)
{
	int a = 0;

	FadeScreen(a);

	for(;;)
	{
	  sceDisplayWaitVblankStart();
	  
	  FadeScreen(a);
	  
	  flipScreen();
	  a+=mod;
	  sceKernelDelayThread(1);
	  if(a>=255) break;
	}
}

char *SplitText(char *text, char splitchar)
{
  int nc = GetNextChar(text, splitchar);
  char *buf = malloc(nc);  
  strncpy(buf, text, nc);
  buf[nc]='\0';
  return buf;
}

int InsertText(char *text, char *ins, int pos)
{
  pos-=1;
    
  for(int s=0;s<strlen(ins);s++)
  {
    for(int i=strlen(text);i>pos;i--)
    {
      text[i+1] = text[i];
    }
  }
  for(int i=0;i<strlen(ins);i++)
   text[pos+1+i]=ins[i];
  
  return 1;
}

int GetNextChar(char *str, char lookup)
{
  char *buf = (char *)malloc(strlen(str)+1);
  buf = str;
	int ret = 0;
	while(1)
	{  
      if(*buf==lookup) break;  
      if(*buf=='\0') return 0; 
      
      buf++;
      ret++;
    }
	
	return ret;
}

int GetNumChars(char *str, char lookup)
{
	char *buf = (char *)malloc(strlen(str)+1);
  buf = str;
	int ret = 0;
	while(1)
	{  
      if(*buf==lookup) ret++;  
      if(*buf=='\0') break; 
      
      buf++;
    }
	
	return ret;
}

float FloatStrGetInfo(char *str, char *lookupstr)
{
  if(!strncasecmp(str, lookupstr, strlen(lookupstr)))
  {
    char *p;
    p = str;
    while(strncasecmp(p, "= ", 2) && p)
      *p++;
    p+=2;
    return atof(p);
  }
  return -1.0;
}

int IntStrGetInfo(char *str, char *lookupstr)
{
  if(!strncasecmp(str, lookupstr, strlen(lookupstr)))
  {
    char *p;
    p = str;
    while(strncasecmp(p, "= ", 2) && p)
      *p++;
    p+=2;

    return atoi(p);
  }
  return -2;
}

int StrStrGetInfo(char *str, char *lookupstr, char *out)
{   
  if(!strncasecmp(str, lookupstr, strlen(lookupstr)))
  {
    char *p;
    p = str;
    while(strncasecmp(p, "= ", 2) && p)
      *p++;
    p+=2;

    strcpy(out, p);
    return 1;
  }
  return 0;
}
