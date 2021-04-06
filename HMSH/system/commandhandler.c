#include "../inc/main.h"

void PlaySE(Engine * engine, char **param)
{
	char *szFile = malloc(strlen(DEFAULT_SE)+strlen(param[0])+1);
	sprintf(szFile, "%s%s", DEFAULT_SE, param[0]);
  se_play(szFile);
}
void Teleport(Engine * engine, char **param)
{
  engine->CurrentMap = atoi(param[0]); 
}

#define NUM_COMMANDS 2
const char AvailableCommands[NUM_COMMANDS][16] = {"PlaySE", "Teleport"};
struct
{
  void (*func)(Engine * engine, char **param);    
} Commands[NUM_COMMANDS];

char **GetParameters(char *str)
{
	char *buf = malloc(strlen(str)+1);
	buf = str;
	buf+=GetNextChar(buf, '(')+1;
	
	int curparam = 0;
	char **ret = malloc(GetNumChars(buf, ',')+1);
	for(int i=0;i<GetNumChars(buf, ',')+1;i++)
	  ret[i] = "";
	
	while(*buf!='\0')
	{
    int NextParam = GetNextChar(buf, ',');
    if(!NextParam) NextParam = GetNextChar(buf, ')');
    if(!NextParam) break;
	  if(NextParam>0){
	  	while(*buf==' ') buf++;
	  	ret[curparam] = (char *)malloc(NextParam+1);
	  	strncpy(ret[curparam], buf, NextParam);
	  	ret[curparam][NextParam]='\0';
	    curparam++;
	  }
	  buf+=NextParam+1;
	  if(*buf=='\0') break;
	}
  return ret;
}

void ProcessSingleCommand(Engine * engine, char *command)
{
  while(*command==' ' || *command=='\n') {if(*command+1=='\0') return;command++;}
  debug_printf(1, GetParameters(command));
  for(int i=0;i<NUM_COMMANDS;i++)
  {
    if(!strncasecmp(AvailableCommands[i], command, strlen(AvailableCommands[i]))) {Commands[i].func(engine, GetParameters(command)); break;}     
  }
}

void ProcessCommands(Engine * engine, char *commandstr)
{
	while(*commandstr!='\0')
	{
    int NextCmd = GetNextChar(commandstr, ':');
	  if(NextCmd>0){
	  	char *command = (char *)malloc(NextCmd);
	  	strncpy(command, commandstr, NextCmd);
	  	command[NextCmd]='\0';
	    ProcessSingleCommand(engine, command);
	  }
	  commandstr+=NextCmd+1;
	  if(*commandstr=='\0') return;
	}
}

void InitCommands()
{
	Commands[0].func = PlaySE;
  Commands[1].func = Teleport;
}
