/* Native tile lettering and the blinking title prompt from supplied text data. */
#include "actors.h"
static uint32_t text_start(struct intro_world *w,unsigned index)
{
    const uint8_t *p;
    if(index>=128) {w->error="intro: unsupported text alphabet";return 0;}
    p=intro_world_table(w,INTRO_DATA,0xd99cu+index*2u,2);return p ? (uint32_t)(0xd99c+is16(iw16(p))) : 0;
}
static void draw_text(struct intro_world *w,unsigned index)
{
    uint32_t at=text_start(w,index),destination=0;unsigned palette=0,header=1,i;const uint8_t *p;
    if(!at) return;
    for(i=0;i<2048;i++) {
        if(header) {
            p=intro_world_table(w,INTRO_DATA,at,3);if(!p) return;at+=3;
            destination=(uint32_t)(0x900000+is16((unsigned)p[0]*128u+p[1]));palette=p[2]<128 ? p[2] : p[2]+0xff00u;header=0;
        }
        p=intro_world_table(w,INTRO_DATA,at++,1);if(!p) return;
        if(!*p) return;
        if(*p==127) {header=1;continue;}
        {uint8_t *dst=intro_world_video(w,destination,4);if(!dst) return;ip16(dst,*p);ip16(dst+2,palette);destination+=128;}
    }
    w->error="intro: unterminated tile text";
}
int intro_title_write(struct intro_world *w)
{
    const uint8_t *p=intro_world_table(w,INTRO_INLINE_DATA,0x2784u+gw(w,0x90),2);if(!p) return -1;
    draw_text(w,iw16(p));
    if(gw(w,0x90)) {ip32(w->video+0xc420,0);ip32(w->video+0xc424,0xffffffffu);ip32(w->video+0xc428,0xffffffffu);ip32(w->video+0xc42c,0xffffffffu);draw_text(w,83);}
    return w->error ? -1 : 0;
}
void intro_actor_ui(struct intro_world *w,uint8_t *a)
{
    unsigned count=0,i,header=1,x=0,y=0;uint32_t at;const uint8_t *p;
    if(a[IA_KIND]!=8) {w->error="intro: native UI actor kind missing";return;}
    if(a[IA_STATE]==0) {
        a[IA_STATE]+=2;a[IA_VISIBLE]=1;a[IA_LAYER]=0;ip32(a+IA_SCALE_X,0);ip16(a+IA_ANGLE,0);ip16(a+IA_X,0);ip16(a+IA_Y,256);
        a[IA_TIMER]=16;a[IA_TIMER+1]=1;ip16(a+IA_PRIORITY,0xe000);ip16(a+IA_BANK,0x2000);
        ip32(a+IA_SEQUENCE,0xffff0000u+(unsigned)(a-w->ram)+128u);ip16(a+0x80,4);at=text_start(w,a[IA_SUBTYPE]);if(!at) return;
        for(i=0;i<2048;i++) {
            if(header) {
                p=intro_world_table(w,INTRO_DATA,at,3);if(!p) return;at+=3;x=((unsigned)p[0]*8u)&1023u;y=((unsigned)p[1]*2u)&1023u;header=0;
            }
            p=intro_world_table(w,INTRO_DATA,at++,1);if(!p) return;
            if(!*p) break;
            if(*p==127) {header=1;continue;}
            if(*p!=32) {
                uint8_t *dst;
                if(count>=47) {w->error="intro: UI text block capacity";return;}
                dst=a+0x86+count*8;ip16(dst,*p);ip16(dst+2,27);ip16(dst+4,x);ip16(dst+6,y);count++;
            }
            x=(x+8u)&65535u;
        }
        if(i==2048) {w->error="intro: unterminated UI text";return;}
        ip16(a+0x82,count);ip16(a+0x84,count-1u);
    } else if(a[IA_STATE]==4 || a[IA_STATE]==6) {intro_world_delete(w,2,(uint16_t)(a-w->ram));return;}
    else if(a[IA_STATE]!=2) {w->error="intro: UI prompt state";return;}
    a[IA_TIMER]--;
    if(!a[IA_TIMER]) {a[IA_TIMER]=16;a[IA_TIMER+1]^=1;a[IA_VISIBLE]^=1;}
}
