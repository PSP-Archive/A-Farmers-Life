#include "../inc/main.h"

void Init_Titlescreen(Engine * engine)
{

}

Titlescreen * titlescreenProcessFile(char *buffer, unsigned size)
{
	Titlescreen * titlescreen = malloc(sizeof(Titlescreen));
	if(!titlescreen) return NULL;

  titlescreen->MaxOptions = 0;

  debug_printf(1, "Lookup [TITLE]");
  while(strlen(buffer)>0)
  {
    if(!strncmp(buffer, "[TITLE]", 5)){
      break;
    }         
    buffer+=GetNextChar(buffer, '\n')+1;              
  }
  buffer+=GetNextChar(buffer, '\n')+1;       
  debug_printf(1, "[TITLE] found, getting [TITLE] entries");
  while(strlen(buffer)>0)
  {
    char buf[strlen(buffer)];
    
    int nl = GetNextChar(buffer, '\n');
    char end[nl+1];
    strncpy(end, buffer, nl);
    end[nl-1]='\0';

    if(StrStrGetInfo(end, "Version", buf))
    {
    	titlescreen->versiontext = malloc(strlen(buf));
    	strcpy(titlescreen->versiontext, buf);
    }

    if(StrStrGetInfo(end, "Bgm", buf))
    {
      sprintf(titlescreen->BgmPath, "%saudio/music/%s", EBOOTPath, buf);
      debug_printf(1, "Bgm found: %s", titlescreen->BgmPath);
    }
    if(StrStrGetInfo(end, "BGImage", buf))
    {
    	debug_printf(1, "BGImage found: %s", buf);
	    titlescreen->background = malloc(sizeof(Image));
      if(!titlescreen->background) error_callback("Could not allocate memory");
      
      titlescreen->background = imageOpenZipFile(DATA_ZIP, "graphics/images/%s", buf);
      if(!titlescreen->background) error_callback("Could not find %s/graphics/images/%s", DATA_ZIP, buf);
    }
    if(StrStrGetInfo(end, "OPImage", buf))
    {
    	debug_printf(1, "OPImage found: %s", buf);
	    titlescreen->buttons = malloc(sizeof(Image));
      if(!titlescreen->buttons) error_callback("Could not allocate memory");
      
      titlescreen->buttons = imageOpenZipFile(DATA_ZIP, "graphics/images/%s", buf);
      if(!titlescreen->buttons) error_callback("Could not find %s/graphics/images/%s", DATA_ZIP, buf);
    }

    if(StrStrGetInfo(end, "Cursor", buf))
    {
    	debug_printf(1, "Cursor found: %s", buf);
    	char curimgbuf[MAX_PATH];
    	char *curtext = malloc(strlen(buf));
      curtext = buf;

      strcpy(curimgbuf, SplitText(curtext, '|'));
      curtext+=strlen(SplitText(curtext, '|'))+1;

      titlescreen->cursor.image = malloc(sizeof(Image));
      if(!titlescreen->cursor.image ) error_callback("Could not allocate memory");
      
      titlescreen->cursor.image = imageOpenZipFile(DATA_ZIP, "graphics/images/%s", curimgbuf);
      if(!titlescreen->cursor.image) error_callback("Could not find %s/graphics/images/%s", DATA_ZIP, curimgbuf);

      titlescreen->cursor.x = atoi(SplitText(curtext, '|'));
      curtext+=strlen(SplitText(curtext, '|'))+1;
      titlescreen->cursor.y = atoi(SplitText(curtext, '|'));
      curtext+=strlen(SplitText(curtext, '|'))+1;
      titlescreen->cursor.width = atoi(SplitText(curtext, '|'));
      curtext+=strlen(SplitText(curtext, '|'))+1;
      titlescreen->cursor.height = atoi(SplitText(curtext, '|'));
      curtext+=strlen(SplitText(curtext, '|'))+1;
    	
    	
    	if(titlescreen->cursor.width<=0) titlescreen->cursor.width = titlescreen->cursor.image->imageWidth;
    	if(titlescreen->cursor.height<=0) titlescreen->cursor.height = titlescreen->cursor.image->imageHeight;
    }

    if(StrStrGetInfo(end, "Option", buf))
    {
    	debug_printf(1, "Option found: %s", buf);
    	int tmpid = 0;

      char *optext = malloc(strlen(buf));
      optext = buf;

      tmpid = atoi(SplitText(optext, '|')); 
      optext+=strlen(SplitText(optext, '|'))+1;
      titlescreen->options[tmpid].x = atoi(SplitText(optext, '|'));
      optext+=strlen(SplitText(optext, '|'))+1;
      titlescreen->options[tmpid].y = atoi(SplitText(optext, '|'));
      optext+=strlen(SplitText(optext, '|'))+1;
      titlescreen->options[tmpid].width = atoi(SplitText(optext, '|'));
      optext+=strlen(SplitText(optext, '|'))+1;
      titlescreen->options[tmpid].height = atoi(SplitText(optext, '|'));
      optext+=strlen(SplitText(optext, '|'))+1;
      titlescreen->options[tmpid].HighlightCommands = malloc(GetNextChar(optext, '|')+1);
      titlescreen->options[tmpid].HighlightCommands = SplitText(optext, '|');
      titlescreen->options[tmpid].HighlightCommands[GetNextChar(optext, '|')] = '\0';
      optext+=GetNextChar(optext, '|')+1;
      titlescreen->options[tmpid].ClickCommands = malloc(GetNextChar(optext, '|')+1);
      titlescreen->options[tmpid].ClickCommands = SplitText(optext, '|');
      titlescreen->options[tmpid].ClickCommands[GetNextChar(optext, '|')] = '\0';
      optext+=GetNextChar(optext, '|')+1;
      
      titlescreen->options[tmpid].active = 0;
      titlescreen->options[tmpid].clicked = 0;

      titlescreen->MaxOptions++;
    }
    
    if(!strncmp(end, "[/TITLE]", 6)){
    	debug_printf(1, "[/TITLE] found, no more entries");
      break;
    }     
    buffer+=nl+1;               
  }

	debug_printf(1, "[TITLE] done");
  
	return titlescreen;
}

void Blit_Titlescreen(Engine * engine)
{
  blitImageToScreen(0, 0, engine->titlescreen->background->imageWidth, engine->titlescreen->background->imageHeight, engine->titlescreen->background, 0, 0);

  for(int i=0;i<engine->titlescreen->MaxOptions;i++)
  {
    if(Collide(engine->titlescreen->options[i].x, engine->titlescreen->options[i].y, engine->titlescreen->options[i].width, engine->titlescreen->options[i].height, engine->titlescreen->cursor.x, engine->titlescreen->cursor.y, 1, 1))
    {	
    	if(!engine->titlescreen->options[i].active) {ProcessCommands(engine, engine->titlescreen->options[i].HighlightCommands);}
      if(now_pad & PSP_CTRL_CROSS) 
      {
      	if(!engine->titlescreen->options[i].clicked) ProcessCommands(engine, engine->titlescreen->options[i].ClickCommands);
      	engine->titlescreen->options[i].clicked = 1;
      }
      else engine->titlescreen->options[i].clicked = 0;
      engine->titlescreen->options[i].active = 1;
    }
    else
      engine->titlescreen->options[i].active = engine->titlescreen->options[i].clicked = 0;

    blitAlphaImageToScreen(engine->titlescreen->options[i].width*i, (engine->titlescreen->options[i].active+engine->titlescreen->options[i].clicked)*engine->titlescreen->options[i].height, engine->titlescreen->options[i].width, engine->titlescreen->options[i].height, engine->titlescreen->buttons, engine->titlescreen->options[i].x, engine->titlescreen->options[i].y);
  }

  blitAlphaImageToScreen(0, 0, engine->titlescreen->cursor.width, engine->titlescreen->cursor.height, engine->titlescreen->cursor.image, engine->titlescreen->cursor.x, engine->titlescreen->cursor.y);

  printTextf(480-(strlen(engine->titlescreen->versiontext)*8), 272-8, RGB(255, 255, 255), engine->titlescreen->versiontext);
}


void Titlescreen_Main(Engine * engine)
{
	engine->titlescreen = malloc(sizeof(Titlescreen));
	
	engine->titlescreen = (Titlescreen *)titlescreenOpenZipFile(DATA_ZIP, "data/system/titlescreen.fld");
	if(engine->titlescreen==NULL) error_callback("Could not load titlescreen");

  FadeInScreen(engine, 10);
  bgm_play(engine->titlescreen->BgmPath);


  while(1)
  {
  	readpad();

    unsigned int curaddrx = (unsigned int) &engine->titlescreen->cursor.x;
    unsigned int curaddry = (unsigned int) &engine->titlescreen->cursor.y;
    readanalog(curaddrx, curaddry);

    clearScreen(0);
  	Blit_Titlescreen(engine);

  	flipScreen();
  	sceDisplayWaitVblankStart();
  	
  	if(engine->CurrentMap!=0) break;
  }
  
  bgm_stop();
	FadeOutScreen(engine, 10);
  
  free(engine->titlescreen->versiontext);
  freeImage(engine->titlescreen->background);
  freeImage(engine->titlescreen->buttons);
  freeImage(engine->titlescreen->cursor.image);
  free(engine->titlescreen);
}
