/* Palette fades recovered from the three helpers used by the intro. */
#include "world.h"

int intro_fade_tick(struct intro_world *w)
{
    unsigned mode,spacing,layer,i,complete=0;
    if(!w || w->error) return -1;
    if(!*ig(w,IG_FADE_WAIT)) return 0;
    if(w->fade_delay) {w->fade_delay--;return 0;}
    if(!*ig(w,IG_FADE_ACTIVE)) {*ig(w,IG_FADE_WAIT)=0;return 1;}
    mode=w->fade_mode>>8;spacing=w->fade_mode&255u;
    if(!spacing || (mode!=0 && mode!=12 && mode!=30)) {w->error="intro: unsupported palette fade mode";return -1;}
    if(mode==30) {
        /* RGB-to-white counts down before writing and retains brightness. */
        w->fade_target=(w->fade_target-1u)&65535u;
        if(w->fade_target&32768u) *ig(w,IG_FADE_ACTIVE)=0;
        else for(i=0;i<2048;i++) {
            uint8_t *p=w->video+0xc000+2*i;unsigned color=iw16(p),shift;
            for(shift=0;shift<12;shift+=4) if(((color>>shift)&15u)!=15u) color+=1u<<shift;
            ip16(p,color);
        }
    } else {
        *ig(w,0x4f40)=0;
        for(layer=0;layer<4;layer++) {
            uint8_t *p=w->video+0xc000+layer*1024u;
            unsigned target=((w->fade_target>>(layer*4u))&15u)<<12;
            /* The first color gates the whole bank, not each color separately. */
            if((iw16(p)&0xf000u)==target) complete|=1u<<layer;
            else for(i=0;i<512;i++) ip16(p+2*i,iw16(p+2*i)+(mode==12 ? 0x1000u : 0xf000u));
        }
        *ig(w,0x4f40)=(uint8_t)complete;
        if(complete==15) *ig(w,IG_FADE_ACTIVE)=0;
    }
    w->fade_delay=spacing-1u;
    return 0;
}
