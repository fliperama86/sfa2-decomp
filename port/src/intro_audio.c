/* Use the user's PS1 opening soundtrack, not captured arcade emulator output.
 * Its Capcom-logo lead-in is outside the selected XA stream. The native intro
 * owns the picture/timing; this adaptation does not claim exact QSound audio. */
#include "intro_audio.h"
#include "../intro/audio.h"
#include "cd.h"
#include <stdlib.h>
#include <string.h>
static struct intro_audio music;
static size_t pull(short *out,size_t frames) {return intro_audio_pull(&music,out,frames);}
static int fail(char *error,size_t size,const char *why)
{if(error && size) snprintf(error,size,"intro soundtrack: %s",why);return -1;}
int port_intro_audio_prepare(char *error,size_t size)
{
    static const uint8_t expected[32]={
        0xf9,0x52,0xbd,0x2c,0xa0,0xb1,0xb5,0xbf,0xda,0x31,0x60,0x5a,0xb8,0x0d,0xd5,0x8c,
        0x4b,0x92,0x12,0x0b,0x3c,0x83,0x6e,0x40,0x42,0xf4,0xf8,0x8a,0x8b,0x67,0xd4,0x1c
    };
    struct port_disc *disc=port_cd_disc();unsigned sector,bytes,i;uint8_t *raw,digest[32];FILE *file;int bad=0;
    if(music.samples) return 0;
    if(!disc || port_disc_find(disc,"Z2O.STR",&sector,&bytes,error,size)) return -1;
    if(bytes!=16896000u) return fail(error,size,"unsupported PS1 opening stream");
    raw=malloc(INTRO_AUDIO_RAW_SIZE);if(!raw) return fail(error,size,"out of memory");
    /* A separate read-only handle never changes the running drive's cursor. */
    file=fopen(disc->path,"rb");if(!file) {free(raw);return fail(error,size,"cannot open supplied disc");}
    for(i=0;i<INTRO_AUDIO_SECTORS;i++) {
        unsigned long long offset=((unsigned long long)sector+558u+8u*i)*PORT_SECTOR;
        if(offset+PORT_SECTOR>disc->length || offset>0x7fffffffu || fseek(file,(long)offset,SEEK_SET) ||
           fread(raw+(size_t)i*PORT_SECTOR,1,PORT_SECTOR,file)!=PORT_SECTOR) {bad=1;break;}
    }
    if(fclose(file)) bad=1;
    if(bad) {free(raw);return fail(error,size,"cannot read soundtrack sectors");}
    port_sha256(raw,INTRO_AUDIO_RAW_SIZE,digest);
    if(memcmp(digest,expected,sizeof digest)) {free(raw);return fail(error,size,"unsupported soundtrack data");}
    bad=intro_audio_decode(&music,raw,INTRO_AUDIO_RAW_SIZE);free(raw);
    return bad ? fail(error,size,"invalid XA soundtrack") : 0;
}
void port_intro_audio_start(void)
{
    /* Same sound-chip setup as the replaced movie's library initialization. */
    port_CdInit();port_audio_lock();music.position=0;music.gain=65536;
    port_cd_audio_override=pull;port_audio_unlock();
}
void port_intro_audio_gain(unsigned gain)
{port_audio_lock();music.gain=gain>65536 ? 65536 : gain;port_audio_unlock();}
void port_intro_audio_stop(void)
{port_audio_lock();port_cd_audio_override=NULL;music.position=music.frames;port_audio_unlock();}
