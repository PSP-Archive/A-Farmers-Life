
#define DEFAULT_SE "audio/sound/"
#define DEFAULT_BGM "audio/music"

void Init_Audio(Engine * engine);

void bgm_init(int channel);
void se_init(int channel);

void bgm_play(char *fname, ...);
void se_play(char *fname, ...);

void bgm_stop();
void se_stop();

void bgm_pause();
void se_pause();

void bgm_end();
void se_end();
