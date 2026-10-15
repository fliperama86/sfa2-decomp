/* Native actors for the vertical montage and its scene transitions. */
#include "actors.h"
#include <string.h>
static const uint8_t *operand(struct intro_world *w,uint32_t address,size_t n)
{return intro_world_table(w,INTRO_INLINE_DATA,address,n);}
static void finish(struct intro_world *w,uint8_t *a)
{intro_world_delete(w,0,(uint16_t)(a-w->ram));}
static void style(uint8_t *a,unsigned layer,unsigned palette,unsigned camera)
{
    a[IA_LAYER]=(uint8_t)layer;a[IA_BYPASS]=a[IA_SHAKE]=0;a[IA_CAMERA]=(uint8_t)camera;
    ip16(a+IA_PALETTE_MODE,palette);ip16(a+IA_PRIORITY,0xe000);ip16(a+IA_BANK,0x2000);
}
static void rise(struct intro_world *w,uint8_t *a)
{
    const uint8_t *p;uint32_t list;unsigned i,j,color,offset;int32_t relative;
    switch(a[IA_STATE]) {
    case 0:
        if(a[IA_SUBTYPE]>2) {w->error="intro: rising montage subtype";return;}
        a[IA_STATE]+=2;style(a,8,0,8);ip32(a+0x50,0);
        p=operand(w,0x785fau+a[IA_SUBTYPE]*8u,7);if(!p) return;
        ip16(a+IA_X,iw16(p));ip16(a+IA_Y,iw16(p+2));ip16(a+IA_VY,iw16(p+4));a[IA_FLIP]=p[6];
        intro_actor_select(w,a,0x786b6,a[IA_SUBTYPE]);break;
    case 2:
        relative=is16(iw16(a+IA_Y)-gw(w,0x294));
        if(relative<=is16(iw16(a+IA_VY))) {color=(iw16(a+0x50)+8u)&65535u;ip16(a+0x50,color>240u ? 240u : color);}
        if(relative<0) a[IA_STATE]+=2;
        if(!*ig(w,IG_FADE_WAIT)) {
            color=iw16(a+0x50)&240u;
            if(color!=iw16(a+0x52)) {
                ip16(a+0x52,color);p=operand(w,0x78694u+a[IA_SUBTYPE]*2u,2);if(!p) return;list=(uint32_t)(0x78694+is16(iw16(p)));
                for(i=0;i<32;i++,list+=2) {
                    uint8_t *dst;p=intro_world_table(w,INTRO_DATA,list,2);if(!p) return;offset=iw16(p);if(offset&32768u) break;
                    dst=intro_world_video(w,0x90c000u+offset,32);if(!dst) return;
                    for(j=0;j<16;j++) ip16(dst+2*j,(iw16(dst+2*j)&4095u)|(color<<8));
                }
                if(i==32) {w->error="intro: montage brightness palette list";return;}
            }
        }
        break;
    case 4:case 6:finish(w,a);return;
    default:w->error="intro: rising montage state";return;
    }
    intro_actor_collect(w,a,1);
}
static void burst(struct intro_world *w,uint8_t *a)
{
    const uint8_t *p;unsigned r,position,speed,sequence,offset;
    switch(a[IA_STATE]) {
    case 0:
        if(a[IA_SUBTYPE]>1) {w->error="intro: burst subtype";return;}
        a[IA_STATE]+=2;
        if(!a[IA_SUBTYPE]) {
            style(a,2,0x8007,0);a[IA_TIMER]=5;a[IA_DELAY]=0;ip32(a+IA_X,0x580000);ip32(a+IA_Y,0x800000);
            ip32(a+IA_VX,0xc000);ip32(a+IA_VY,0x4000);ip32(a+IA_AX,0);ip32(a+IA_AY,0);
            intro_actor_select(w,a,0x229120,20);return;
        }
        style(a,0,0x10a,0);ip32(a+IA_X,0xf00000);ip32(a+IA_Y,0x800000);ip32(a+IA_AX,0);ip32(a+IA_AY,0xfffffc00u);
        position=intro_world_random(w);speed=intro_world_random(w);r=intro_world_random(w);a[IA_FLIP]=(uint8_t)((r&16u)>>4);
        p=operand(w,0x78a12u+(r&15u),1);if(!p) return;sequence=*p;
        p=operand(w,0x78a22u+(position&15u)*2u,2);if(!p) return;ip16(a+IA_X,iw16(a+IA_X)+iw16(p));
        p=operand(w,0x78a22u+((position&240u)>>3),2);if(!p) return;ip16(a+IA_Y,iw16(a+IA_Y)+iw16(p));
        p=operand(w,0x78a42u+(speed&15u)*4u,4);if(!p) return;ip32(a+IA_VX,iw32(p));
        p=operand(w,0x78a42u+((speed&240u)>>2),4);if(!p) return;ip32(a+IA_VY,iw32(p));
        intro_actor_select(w,a,0x229120,sequence);return;
    case 2:
        ip32(a+IA_X,iw32(a+IA_X)+iw32(a+IA_VX));ip32(a+IA_VX,iw32(a+IA_VX)+iw32(a+IA_AX));
        ip32(a+IA_Y,iw32(a+IA_Y)+iw32(a+IA_VY));ip32(a+IA_VY,iw32(a+IA_VY)+iw32(a+IA_AY));
        if(a[IA_SUBTYPE]) {
            if(((iw16(a+IA_X)+64u)&65535u)>512u || ((iw16(a+IA_Y)+64u)&65535u)>384u) a[IA_STATE]=6;
        } else {
            a[IA_TIMER]--;
            if(!a[IA_TIMER]) {
                a[IA_TIMER]=5;p=operand(w,0x78b30u+a[IA_DELAY]*2u,2);if(!p) return;offset=iw16(p);
                if(offset&32768u) {a[IA_STATE]+=2;sw(w,IG_INTERVAL,iw16(a+IA_KIND));offset&=32767u;}
                a[IA_DELAY]++;intro_world_palette_copy(w,0x2ab6e0u+offset,0x90cfe0,16,-1);
            }
        }
        intro_actor_tick(w,a);intro_actor_collect(w,a,1);return;
    case 4:case 6:finish(w,a);return;
    default:w->error="intro: burst actor state";
    }
}
static void panorama(struct intro_world *w,uint8_t *a)
{
    unsigned type=a[IA_SUBTYPE],index;uint32_t table;const uint8_t *p;
    switch(a[IA_STATE]) {
    case 0:
        if(type>3) {w->error="intro: panorama subtype";return;}
        a[IA_STATE]+=2;a[IA_BYPASS]=a[IA_CAMERA]=a[IA_SHAKE]=0;
        ip16(a+IA_PALETTE_MODE,type<2 ? 0x10bu+type : 0);ip16(a+IA_BANK,type<2 ? 0 : 0x2000);
        p=operand(w,0x78c48u+type*16u,16);if(!p) return;table=iw32(p);index=p[15];
        ip16(a+IA_X,iw16(p+4));ip16(a+IA_Y,iw16(p+6));ip16(a+IA_PRIORITY,iw16(p+8));a[IA_LAYER]=p[12];a[IA_FLIP]=p[13];
        if(type<2) intro_actor_select24(w,a,table,index);
        else {intro_actor_select(w,a,table,index);ip32(a+0x60,iw32(a+IA_SEQUENCE));}
        break;
    case 2:
        if(type<2) intro_actor_tick24(w,a);
        else ip32(a+IA_SEQUENCE,iw32(a+0x60)+gw(w,0x2b4)*16u);
        break;
    case 4:case 6:finish(w,a);return;
    default:w->error="intro: panorama state";return;
    }
    intro_actor_collect(w,a,1);
}
static void curtain(struct intro_world *w,uint8_t *a)
{
    const uint8_t *p;unsigned type=a[IA_SUBTYPE];
    switch(a[IA_STATE]) {
    case 0:
        if(type>1) {w->error="intro: curtain subtype";return;}
        a[IA_STATE]+=2;style(a,0,0x800d,0);
        p=operand(w,0x78de8u+type*16u,16);if(!p) return;
        ip32(a+IA_X,iw32(p));ip32(a+IA_Y,iw32(p+4));ip32(a+IA_VY,iw32(p+8));ip32(a+IA_AY,iw32(p+12));
        intro_actor_select(w,a,0x229120,30+type);return;
    case 2:
        ip32(a+IA_VY,iw32(a+IA_VY)+iw32(a+IA_AY));ip32(a+IA_Y,iw32(a+IA_Y)+iw32(a+IA_VY));
        if(iw16(a+IA_Y)==240) {a[IA_STATE]+=2;ip32(a+IA_Y,0xf00000);*ig(w,IG_INTERVAL+type)=a[IA_KIND];}
        break;
    case 4:break;
    case 6:finish(w,a);return;
    default:w->error="intro: curtain state";return;
    }
    intro_actor_collect(w,a,1);
}
static void flash_portrait(struct intro_world *w,uint8_t *a)
{
    const uint8_t *p;uint16_t id;uint8_t *c;
    switch(a[IA_STATE]) {
    case 0:
        if(a[IA_SUBTYPE]>4) {w->error="intro: flash portrait subtype";return;}
        a[IA_STATE]+=2;id=intro_world_alloc(w,1);
        if(id) {c=w->ram+id;c[IA_ACTIVE]=1;ip16(c+IA_KIND,0x2202);ip16(c+IA_LINK,(unsigned)(a-w->ram));ip16(a+0x2c,id);}
        style(a,0,0,0);a[IA_BYPASS]=1;a[IA_FLIP]=0;
        p=operand(w,0x7a6e6u+a[IA_SUBTYPE]*16u,16);if(!p) return;
        ip16(a+0x50,iw16(p));ip16(a+IA_Y,iw16(p+2));ip32(a+IA_X,iw32(p+4));ip32(a+IA_VX,iw32(p+8));ip32(a+IA_AX,iw32(p+12));
        intro_actor_select(w,a,0x7a736,a[IA_SUBTYPE]);break;
    case 2:
        ip32(a+IA_X,iw32(a+IA_X)+iw32(a+IA_VX));ip32(a+IA_VX,iw32(a+IA_VX)+iw32(a+IA_AX));
        if(iw16(a+0x50)==iw16(a+IA_X)) a[IA_STATE]+=2;
        break;
    case 4:*ig(w,IG_TIMER+1)=0;/* fall through */
    case 6:finish(w,a);return;
    default:w->error="intro: flash portrait state";return;
    }
    intro_actor_collect(w,a,0);
}
void intro_actor_montage(struct intro_world *w,uint8_t *a)
{
    switch(a[IA_KIND]) {
    case 0x87:rise(w,a);return;
    case 0x88:burst(w,a);return;
    case 0x89:panorama(w,a);return;
    case 0x8a:curtain(w,a);return;
    case 0x8e:flash_portrait(w,a);return;
    default:w->error="intro: montage actor kind";
    }
}
