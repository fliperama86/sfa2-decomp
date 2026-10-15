#include "../world.h"
#include <assert.h>
#include <string.h>
static struct intro_world w;
static void start(unsigned mode,unsigned target)
{memset(&w,0,sizeof w);*ig(&w,IG_FADE_ACTIVE)=255;*ig(&w,IG_FADE_WAIT)=255;w.fade_mode=mode;w.fade_target=target;}
int main(void)
{
    unsigned i,color;
    start(0xc01,0xffff);
    for(i=0;i<2048;i++) ip16(w.video+0xc000+2*i,0xf135);
    ip16(w.video+0xc400,0xe246);ip16(w.video+0xc402,0xf357);
    assert(!intro_fade_tick(&w));assert(*ig(&w,0x4f40)==13);
    assert(iw16(w.video+0xc400)==0xf246);assert(iw16(w.video+0xc402)==0x357);
    assert(*ig(&w,IG_FADE_ACTIVE)==255);
    assert(!intro_fade_tick(&w));assert(!*ig(&w,IG_FADE_ACTIVE));assert(*ig(&w,IG_FADE_WAIT)==255);
    assert(intro_fade_tick(&w)==1 && !*ig(&w,IG_FADE_WAIT));
    start(0xc01,0x1234);
    for(i=0;i<4;i++) ip16(w.video+0xc000+i*1024,(4-i)<<12);
    assert(!intro_fade_tick(&w));assert(*ig(&w,0x4f40)==15);
    start(1,0);ip16(w.video+0xc000,0x1345);ip16(w.video+0xc002,0x456);
    assert(!intro_fade_tick(&w));assert(iw16(w.video+0xc000)==0x345);assert(iw16(w.video+0xc002)==0xf456);
    start(0x1e03,1);ip16(w.video+0xc000,0x8e0f);assert(!intro_fade_tick(&w));assert(iw16(w.video+0xc000)==0x8f1f);
    assert(!intro_fade_tick(&w));assert(!intro_fade_tick(&w));assert(iw16(w.video+0xc000)==0x8f1f);
    assert(!intro_fade_tick(&w) && !*ig(&w,IG_FADE_ACTIVE));assert(w.fade_target==65535);
    /* Exhaust every packed color, independent channel saturation reference. */
    for(color=0;color<65536;color+=2048) {
        start(0x1e01,1);
        for(i=0;i<2048;i++) ip16(w.video+0xc000+2*i,color+i);
        assert(!intro_fade_tick(&w));
        for(i=0;i<2048;i++) {
            unsigned x=color+i,r=(x>>8)&15,g=(x>>4)&15,b=x&15;
            if(r<15) r++;
            if(g<15) g++;
            if(b<15) b++;
            assert(iw16(w.video+0xc000+2*i)==((x&0xf000)|(r<<8)|(g<<4)|b));
        }
    }
    start(0x1e00,1);assert(intro_fade_tick(&w)==-1 && w.error);
    return 0;
}
