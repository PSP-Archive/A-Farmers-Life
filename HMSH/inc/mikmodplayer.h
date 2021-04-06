
void mm_init(int channel);


//Load a wave file
int mm_loadwav(char *filename);
//Play the loaded wave file
int mm_playwav();
//Play/load module
int mm_playmod(char *filename);
//Stop module
int mm_stopmod();
//Exit MikMod
void mm_exit();
