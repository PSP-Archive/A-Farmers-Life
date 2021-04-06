// modplayer.h: headers for psp modplayer code
//
// All public functions for modplayer
//
//////////////////////////////////////////////////////////////////////
#ifndef _OGGPLAYER_H_
#define _OGGPLAYER_H_

#ifdef __cplusplus
extern "C" {
#endif

// private functions
    void OGG_Init(int channel);
    int OGG_Play();
    void OGG_Pause();
    int OGG_Stop();
    void OGG_End();
    int OGG_IsPlaying();
    
    void OGG_SetVolume(float volume);
    float OGG_GetVolume();
    
    int OGG_LoadMemory(const void *buffer, int bufsize);
    int OGG_Load(char *filename);
    void OGG_FreeTune();
    void OGG_GetTimeString(char *dest);
    int OGG_EndOfStream();


#ifdef __cplusplus
}
#endif
#endif
