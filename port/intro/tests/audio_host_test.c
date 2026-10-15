/* Synthetic media, hash dependency is explicit: this tests extraction/lifetime,
 * not SHA-256 or the actual game's asset identity. */
#include "../../src/intro_audio.h"
#include "../../src/cd.h"
#include "../audio.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
static struct port_disc disc;
static int depth,starts,bad_hash=1;
size_t (*port_cd_audio_override)(short *,size_t);
struct port_disc *port_cd_disc(void) {return &disc;}
void port_sha256(const unsigned char *data,size_t n,unsigned char out[32])
{
    static const unsigned char expected[]={0xf9,0x52,0xbd,0x2c,0xa0,0xb1,0xb5,0xbf,0xda,0x31,0x60,0x5a,0xb8,0x0d,0xd5,0x8c,0x4b,0x92,0x12,0x0b,0x3c,0x83,0x6e,0x40,0x42,0xf4,0xf8,0x8a,0x8b,0x67,0xd4,0x1c};
    unsigned i;assert(n==INTRO_AUDIO_RAW_SIZE);
    for(i=0;i<956;i++) assert(data[i*2352+16]==1 && data[i*2352+17]==1);
    memcpy(out,expected,32);if(bad_hash) out[0]^=1;
}
void port_audio_lock(void) {assert(!depth);depth=1;}
void port_audio_unlock(void) {assert(depth);depth=0;}
int port_CdInit(void) {starts++;return 1;}
static void little(unsigned char *p,unsigned v)
{p[0]=(unsigned char)v;p[1]=(unsigned char)(v>>8);p[2]=(unsigned char)(v>>16);p[3]=(unsigned char)(v>>24);}
static void record(unsigned char *p,const char *name,unsigned extent,unsigned bytes,int directory)
{unsigned n=(unsigned)strlen(name);p[0]=(unsigned char)(34+n);little(p+2,extent);little(p+10,bytes);p[25]=(unsigned char)(directory ? 2 : 0);p[32]=(unsigned char)n;memcpy(p+33,name,n);}
int main(int argc,char **argv)
{
    FILE *f;unsigned i;unsigned char raw[2352]={0};short samples[2];char error[256];
    assert(argc==2 && strlen(argv[1])<sizeof disc.path);strcpy(disc.path,argv[1]);
    disc.length=(10ull+8250)*2352;raw[16]=raw[20]=1;raw[17]=raw[21]=1;raw[18]=raw[22]=0x64;raw[19]=raw[23]=1;
    f=fopen(disc.path,"wb");assert(f);
    for(i=0;i<956;i++) {assert(!fseek(f,(long)((10+558+8*i)*2352),SEEK_SET));assert(fwrite(raw,1,sizeof raw,f)==sizeof raw);}
    assert(!fseek(f,(long)disc.length-1,SEEK_SET));assert(fputc(0,f)!=EOF);
    memset(raw,0,sizeof raw);raw[24]=1;memcpy(raw+25,"CD001",5);record(raw+24+156,"X",18,2048,1);
    assert(!fseek(f,16*2352,SEEK_SET) && fwrite(raw,1,sizeof raw,f)==sizeof raw);
    memset(raw,0,sizeof raw);record(raw+24,"PAC",19,2048,1);
    assert(!fseek(f,18*2352,SEEK_SET) && fwrite(raw,1,sizeof raw,f)==sizeof raw);
    memset(raw,0,sizeof raw);record(raw+24,"Z2O.STR;1",10,16896000,0);
    assert(!fseek(f,19*2352,SEEK_SET) && fwrite(raw,1,sizeof raw,f)==sizeof raw);
    assert(!fclose(f));disc.f=fopen(disc.path,"rb");assert(disc.f);
    assert(port_intro_audio_prepare(error,sizeof error)==-1 && strstr(error,"unsupported soundtrack"));
    bad_hash=0;assert(!port_intro_audio_prepare(error,sizeof error));
    assert(!port_intro_audio_prepare(error,sizeof error)); /* Cache, no disc re-read. */
    port_intro_audio_start();assert(starts==1 && port_cd_audio_override && !depth);
    port_audio_lock();assert(port_cd_audio_override(samples,1)==1 && samples[0]==0);port_audio_unlock();
    port_intro_audio_gain(0);port_intro_audio_stop();assert(!port_cd_audio_override && !depth);
    port_intro_audio_start();assert(starts==2 && port_cd_audio_override);port_intro_audio_stop();assert(!fclose(disc.f));
    return 0;
}
