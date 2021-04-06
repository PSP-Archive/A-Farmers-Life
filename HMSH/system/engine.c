#include "../inc/main.h"

void Init_Engine(Engine * engine)
{
	engine->CurrentMap = 0;
	Init_Player(engine);
	Init_System(engine);
	Init_Map(engine);
	Init_Audio(engine);
	Init_Titlescreen(engine);
	InitCommands();
}
