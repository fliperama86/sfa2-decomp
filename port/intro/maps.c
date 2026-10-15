/* Native tile-map block writer, with the original plane-local wrapping. */
#include "world.h"
void intro_map_write(struct intro_world *w,unsigned layer,uint32_t source,uint32_t destination,unsigned visible)
{
    static const unsigned row_masks[]={127,63,31},wrap_masks[]={0xdf83,0xcfc3,0xc7e3};
    static const unsigned bank_steps[]={0x2000,0x1000,0x800},bank_masks[]={0x2000,0x3000,0x3800};
    static const unsigned column_masks[]={0xe000,0xf000,0xf800},column_steps[]={128,64,32},column_bits[]={0x1ffc,0xffc,0x7fc};
    const uint8_t *p;unsigned inner,outer,x,y,bias,blank;uint32_t at,start=destination;
    if(layer>2) {w->error="intro: map writer layer";return;}
    p=intro_world_table(w,INTRO_DATA,source,6);if(!p) return;inner=iw16(p+2)+1u;outer=iw16(p+4)+1u;
    if(inner>4096 || outer>4096 || inner*outer>4096) {w->error="intro: map block dimensions";return;}
    p=intro_world_table(w,INTRO_DATA,source+6,inner*outer*4u);if(!p) return;
    bias=layer==0 ? 0 : layer==1 ? 0xa400 : 0x900;blank=layer==2 ? 0 : 32;
    for(x=0;x<outer;x++) {
        at=start;
        for(y=0;y<inner;y++,p+=4) {
            uint8_t *dst=intro_world_video(w,at,4);if(!dst) return;
            ip16(dst,(visible ? iw16(p) : blank)+bias);ip16(dst+2,iw16(p+2));at+=4;
            if(!(at&row_masks[layer])) {
                at-=4;at=(at&0xffff0000u)|(at&wrap_masks[layer])|((at+bank_steps[layer])&bank_masks[layer]);
            }
        }
        start=(start&0xffff0000u)|(start&column_masks[layer])|((start+column_steps[layer])&column_bits[layer]);
    }
}
uint32_t intro_map_position(struct intro_world *w,unsigned layer,unsigned x,unsigned y)
{
    unsigned offset;
    if(layer==0) {y=~y;offset=((x&0x1f8u)<<4)+((y&0x100u)<<5)+((y&0xf8u)>>1);}
    else if(layer==1) {y=0u-y;offset=((x&0x3f0u)<<2)+((y&0x300u)<<4)+((y&0xf0u)>>2);}
    else if(layer==2) {y=0u-y;offset=(x&0x7e0u)+((y&0x700u)<<3)+((y&0xe0u)>>3);}
    else {w->error="intro: map position layer";return 0;}
    return (gw(w,0x18+2*layer)<<8)+offset;
}
