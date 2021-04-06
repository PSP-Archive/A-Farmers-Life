
#define MAX_MAPS 2


typedef struct
{
	int x;
	int y;
} POINT;


typedef struct
{
	Image * image;
	int x;
	int y;
	int direction;
} Player;

typedef struct
{
	
} System;

typedef struct {
	char Name[MAX_PATH];

	char BgmPath[MAX_PATH];
	
	Image * CurrentMap;
	
	Image * TilesetImg;
	int TileWidth;
	int TileHeight;
	
	int StartX;
	int StartY;
	
	int Width;
	int Height;
	
	int NumObjects;
	
	char *Data;
} Map;

typedef struct
{
	
}Audio;

typedef struct
{
	char *versiontext;
	
	Image * background;
	Image * buttons;
	char BgmPath[MAX_PATH];
	int MaxOptions;
	struct
	{
		Image * image;
		int x;
	  int y;
	  int width;
	  int height;
	}cursor;
	struct
  {
	  int x;
	  int y;
	  int width;
	  int height;
	  int active;
	  int clicked;
	  char *HighlightCommands;
	  char *ClickCommands;
  } options[999];
} Titlescreen;



typedef struct
{
	int CurrentMap;
	Player * player;
	System * system;
	Map * map;
	Audio * audio;
	Titlescreen * titlescreen;
} Engine;

void Init_Engine(Engine *engine);
