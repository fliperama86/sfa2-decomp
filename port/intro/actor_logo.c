/* Native title actors for the supplied arcade variant's opening and finale. */
#include "actors.h"
#include <string.h>
static void logo_record(struct intro_world *w,uint8_t *a,unsigned index,unsigned frame)
{
    const uint8_t *p=intro_world_table(w,INTRO_DATA,0x7528au+(index&255u)*2u,2);uint32_t address;if(!p) return;
    address=(uint32_t)(0x7528a+is16(iw16(p)+(frame&255u)*16u));p=intro_world_table(w,INTRO_DATA,address,16);if(!p) return;
    ip32(a+IA_SEQUENCE,address);memcpy(a+IA_TICKS,p,4);
}
static void logo_style(uint8_t *a,unsigned bypass,unsigned priority)
{
    a[IA_LAYER]=2;a[IA_BYPASS]=(uint8_t)bypass;a[IA_FLIP]=a[IA_CAMERA]=a[IA_SHAKE]=0;
    ip16(a+IA_PALETTE_MODE,0);ip16(a+IA_PRIORITY,priority);ip16(a+IA_BANK,0x2000);
}
static void logo_palette_step(struct intro_world *w,uint8_t *a,int begin)
{
    const uint8_t *p;uint32_t address;
    if(begin) address=0x74f28;
    else {
        ip16(a+IA_TICKS,iw16(a+IA_TICKS)-1u);if(iw16(a+IA_TICKS) || (a[IA_FLAGS]&128u)) return;
        address=iw32(a+0x60)+6u;
    }
    p=intro_world_table(w,INTRO_DATA,address,6);if(!p) return;ip32(a+0x60,address);memcpy(a+IA_TICKS,p,4);
    intro_world_palette_copy(w,(uint32_t)(0x29c100+is16(iw16(p+4))),0x90c220,16,-1);
}
static void logo_spark_palette(struct intro_world *w,uint8_t *a)
{
    const uint8_t *p=intro_world_table(w,INTRO_INLINE_DATA,0x75010u+(a[IA_FLAGS+1]&127u)*4u,4);if(!p) return;
    intro_world_palette_copy(w,(uint32_t)(0x29c100+is16(iw16(p))),0x90c280,16,-1);
    intro_world_palette_copy(w,(uint32_t)(0x29c100+is16(iw16(p+2))),0x90c2a0,16,-1);
}
static int logo_move(uint8_t *a,int move_x,int rotate)
{
    unsigned busy=0,value;
    if(move_x && iw16(a+0x54)!=iw16(a+IA_X)) {busy=1;ip32(a+IA_X,iw32(a+IA_X)+iw32(a+IA_VX));}
    if(iw16(a+0x56)!=iw16(a+IA_Y)) {busy=1;ip32(a+IA_Y,iw32(a+IA_Y)+iw32(a+IA_VY));}
    value=iw16(a+IA_SCALE_X);
    if(value) {busy=1;value+=iw16(a+0x50);ip16(a+IA_SCALE_X,value);ip16(a+IA_SCALE_Y,value);}
    value=iw16(a+IA_ANGLE);
    if(rotate && value) {busy=1;ip16(a+IA_ANGLE,(value+iw16(a+0x52))&240u);}
    return !busy;
}
void intro_actor_logo(struct intro_world *w,uint8_t *a)
{
    unsigned variant=a[IA_VARIANT],type=a[IA_SUBTYPE],state=a[IA_STATE],value;uint16_t id;uint8_t *spark;
    if(*ig(w,0x89) || (variant!=1 && variant!=4) || type>3 || (variant==4 && type==3)) {w->error="intro: unsupported regional logo actor";return;}
    if(state==6) {intro_world_delete(w,0,(uint16_t)(a-w->ram));return;}
    if(variant==4) {
        if(state==0) {
            a[IA_STATE]+=2;logo_style(a,1,0xe000);
            ip16(a+IA_X,type==0 ? 102 : type==1 ? 255 : 192);ip16(a+IA_Y,type==0 ? 157 : type==1 ? 176 : 112);
            logo_record(w,a,type,type==2 ? 3 : 1);return;
        }
        if(state==2) intro_actor_collect(w,a,0);
        else if(state==4) intro_world_delete(w,0,(uint16_t)(a-w->ram));
        else w->error="intro: opening logo state";
        return;
    }
    if(state==0) {
        a[IA_STATE]+=2;logo_style(a,type==3,0xa000);
        if(type==3) {
            ip16(a+IA_X,192);ip16(a+IA_Y,72);*ig(w,IG_FLAGS+3)=a[IA_KIND];logo_record(w,a,4,3);logo_palette_step(w,a,1);return;
        }
        ip32(a+IA_X,0xc00000);ip32(a+IA_Y,0x800000);ip16(a+IA_SCALE_X,0xfff0);ip16(a+IA_SCALE_Y,0xfff0);
        ip16(a+IA_ANGLE,type==2 ? 0 : 1);ip16(a+0x50,1);
        if(type==0) {
            ip32(a+IA_VX,0xfffa6000u);ip32(a+IA_VY,0x1d000);ip16(a+0x54,102);ip16(a+0x56,157);ip16(a+0x52,0xfff0);a[IA_TIMER]=8;
        } else if(type==1) {
            ip32(a+IA_VX,0x3f000);ip32(a+IA_VY,0x30000);ip16(a+0x54,255);ip16(a+0x56,176);ip16(a+0x52,16);
        } else {ip32(a+IA_VY,0xffff0000u);ip16(a+0x56,112);}
        logo_record(w,a,type,0);return;
    }
    if(type<2) {
        if(state==2) {
            if(!type && a[IA_TIMER]) {a[IA_TIMER]--;return;}
            if(type==1 && *ig(w,IG_FLAGS)!=a[IA_KIND]) return;
            if(logo_move(a,1,1)) {a[IA_STATE]+=2;*ig(w,IG_FLAGS+type)=a[IA_KIND];ip32(a+IA_SEQUENCE,iw32(a+IA_SEQUENCE)+16);}
        } else if(state!=4) {w->error="intro: rotating logo state";return;}
        intro_actor_collect(w,a,0);return;
    }
    if(type==2) {
        if(state==2) switch(a[IA_STEP]) {
        case 0:
            if(*ig(w,IG_FLAGS+1)!=a[IA_KIND]) return;
            if(logo_move(a,0,0)) {a[IA_STEP]+=2;a[IA_TIMER]=16;}
            break;
        case 2:
            a[IA_TIMER]--;if(!a[IA_TIMER]) {a[IA_STEP]+=2;*ig(w,IG_LINK)=a[IA_KIND];ip32(a+IA_SEQUENCE,iw32(a+IA_SEQUENCE)+16);}
            break;
        case 4:if(!gl(w,IG_FLAGS)) a[IA_STEP]+=2;break;
        case 6:
            if(*ig(w,IG_FLAGS+3)==a[IA_KIND]) {a[IA_STATE]+=2;a[IA_STEP]=0;ip32(a+IA_SEQUENCE,iw32(a+IA_SEQUENCE)+32);*ig(w,IG_LINK)=a[IA_KIND];}
            break;
        default:w->error="intro: subtitle movement state";return;
        } else if(state!=4) {w->error="intro: subtitle actor state";return;}
        intro_actor_collect(w,a,0);return;
    }
    if(state==2) {
        if(gl(w,IG_FLAGS)) return;
        if(a[IA_FLAGS]&128u) {a[IA_STATE]+=2;a[IA_STEP]=0;*ig(w,IG_FLAGS+3)=a[IA_KIND];}
        else logo_palette_step(w,a,0);
    } else if(state==4) {
        switch(a[IA_STEP]) {
        case 0:a[IA_STEP]+=2;ip16(a+IA_Y,112);intro_actor_select(w,a,0x229120,45);break;
        case 2:
            a[IA_STEP]+=2;logo_spark_palette(w,a);intro_world_palette_copy(w,0x2a1aa0,0x90c220,16,-1);break;
        case 4:
            intro_actor_tick(w,a);logo_spark_palette(w,a);
            if(iw16(a+IA_TICKS)==1 && !(a[IA_FLAGS+1]&128u)) {
                value=a[IA_FLAGS+1];
                if(!(intro_world_random(w)&8u)) {
                    id=intro_world_alloc(w,0);if(id) {spark=w->ram+id;spark[IA_ACTIVE]++;ip16(spark+IA_KIND,0x8b09);spark[IA_DELAY]=(uint8_t)value;}
                }
            }
            break;
        default:w->error="intro: title spark state";return;
        }
    } else {w->error="intro: final title actor state";return;}
    intro_actor_collect(w,a,0);
}
