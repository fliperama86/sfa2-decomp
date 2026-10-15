#include "../audio.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
int main(void)
{
    struct intro_audio a={0};uint8_t *raw=calloc(1,INTRO_AUDIO_RAW_SIZE);int16_t out[4096];unsigned i;size_t n,total=0;
    assert(raw);
    for(i=0;i<INTRO_AUDIO_SECTORS;i++) {
        uint8_t *s=raw+(size_t)i*2352;
        s[16]=s[20]=1;s[17]=s[21]=1;s[18]=s[22]=0x64;s[19]=s[23]=1;
    }
    assert(intro_audio_decode(&a,raw,INTRO_AUDIO_RAW_SIZE)==0);
    assert(a.frames==2248512u && a.gain==65536 && !a.position);
    while((n=intro_audio_pull(&a,out,2048))) {for(i=0;i<n*2;i++) assert(out[i]==0);total+=n;}
    assert(total==a.frames && a.position==a.frames);
    a.position=0;a.samples[0]=32767;a.samples[1]=-32768;a.gain=32768;
    assert(intro_audio_pull(&a,out,1)==1 && out[0]==16383 && out[1]==-16384);
    a.position=0;a.gain=65536;assert(intro_audio_pull(&a,out,1)==1 && out[1]==-32768);
    raw[19]=raw[23]=0;assert(intro_audio_decode(&a,raw,INTRO_AUDIO_RAW_SIZE)==-1 && a.frames==total);
    assert(intro_audio_decode(&a,raw,INTRO_AUDIO_RAW_SIZE-1)==-1);
    intro_audio_free(&a);assert(!a.samples && !a.frames);free(raw);return 0;
}
