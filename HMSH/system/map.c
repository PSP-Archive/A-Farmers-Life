#include "../inc/main.h"


void Init_Map(Engine * engine)
{
  
}


Map *mapProcessFile(char *buffer, unsigned size)
{
	Map * map = malloc(sizeof(Map));
	if(!map) return NULL;

  map->BgmPath[0]='\0';

  debug_printf(1, "Lookup [MAP]");
  while(strlen(buffer)>0)
  {
    if(!strncmp(buffer, "[MAP]", 5)){
      break;
    }         
    buffer+=GetNextChar(buffer, '\n')+1;              
  }
  buffer+=GetNextChar(buffer, '\n')+1;       
  debug_printf(1, "[MAP] found, getting [MAP] entries");
  while(strlen(buffer)>0)
  {
    char buf[strlen(buffer)];
    
    int nl = GetNextChar(buffer, '\n');
    char end[nl+1];
    strncpy(end, buffer, nl);
    end[nl-1]='\0';

    if(StrStrGetInfo(end, "Name", buf))
    {
      StrStrGetInfo(end, "Name", map->Name);
      debug_printf(1, map->Name);
    }
    if(StrStrGetInfo(end, "Bgm", buf))
    {
      sprintf(map->BgmPath, "%saudio/music/%s", EBOOTPath, buf);
      debug_printf(1, map->BgmPath);
    }
    if(StrStrGetInfo(end, "Image", buf))
    {
	    map->TilesetImg = malloc(sizeof(Image));
      if(!map->TilesetImg) error_callback("Could not allocate memory");
      
      map->TilesetImg = imageOpenZipFile(DATA_ZIP, "graphics/tileset/%s", buf);
      if(!map->TilesetImg) error_callback("Could not find %s/graphics/tileset/%s", DATA_ZIP, buf);
    }
    if(StrStrGetInfo(end, "Data", buf))
    {
    	map->Data = malloc(map->Width * map->Height * 2); memset(map->Data, 0, map->Width * map->Height * 2);
    	if(!map->Data) error_callback("Could not allocate memory");

      buffer+=7;
      
      while((nl = GetNextChar(buffer, '\n')-1)==map->Width*2)
      {
      	char datbuf[nl+1];
      	strncpy(datbuf, buffer, nl);
      	datbuf[nl]='\0';
      	debug_printf(1, datbuf);
      	strncat(map->Data, datbuf, nl);
      	buffer+=nl+2;
      }
      map->Data[map->Width * map->Height * 2]='\0';
      continue;
    }
    if(IntStrGetInfo(end, "TileWidth")>-2)
    	map->TileWidth = IntStrGetInfo(end, "TileWidth");
    if(IntStrGetInfo(end, "TileHeight")>-2)
    	map->TileHeight = IntStrGetInfo(end, "TileHeight");
    if(IntStrGetInfo(end, "StartX")>-2)
    	map->StartX = IntStrGetInfo(end, "StartX");
    if(IntStrGetInfo(end, "StartY")>-2)
    	map->StartY = IntStrGetInfo(end, "StartY");
    if(IntStrGetInfo(end, "Width")>-2)
    	map->Width = IntStrGetInfo(end, "Width");
    if(IntStrGetInfo(end, "Height")>-2)
    	map->Height = IntStrGetInfo(end, "Height");
    if(IntStrGetInfo(end, "Objects")>-2)
    	map->NumObjects = IntStrGetInfo(end, "Objects");
    
    if(!strncmp(end, "[/MAP]", 6)){
    	debug_printf(1, "[/MAP] found, no more entries");
      break;
    }     
    buffer+=nl+1;               
  }
  
	debug_printf(1, "[MAP] done");
  
	return map;
}

void GetCurrent_Map(Engine * engine)
{
	const int rows = 30, cols = 17;
	
	int cx = 0, cy = 0;  //Center if the map is smaller than the screen
	int ym = 0, xm = 0;
	
	if(engine->map->Width*engine->map->TileWidth<480) cx = (480-engine->map->Width*engine->map->TileWidth)/2;
	if(engine->map->Height*engine->map->TileHeight<272) cy = (272-engine->map->Height*engine->map->TileHeight)/2;
	
	for(int y=0;y<cols;y++)
	{
		for(int x=0;x<rows;x++)
		{
			if(x>engine->map->Width) break;
			int sx = 0, sy = 0;
			
			char data[2];
			data[0] = engine->map->Data[(xm+(engine->map->StartX*2))+(ym+(engine->map->StartY*engine->map->Width*2))];
			data[1] = engine->map->Data[(xm+(engine->map->StartX*2+1))+(ym+(engine->map->StartY*engine->map->Width*2))];
			data[2] = '\0';
			
			sx = atoi(data);
			while(sx>=engine->map->TilesetImg->imageWidth/engine->map->TileWidth)
			{
				sx-=engine->map->TilesetImg->imageWidth/engine->map->TileWidth;
				sy++;
			}
			
			blitAlphaImageToImage(sx*engine->map->TileWidth, sy*engine->map->TileHeight, engine->map->TileWidth, engine->map->TileHeight, engine->map->TilesetImg, (x*engine->map->TileWidth)+cx, (y*engine->map->TileHeight)+cy, engine->map->CurrentMap);
		  //printTextf(x*engine->map->TileWidth, y*engine->map->TileHeight, RGB(0, 0, 0), "%d\n%d", sx, sy);
		  
		  xm+=2;
		}
		if(y>engine->map->Height) break;
		ym+=engine->map->Width*2;
		xm=0;
		if(!engine->map->Data[(xm+(engine->map->StartX*2))+(ym+(engine->map->StartY*engine->map->Width*2))]) break;
	}
}

void Blit_Map(Engine * engine)
{
	blitImageToScreen(0, 0, engine->map->CurrentMap->imageWidth, engine->map->CurrentMap->imageHeight, engine->map->CurrentMap, 0, 0);
	#ifdef FL_DEBUG
	printTextf(1, 1, RGB(255, 255, 255), "%s\nWidth: %d\nHeight: %d", engine->map->Name, engine->map->Width, engine->map->Height);
  #endif
}

void Reload_Map(Engine * engine)
{
	GetCurrent_Map(engine);
}

void Map_Main(Engine * engine)
{
	engine->map = malloc(sizeof(Map));
	
	char path[MAX_PATH];
	sprintf(path, "data/map/map%03d.fld", engine->CurrentMap);
	engine->map = mapOpenZipFile(DATA_ZIP, path);
	if(engine->map==NULL) error_callback("Could not load map: %s", path);
		
	engine->map->CurrentMap = malloc(sizeof(Image));
  engine->map->CurrentMap = createImage(480, 272);
	Reload_Map(engine);
		
	FadeInScreen(engine, 10);
	bgm_play(engine->map->BgmPath);

  while(1)
  { 	          
    readpad();
    if(new_pad & PSP_CTRL_DOWN)
    {	if(17+engine->map->StartY<engine->map->Height) engine->map->StartY++; Reload_Map(engine); }
    if(new_pad & PSP_CTRL_UP)
    {	if(engine->map->StartY>0) engine->map->StartY--; Reload_Map(engine); }
    if(new_pad & PSP_CTRL_RIGHT)
    {	if(30+engine->map->StartX<engine->map->Width) engine->map->StartX++; Reload_Map(engine); }
    if(new_pad & PSP_CTRL_LEFT)
    {	if(engine->map->StartX>0) engine->map->StartX--; Reload_Map(engine); }
    if(new_pad & PSP_CTRL_CIRCLE)
    {engine->CurrentMap=0; break;}
    if(new_pad & PSP_CTRL_CROSS)
    {
    	engine->CurrentMap++;
    	if(engine->CurrentMap>MAX_MAPS) engine->CurrentMap = 	1;
    	break;
    }

    clearScreen(0);

    Blit_Map(engine);
    
    //#ifdef FL_DEBUG
    GetFPS();
    //#endif

    flipScreen();
    sceDisplayWaitVblankStart();
  }
  
  bgm_stop();
	FadeOutScreen(engine, 10);
  
  freeImage(engine->map->TilesetImg);
  free(engine->map->Data);
  free(engine->map);
}
