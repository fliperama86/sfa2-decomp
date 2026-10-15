/* Native Ryu/Ken, energy trails and title sparks. */
#include "actors.h"
#include <string.h>
static const uint8_t *operand(struct intro_world *w,uint32_t address,size_t n)
{return intro_world_table(w,INTRO_INLINE_DATA,address,n);}
static void bounds(struct intro_world *w,uint8_t *a)
{
    if(((iw16(a+IA_X)-gw(w,0x290)+96u)&65535u)>576u || ((iw16(a+IA_Y)-gw(w,0x294)+96u)&65535u)>448u) a[IA_STATE]=6;
}
static void style(uint8_t *a,unsigned layer,unsigned palette,unsigned priority,unsigned camera)
{
    a[IA_LAYER]=(uint8_t)layer;a[IA_BYPASS]=a[IA_FLIP]=a[IA_SHAKE]=0;a[IA_CAMERA]=(uint8_t)camera;
    ip16(a+IA_PALETTE_MODE,palette);ip16(a+IA_PRIORITY,priority);ip16(a+IA_BANK,0x2000);
}
static void sequence_frame(struct intro_world *w,uint8_t *a,unsigned offset,unsigned frame)
{
    const uint8_t *p=intro_world_table(w,INTRO_DATA,0x229120u+offset,2);uint32_t address;if(!p) return;
    address=(uint32_t)(0x229120+is16(iw16(p)+frame*16u));p=intro_world_table(w,INTRO_DATA,address,16);if(!p) return;
    ip32(a+IA_SEQUENCE,address);memcpy(a+IA_TICKS,p,4);
}
static void position(struct intro_world *w,uint8_t *a,uint32_t table,unsigned index)
{
    const uint8_t *p=operand(w,table+index*4u,4);if(!p) return;ip16(a+IA_X,iw16(p));ip16(a+IA_Y,iw16(p+2));
}
static uint8_t *spawn(struct intro_world *w,unsigned subtype)
{
    uint16_t id=intro_world_alloc(w,0);uint8_t *a;if(!id) return NULL;
    a=w->ram+id;a[IA_ACTIVE]++;ip16(a+IA_KIND,0x8b00u+subtype);return a;
}
static void random_pose(struct intro_world *w,uint8_t *a)
{
    const uint8_t *p=operand(w,0x7999eu+(intro_world_random(w)&15u),1);unsigned r;if(!p) return;
    intro_actor_select(w,a,0x229120,*p);r=intro_world_random(w)&31u;a[IA_TIMER]=(uint8_t)(r<8 ? r+8 : r);
}
static void trail(struct intro_world *w,uint8_t *a,int write)
{
    uint32_t base=iw32(a+0x60),seq;unsigned at=iw16(a+0x64);uint8_t *p;
    if((base&0xffff0000u)!=0xffff0000u || (base&65535u)+at+8u>65536u) {w->error="intro: duel trail range";return;}
    p=w->ram+(base&65535u)+at;
    if(write) {
        seq=iw32(a+IA_SEQUENCE)&0xffffffu;if(iw16(a+IA_STATE)==0x202) seq|=0x80000000u;
        ip16(p,iw16(a+IA_X));ip16(p+2,iw16(a+IA_Y));ip32(p+4,seq);
    } else {
        seq=iw32(p+4);if(seq&0x80000000u) {a[IA_STATE]+=2;seq&=0xffffffu;}
        ip16(a+IA_X,iw16(p));ip16(a+IA_Y,iw16(p+2));ip32(a+IA_SEQUENCE,seq);
    }
    ip16(a+0x64,(at+8u)&255u);
}
static void palette_sequence(struct intro_world *w,uint8_t *a,int begin)
{
    const uint8_t *p;uint32_t address,list,source;unsigned i,j,destination,selector,command;
    if(begin) {
        p=intro_world_table(w,INTRO_DATA,0x79d82,2);if(!p) return;address=(uint32_t)(0x79d82+is16(iw16(p)));
    } else {
        ip16(a+IA_TICKS,iw16(a+IA_TICKS)-1u);if(iw16(a+IA_TICKS)) return;address=iw32(a+IA_SEQUENCE)+8;
        if(a[IA_FLAGS]&128u) {p=intro_world_table(w,INTRO_DATA,address,4);if(!p) return;address=iw32(p);}
    }
    p=intro_world_table(w,INTRO_DATA,address,8);if(!p) return;ip32(a+IA_SEQUENCE,address);memcpy(a+IA_TICKS,p,4);list=iw32(p+4);
    for(i=0;i<128;i++,list+=6) {
        uint8_t *dst;p=intro_world_table(w,INTRO_DATA,list,6);if(!p) return;destination=iw16(p);selector=iw16(p+2);command=iw16(p+4);
        p=operand(w,0x18d58u+selector,4);if(!p) return;source=iw32(p)+(command&0xffe0u);
        p=intro_world_table(w,INTRO_DATA,source,32);dst=intro_world_video(w,0x90c000u+destination,32);if(!p || !dst) return;
        for(j=0;j<16;j++) ip16(dst+j*2,(iw16(dst+j*2)&0xf000u)|(iw16(p+j*2)&4095u));
        if(command&1u) return;
    }
    w->error="intro: duel palette command list";
}
static void leader_motion(struct intro_world *w,uint8_t *a)
{
    const uint8_t *p;uint8_t *spark;unsigned index=a[IA_TIMER]*8u+a[IA_VARIANT]*4u;
    p=operand(w,0x799f6u+index,4);if(!p) return;ip16(a+IA_X,iw16(p));ip16(a+IA_Y,iw16(p+2));
    a[IA_TIMER]++;if(a[IA_TIMER]>31) a[IA_TIMER]=31;
    a[IA_TIMER+1]--;
    if(!a[IA_TIMER+1]) {
        a[IA_TIMER+1]=(uint8_t)((intro_world_random(w)&3u)+1u);spark=spawn(w,12);
        if(spark) ip16(spark+IA_LINK,(unsigned)(a-w->ram));
    }
}
void intro_actor_duel(struct intro_world *w,uint8_t *a)
{
    unsigned type=a[IA_SUBTYPE],variant=a[IA_VARIANT],state=a[IA_STATE],r,x,y;uint8_t *parent,*c;const uint8_t *p;
    if(type>12 || ((type<=2 || type==4 || type==5 || type==7 || type==8 || type==11) && variant>1)) {w->error="intro: duel subtype or side";return;}
    if(state==4 || state==6) {intro_world_delete(w,0,(uint16_t)(a-w->ram));return;}
    if(state!=0 && state!=2) {w->error="intro: duel actor state";return;}
    if(state==0) {
        a[IA_STATE]+=2;
        if(type==3) {palette_sequence(w,a,1);return;}
        if(type==12) {
            parent=intro_world_actor(w,0,iw16(a+IA_LINK));if(!parent) return;memcpy(a+IA_LAYER,parent+IA_LAYER,7);
            ip16(a+IA_X,iw16(parent+IA_X));ip16(a+IA_Y,iw16(parent+IA_Y));memcpy(a+IA_PRIORITY,parent+IA_PRIORITY,4);ip16(a+IA_ROW,0);a[IA_LAYER]+=2;
            r=(uint8_t)(parent[IA_TIMER]+1u);r=(r>>1)+(parent[IA_VARIANT] ? 4u : 12u);a[IA_DELAY]=(uint8_t)(r&15u);
            r=intro_world_random(w)&3u;p=operand(w,0x7969cu+r*4u,4);if(!p) return;ip32(a+0x60,iw32(p));
            p=operand(w,0x79698u+r,1);if(!p) return;sequence_frame(w,a,*p,a[IA_DELAY]);intro_actor_collect(w,a,1);return;
        }
        if(type==9) style(a,0,0,0xa000,0);
        else if(type==10) style(a,6,0,0x2000,8);
        else if(type==11) {style(a,6,0,0xa000,8);a[IA_BYPASS]=1;}
        else {style(a,type==5 || type==8 ? 2 : 0,0,0xe000,8);ip16(a+IA_ROW,0);}
        if(type==0) {
            a[IA_LAYER]=(uint8_t)(variant ? 4 : 8);ip16(a+IA_PALETTE_MODE,variant ? 0x8002 : 0);ip16(a+IA_PRIORITY,variant ? 0x6000 : 0xa000);
            p=operand(w,0x7971au+variant*8u,8);if(!p) return;x=iw32(p);y=iw32(p+4);ip32(a+0x50,x);ip32(a+0x54,y);
            p=operand(w,0x7972au+a[IA_DELAY]*16u,16);if(!p) return;ip32(a+IA_X,x+iw32(p));ip32(a+IA_Y,y+iw32(p+4));memcpy(a+IA_VX,p+8,8);
            sequence_frame(w,a,0x40,a[IA_DELAY]);intro_actor_collect(w,a,1);return;
        }
        if(type==1 || type==2) {
            ip16(a+IA_PALETTE_MODE,variant ? 0x8002 : 0);position(w,a,0x79940,variant);intro_actor_select(w,a,0x229120,type==1 ? 33 : 34);
            if(type==2) intro_actor_collect(w,a,1);
            return;
        }
        if(type==4) {ip16(a+IA_PALETTE_MODE,variant ? 0x8002 : 0);position(w,a,0x7995e,variant);random_pose(w,a);intro_actor_collect(w,a,1);return;}
        if(type==5) {
            a[IA_TIMER]=*ig(w,0xb5)&1u;position(w,a,0x7997c,variant);sequence_frame(w,a,0x46,variant);
            bounds(w,a);a[IA_TIMER]^=1;if(a[IA_TIMER]) intro_actor_collect(w,a,1);return;
        }
        if(type==6) {
            a[IA_FLIP]=(uint8_t)(intro_world_random(w)&3u);x=(intro_world_random(w)&127u)*3u+64u;
            ip16(a+IA_PALETTE_MODE,x>=256 ? 0x8002 : 0);ip16(a+IA_X,x);ip16(a+IA_Y,(intro_world_random(w)&255u)+512u);
            intro_actor_select(w,a,0x79c24,0);return;
        }
        if(type==7) {
            ip16(a+IA_TIMER,2);ip16(a+0x64,0);ip32(a+0x60,0xffff4bb4u+variant*256u);ip16(a+IA_X,variant ? 352 : 160);ip16(a+IA_Y,368);
            intro_actor_select(w,a,0x229120,39);trail(w,a,1);intro_actor_collect(w,a,1);return;
        }
        if(type==8) {
            a[IA_PALETTE_MODE]=128;a[IA_PALETTE]=a[IA_DELAY];ip16(a+0x64,0);a[IA_TIMER]=(uint8_t)(a[IA_DELAY]*4u);
            ip32(a+0x60,0xffff4bb4u+variant*256u);trail(w,a,0);intro_actor_collect(w,a,1);return;
        }
        if(type==9) {
            position(w,a,0x79b38,a[IA_DELAY]);intro_actor_select(w,a,0x79c24,1);
            /* Title sparks execute their update on the initialization tick. */
            if(a[IA_FLAGS]&128u) a[IA_STATE]+=2;
            intro_actor_tick(w,a);intro_actor_collect(w,a,1);return;
        }
        if(type==10) {ip16(a+IA_X,256);ip16(a+IA_Y,896);intro_actor_select(w,a,0x79c24,2);bounds(w,a);intro_actor_collect(w,a,1);return;}
        if(type==11) {
            p=operand(w,0x7956au+variant*16u,16);if(!p) return;ip16(a+IA_X,iw16(p));ip16(a+IA_Y,iw16(p+2));
            ip32(a+IA_VX,iw32(p+4));ip32(a+IA_VY,iw32(p+8));ip32(a+IA_AY,iw32(p+12));intro_actor_select(w,a,0x229120,46+variant);return;
        }
    }
    switch(type) {
    case 0:
        ip32(a+IA_X,iw32(a+IA_X)+iw32(a+IA_VX));ip32(a+IA_Y,iw32(a+IA_Y)+iw32(a+IA_VY));
        if(((iw16(a+0x50)-iw16(a+IA_X)+2u)&65535u)<=4u && ((iw16(a+0x54)-iw16(a+IA_Y)+2u)&65535u)<=4u) a[IA_STATE]+=2;
        bounds(w,a);break;
    case 1:
        if(a[IA_FLAGS]&128u) a[IA_STATE]+=2;
        if(a[IA_FLAGS+1]&1u) {c=spawn(w,2);if(c) {c[IA_VARIANT]=(uint8_t)variant;a[IA_FLAGS+1]&=254u;}}
        intro_actor_tick(w,a);if(a[IA_FLAGS+1]&2u) return;break;
    case 2:
        if(a[IA_FLAGS]&128u) a[IA_STATE]+=2;
        if(a[IA_FLAGS+1]) {*ig(w,IG_SIGNAL+variant)=a[IA_KIND];a[IA_FLAGS+1]=0;}
        intro_actor_tick(w,a);break;
    case 3:
        if(a[IA_FLAGS]&128u) {a[IA_STATE]+=2;*ig(w,IG_SIGNAL)=a[IA_KIND];}
        palette_sequence(w,a,0);return;
    case 4:
        bounds(w,a);a[IA_TIMER]--;if(!a[IA_TIMER]) random_pose(w,a);else intro_actor_tick(w,a);break;
    case 5:bounds(w,a);a[IA_TIMER]^=1;if(!a[IA_TIMER]) return;break;
    case 6:
        if(a[IA_FLAGS]&128u) a[IA_STATE]+=2;
        intro_actor_tick(w,a);bounds(w,a);break;
    case 7:
        if(a[IA_STEP]==0) {
            leader_motion(w,a);
            if(((256u-iw16(a+IA_X)+4u)&65535u)<=8 && ((384u-iw16(a+IA_Y)+4u)&65535u)<=8) {
                a[IA_STEP]+=2;*ig(w,IG_SIGNAL+variant)=a[IA_KIND];ip16(a+IA_X,256);ip16(a+IA_Y,384);
            }
            intro_actor_tick(w,a);trail(w,a,1);
        } else if(a[IA_STEP]==2) {
            if(*ig(w,IG_SIGNAL)==*ig(w,IG_SIGNAL+1)) {
                if(variant) {a[IA_STATE]+=2;return;}
                a[IA_STEP]+=2;intro_actor_select(w,a,0x229120,40);
            } else {intro_actor_tick(w,a);trail(w,a,1);}
        } else if(a[IA_STEP]==4) intro_actor_tick(w,a);
        else {w->error="intro: duel leader motion state";return;}
        break;
    case 8:
        if(a[IA_STEP]==0) {a[IA_TIMER]--;if(!a[IA_TIMER]) a[IA_STEP]+=2;}
        else if(a[IA_STEP]==2) trail(w,a,0);
        else {w->error="intro: duel follower delay state";return;}
        break;
    case 9:if(a[IA_FLAGS]&128u) a[IA_STATE]+=2;intro_actor_tick(w,a);break;
    case 10:bounds(w,a);break;
    case 11:
        if(a[IA_STEP]==0) {if(gw(w,0x290)>=(variant ? 320u : 256u)) {a[IA_STEP]+=2;ip16(a+IA_PRIORITY,0xe000);}}
        else if(a[IA_STEP]==2) {
            ip32(a+IA_X,iw32(a+IA_X)+iw32(a+IA_VX));ip32(a+IA_Y,iw32(a+IA_Y)+iw32(a+IA_VY));ip32(a+IA_VY,iw32(a+IA_VY)+iw32(a+IA_AY));
            intro_actor_tick(w,a);bounds(w,a);
        } else {w->error="intro: passing actor motion state";return;}
        break;
    case 12:
        p=intro_world_table(w,INTRO_DATA,iw32(a+0x60)+a[IA_DELAY]*4u,4);if(!p) return;
        ip32(a+IA_X,iw32(a+IA_X)+(uint32_t)is16(iw16(p))*256u);ip32(a+IA_Y,iw32(a+IA_Y)+(uint32_t)is16(iw16(p+2))*256u);bounds(w,a);break;
    }
    intro_actor_collect(w,a,1);
}
