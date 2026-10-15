#include "audio.h"
#include "../src/xa.h"
#include <stdlib.h>
#include <string.h>
int intro_audio_decode(struct intro_audio *out,const uint8_t *raw,size_t size)
{
    struct intro_audio next;struct xa_state state;unsigned i;
    int16_t decoded[XA_MAX_FRAMES*2],resampled[XA_MAX_FRAMES*6];
    size_t capacity=(size_t)INTRO_AUDIO_SECTORS*2352u;
    if(!out || !raw || size!=INTRO_AUDIO_RAW_SIZE) return -1;
    memset(&next,0,sizeof next);next.gain=65536;next.samples=malloc(capacity*2*sizeof *next.samples);
    if(!next.samples) return -1;
    xa_reset(&state);
    for(i=0;i<INTRO_AUDIO_SECTORS;i++) {
        struct xa_info info;unsigned n;
        if(xa_decode(&state,raw+(size_t)i*XA_SECTOR,decoded,&info) || info.file!=1 || info.channel!=1 ||
           !info.stereo || info.rate!=37800 || info.frames!=2016 || info.bits!=4) goto bad;
        n=xa_resample(&state,decoded,info.frames,(unsigned)info.rate,resampled);
        if(n>capacity-next.frames) goto bad;
        memcpy(next.samples+next.frames*2,resampled,(size_t)n*2*sizeof *resampled);next.frames+=n;
    }
    intro_audio_free(out);*out=next;return 0;
bad:
    free(next.samples);return -1;
}
void intro_audio_free(struct intro_audio *a)
{if(a) {free(a->samples);memset(a,0,sizeof *a);}}
size_t intro_audio_pull(struct intro_audio *a,int16_t *out,size_t frames)
{
    size_t n,i;
    if(!a || !out || !a->samples || a->position>a->frames || a->gain>65536) return 0;
    n=a->frames-a->position;if(n>frames) n=frames;
    for(i=0;i<n*2;i++) out[i]=(int16_t)((int32_t)a->samples[a->position*2+i]*(int32_t)a->gain/65536);
    a->position+=n;return n;
}
