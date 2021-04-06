
int Collide(int fx, int fy, int fw, int fh, int sx, int sy, int sw, int sh);

void FadeInScreen(Engine * engine, int mod);
void FadeOutScreen(Engine * engine, int mod);

char *SplitText(char *text, char splitchar);

int InsertText(char *text, char *ins, int pos);
int GetNextChar(char *str, char lookup);
float FloatStrGetInfo(char *str, char *lookupstr);
int IntStrGetInfo(char *str, char *lookupstr);
int StrStrGetInfo(char *str, char *lookupstr, char *out);
