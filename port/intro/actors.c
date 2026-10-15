/* Recovered native actor state machines. Inline operands and indirect resource
 * reads remain distinct; neither resource class is executable at runtime. */
#include "actors.h"
#include "particles.h"
#include <string.h>

static int record(struct intro_world *w,uint8_t *a,uint32_t address)
{
    const uint8_t *p=intro_world_table(w,INTRO_DATA,address,16);if(!p) return -1;
    ip32(a+IA_SEQUENCE,address);memcpy(a+IA_TICKS,p,4);return 0;
}
int intro_actor_select(struct intro_world *w,uint8_t *a,uint32_t table,unsigned index)
{
    const uint8_t *p=intro_world_table(w,INTRO_DATA,table+(index&255u)*2u,2);if(!p) return -1;
    return record(w,a,(uint32_t)(table+is16(iw16(p))));
}
int intro_actor_tick(struct intro_world *w,uint8_t *a)
{
    uint32_t next;const uint8_t *p;
    ip16(a+IA_TICKS,iw16(a+IA_TICKS)-1u);if(iw16(a+IA_TICKS)) return 0;
    next=iw32(a+IA_SEQUENCE)+16u;
    if(a[IA_FLAGS]&128u) {p=intro_world_table(w,INTRO_DATA,next,4);if(!p) return -1;next=iw32(p);}
    return record(w,a,next);
}
void intro_actor_collect(struct intro_world *w,uint8_t *a,int cull)
{
    unsigned camera,x,list,top;const uint8_t *p;
    if(w->error) return;
    if(cull) {
        camera=a[IA_CAMERA];
        if(camera==0) {p=intro_world_table(w,INTRO_DATA,0x40,2);if(!p) return;x=iw16(p);}
        else if(camera==4 || camera==8 || camera==12) x=gw(w,0x210+(camera-4)*32);
        else {w->error="intro: actor camera selector";return;}
        if(((iw16(a+IA_X)-x+64u)&65535u)>512u) {a[IA_VISIBLE]=0;return;}
    }
    a[IA_VISIBLE]=1;
    if(a[IA_CLASS]==0 || a[IA_CLASS]==4 || a[IA_CLASS]==12) return;
    if(a[IA_CLASS]!=16) {w->error="intro: unsupported actor draw class";return;}
    if(*ig(w,0x124) && !a[0x30]) return;
    if((a[IA_LAYER]&1u) || a[IA_LAYER]>14) {w->error="intro: actor draw layer";return;}
    list=a[IA_LAYER]/2+4;top=gw(w,0x60fc+2*list);
    if(top<0xe286 || top>0xe546 || *ig(w,0x611a+list)>=44) {w->error="intro: actor draw list capacity";return;}
    ip16(w->ram+top-2,(unsigned)(a-w->ram));sw(w,0x60fc+2*list,top-2);(*ig(w,0x611a+list))++;
}
static void done(struct intro_world *w,uint8_t *a) {intro_world_delete(w,0,(uint16_t)(a-w->ram));}
static void tick_collect(struct intro_world *w,uint8_t *a,int cull)
{if(!intro_actor_tick(w,a)) intro_actor_collect(w,a,cull);}
static void flame(struct intro_world *w,uint8_t *a)
{
    unsigned kind=a[IA_SUBTYPE],offset;const uint8_t *p;
    switch(a[IA_STATE]) {
    case 0:
        a[IA_STATE]+=2;a[IA_FLIP]=a[IA_SHAKE]=0;ip16(a+IA_BANK,0x2000);ip16(a+IA_PALETTE_MODE,0);
        ip16(a+IA_X+2,0);ip16(a+IA_Y+2,0);
        if(!kind) {ip16(a+IA_X,192);ip16(a+IA_Y,256);}
        else {
            if(kind>6) {w->error="intro: flame subtype";return;}
            ip32(a+IA_AX,0);ip32(a+IA_AY,0xffffc000u);ip32(a+0x60,0x24000);
            offset=(kind-1u)*16u+(intro_world_random(w)&8u);
            p=intro_world_table(w,INTRO_INLINE_DATA,0x73d9au+offset,8);if(!p) return;
            memcpy(a+IA_VX,p,8);offset=(intro_world_random(w)&127u)*4u;
            p=intro_world_table(w,INTRO_INLINE_DATA,0x73e10u+offset,4);if(!p) return;
            ip16(a+IA_X,iw16(p));ip16(a+IA_Y,iw16(p+2));
        }
        if(!intro_actor_select(w,a,0x229120,kind)) intro_actor_collect(w,a,1);
        return;
    case 2:
        if(kind) {
            struct intro_particle_motion m;
            m.x=iw32(a+IA_X);m.y=iw32(a+IA_Y);m.vx=iw32(a+IA_VX);m.vy=iw32(a+IA_VY);
            m.sway=iw32(a+IA_AX);m.acceleration=iw32(a+IA_AY);m.limit=iw32(a+0x60);
            intro_particle_move(&m);
            ip32(a+IA_X,m.x);ip32(a+IA_Y,m.y);ip32(a+IA_AX,m.sway);ip32(a+IA_AY,m.acceleration);
        } else {
            /* The stationary leader still performs the original sway update. */
            uint32_t acceleration=iw32(a+IA_AY);
            if(2u>=iw32(a+0x60)) {acceleration=0u-acceleration;ip32(a+IA_AY,acceleration);}
            ip32(a+IA_AX,iw32(a+IA_AX)+acceleration);
        }
        if(intro_actor_tick(w,a)) return;
        if(((iw16(a+IA_X)+32u)&65535u)>448u || ((iw16(a+IA_Y)+32u)&65535u)>288u) a[IA_STATE]=6;
        intro_actor_collect(w,a,1);
        return;
    case 4:case 6:done(w,a);return;
    default:w->error="intro: flame actor state";
    }
}
static void strip(struct intro_world *w,uint8_t *a)
{
    const uint8_t *p;
    switch(a[IA_STATE]) {
    case 0:
        if(a[IA_SUBTYPE]>3) {w->error="intro: strip subtype";return;}
        a[IA_STATE]+=2;ip16(a+IA_PALETTE_MODE,0);a[IA_CAMERA]=a[IA_SHAKE]=0;
        ip16(a+IA_PRIORITY,0xe000);ip16(a+IA_BANK,0x2000);ip32(a+IA_SCALE_X,0);ip16(a+IA_ANGLE,0);
        p=intro_world_table(w,INTRO_INLINE_DATA,0x760aeu+16u*a[IA_SUBTYPE],16);if(!p) return;
        memcpy(a+IA_LAYER,p,3);ip16(a+IA_X,iw16(p+4));ip16(a+IA_Y,iw16(p+6));
        ip32(a+IA_VX,(uint32_t)is16(iw16(p+8))*256u);ip32(a+IA_AX,(uint32_t)is16(iw16(p+10))*256u);
        if(!intro_actor_select(w,a,iw32(p+12),p[3])) intro_actor_collect(w,a,1);
        return;
    case 2:
        switch(a[IA_STEP]) {
        case 0:
            if(a[IA_SUBTYPE]<=1) {
                ip32(a+IA_X,iw32(a+IA_X)+iw32(a+IA_VX));ip32(a+IA_VX,iw32(a+IA_VX)+iw32(a+IA_AX));
                if(iw16(a+IA_VX)<=1) {a[IA_STEP]+=2;a[IA_TIMER]=32;}
            }
            break;
        case 2:
            ip32(a+IA_X,iw32(a+IA_X)+iw32(a+IA_VX));a[IA_TIMER]--;
            if(!a[IA_TIMER]) {a[IA_STEP]+=2;ip32(a+IA_AX,0u-iw32(a+IA_AX));*ig(w,IG_TIMER)=0x82;}
            break;
        case 4:
            ip16(a+IA_X,iw16(a+IA_X)+iw16(a+IA_VX));ip32(a+IA_VX,iw32(a+IA_VX)+iw32(a+IA_AX));
            if(((iw16(a+IA_X)+80u)&65535u)>544u) a[IA_STATE]+=2;
            /* This substate really updates and collects twice per tick. */
            tick_collect(w,a,1);break;
        default:w->error="intro: strip motion state";return;
        }
        tick_collect(w,a,1);return;
    case 4:case 6:done(w,a);return;
    default:w->error="intro: strip actor state";
    }
}
static void wind_actor(struct intro_world *w,uint8_t *a)
{
    unsigned kind=a[IA_SUBTYPE],x,y,r;const uint8_t *p;
    switch(a[IA_STATE]) {
    case 0:
        if(kind>5) {w->error="intro: wind subtype";return;}
        a[IA_STATE]+=2;a[IA_FLIP]=a[IA_CAMERA]=a[IA_SHAKE]=0;ip16(a+IA_BANK,0x2000);ip16(a+IA_X+2,0);ip16(a+IA_Y+2,0);
        if(kind<3) {
            ip16(a+IA_PRIORITY,0xa000);a[IA_LAYER]=(uint8_t)(kind*2+10);x=448;
            if(a[IA_DELAY]) {x=intro_world_random(w)&511u;if(x>384) x-=128;}
            ip16(a+IA_X,x);y=intro_world_random(w)&127u;if(y<=112) y+=72;ip16(a+IA_Y,y);
        } else {
            ip16(a+IA_PRIORITY,0xe000);a[IA_LAYER]=(uint8_t)((kind-2)*2);
            ip16(a+IA_X,(intro_world_random(w)&255u)+64u);ip16(a+IA_Y,a[IA_DELAY] ? intro_world_random(w)&255u : 0xffd0u);
        }
        r=(intro_world_random(w)&12u)+kind*16u;p=intro_world_table(w,INTRO_INLINE_DATA,0x76710u+r,4);if(!p) return;
        ip32(a+IA_VX,kind<3 ? iw32(p) : 0);ip32(a+IA_VY,kind<3 ? 0 : iw32(p));
        if(!intro_actor_select(w,a,0x229120,kind+(kind<3?9u:14u))) intro_actor_collect(w,a,1);
        return;
    case 2:
        ip32(a+IA_X,iw32(a+IA_X)-iw32(a+IA_VX));
        if(is16(iw16(a+IA_X))<=-64) a[IA_STATE]+=2;
        else {ip32(a+IA_Y,iw32(a+IA_Y)+iw32(a+IA_VY));if(is16(iw16(a+IA_Y))>=288) a[IA_STATE]+=2;}
        intro_actor_collect(w,a,1);
        return;
    case 4:case 6:done(w,a);return;
    default:w->error="intro: wind actor state";
    }
}
int intro_actor_step_native(struct intro_world *w,unsigned pool,uint16_t actor)
{
    uint8_t *a=intro_world_actor(w,pool,actor);if(!a) return -1;
    if(pool==1) {intro_actor_secondary(w,a);return w->error ? -1 : 0;}
    if(pool==2) {intro_actor_ui(w,a);return w->error ? -1 : 0;}
    if(pool!=0) {w->error="intro: native actor pool missing";return -1;}
    switch(a[IA_KIND]) {
    case 1:case 0x8d:intro_actor_map(w,a);break;
    case 0x8c:intro_actor_palette(w,a);break;
    case 0x80:flame(w,a);break;
    case 0x81:intro_actor_logo(w,a);break;
    case 0x8b:intro_actor_duel(w,a);break;
    case 0x82:strip(w,a);break;
    case 0x84:wind_actor(w,a);break;
    case 0x83:intro_actor_strips(w,a);break;
    case 0x85:intro_actor_portrait(w,a);break;
    case 0x86:intro_actor_fighter(w,a);break;
    case 0x87:case 0x88:case 0x89:case 0x8a:case 0x8e:intro_actor_montage(w,a);break;
    default:w->error="intro: native primary actor missing";
    }
    return w->error ? -1 : 0;
}
