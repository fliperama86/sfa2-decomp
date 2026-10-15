/* Shared native intro data operations. No original instructions are retained. */
#include "world.h"
#include "particles.h"
#include <string.h>

struct pool { unsigned first,count,stride,stack,free_pointer,free_count,class_id; };
static const struct pool pools[]={
    {0x2100,40,128,0x61aa,0x60f4,0x6116,16},
    {0x3700,16,128,0x61ca,0x60f6,0x6117,20},
    {0x3f00,8,512,0x61da,0x60f8,0x6118,24},
    {0x3500,4,128,0x61e2,0x60fa,0x6119,16},
    {0x1400,16,128,0x6146,0x60f0,0x6114,8},
    {0x1c00,10,128,0x615a,0x60f2,0x6115,8}
};
const uint8_t *intro_world_table(struct intro_world *w,unsigned plane,uint32_t address,size_t n)
{
    const uint8_t *p=w->read ? w->read(w->read_context,plane,address,n) : NULL;
    if(!p) w->error="intro: missing typed data resource";
    return p;
}
uint8_t *intro_world_video(struct intro_world *w,uint32_t address,size_t n)
{
    if(address<0x900000u || address>0x940000u || n>0x940000u-address) {
        w->error="intro: video data range";return NULL;
    }
    return w->video+address-0x900000u;
}
uint8_t *intro_world_actor(struct intro_world *w,unsigned pool,uint16_t actor)
{
    const struct pool *p;
    unsigned offset;
    if(pool>=sizeof pools/sizeof *p) {w->error="intro: actor pool";return NULL;}
    p=&pools[pool];offset=(actor-0x8000u-p->first)&65535u;
    if(offset>=p->count*p->stride || offset%p->stride) {w->error="intro: actor link";return NULL;}
    return w->ram+actor;
}
static void reset_draw_lists(struct intro_world *w)
{
    static const uint16_t tops[]={0x6202,0x6216,0x6252,0x6286,0x62de,0x6336,0x638e,0x63e6,0x643e,0x6496,0x64ee,0x6546};
    unsigned i;
    for(i=0;i<12;i++) sw(w,0x60fc+2*i,0x8000u+tops[i]);
    sw(w,0x6546,0xe552);memset(ig(w,0x611a),0,12);
    sw(w,0x654a,0);sw(w,0x654e,0);
}
void intro_world_reset_actors(struct intro_world *w)
{
    unsigned i,j;
    memset(ig(w,0x400),0,0xc00);memset(ig(w,0x200),0,0x200);
    *ig(w,0xa9)=255;memset(ig(w,0x100),0,128);memset(ig(w,0x6d52),0,256);
    sw(w,0x654c,0);sw(w,0x6550,0);memset(ig(w,0x4f00),0,64);
    *ig(w,0x1e48)=28;*ig(w,0x8a)=*ig(w,0x8b)=*ig(w,0x8c)=0;
    for(i=0x400;i<=0x1000;i+=0x400) {
        sl(w,i+4,0);sl(w,i+0x28,0);sl(w,i+0x2c,0);
        *ig(w,i+0x31)=*ig(w,i+0x5e)=*ig(w,i+0x5d)=0;
        sl(w,i+0x94,0);memset(ig(w,i+0x100),0,0x300);
    }
    *ig(w,0xc08)=*ig(w,0x1008)=4;
    for(i=0;i<sizeof pools/sizeof *pools;i++) {
        const struct pool *p=&pools[i];
        *ig(w,p->free_count)=(uint8_t)p->count;
        for(j=0;j<p->count;j++) {
            unsigned offset=p->first+j*p->stride;
            memset(ig(w,offset),0,p->stride);*ig(w,offset+8)=(uint8_t)p->class_id;
            sw(w,p->stack-2*(j+1),0x8000u+offset);
        }
        sw(w,p->free_pointer,0x8000u+p->stack-2*p->count);
    }
    memset(ig(w,0x6552),0,2048);
    for(i=0;i<4;i++) sw(w,0x6216+2*i,0x8400u+i*0x400u);
    reset_draw_lists(w);
}
uint16_t intro_world_alloc(struct intro_world *w,unsigned pool)
{
    const struct pool *p;unsigned top,actor,count;
    if(pool>=sizeof pools/sizeof *p) {w->error="intro: allocation pool";return 0;}
    p=&pools[pool];count=*ig(w,p->free_count);if(!count) return 0;
    top=gw(w,p->free_pointer);
    if(count>p->count || top!=0x8000u+p->stack-2*count) {w->error="intro: free list";return 0;}
    actor=iw16(w->ram+top);if(!intro_world_actor(w,pool,(uint16_t)actor)) return 0;
    sw(w,p->free_pointer,top+2);*ig(w,p->free_count)=(uint8_t)(count-1);
    return (uint16_t)actor;
}
int intro_world_delete(struct intro_world *w,unsigned pool,uint16_t actor)
{
    uint8_t *a=intro_world_actor(w,pool,actor);const struct pool *p;unsigned top,count;
    if(!a) return -1;
    p=&pools[pool];count=*ig(w,p->free_count);top=gw(w,p->free_pointer);
    if(count>=p->count || top!=0x8000u+p->stack-2*count) {w->error="intro: free list return";return -1;}
    memset(a,0,16);a[IA_CLASS]=(uint8_t)p->class_id;
    if(pool!=2) { a[0x30]=0;ip16(a+0x26,0); }
    if(pool==0 || pool==3 || pool>=4) {ip32(a+0x28,0);ip32(a+0x2c,0);}
    ip16(w->ram+top-2,actor);sw(w,p->free_pointer,top-2);*ig(w,p->free_count)=(uint8_t)(count+1);
    return 0;
}
void intro_world_update_actors(struct intro_world *w,unsigned pool)
{
    unsigned i;const struct pool *p;
    if(pool>=4 || !w->actor_step) {w->error="intro: native actor updater missing";return;}
    p=&pools[pool];*ig(w,0xb5)=(uint8_t)p->count;
    for(i=0;i<p->count && !w->error;i++) {
        uint16_t actor=(uint16_t)(0x8000u+p->first+i*p->stride);
        if(w->ram[actor] && w->actor_step(w,pool,actor)) {if(!w->error) w->error="intro: native actor handler missing";return;}
        (*ig(w,0xb5))--;
    }
}
void intro_world_clear_maps(struct intro_world *w)
{
    unsigned i;
    for(i=0;i<0x4000;i+=4) {ip32(w->video+i,0x00200000u);ip32(w->video+0x4000+i,0xa4200000u);ip32(w->video+0x8000+i,0x09000000u);}
    for(i=0xe000;i<0x10000;i+=2) ip16(w->video+i,192);
    if(!*ig(w,0x95)) {*ig(w,0x94)^=1;*ig(w,0x95)=255;}
}
void intro_world_load_map(struct intro_world *w,unsigned layer,unsigned index,uint32_t destination)
{
    const uint8_t *src;uint8_t *dst;unsigned i,count,bias;
    if(layer>2) {w->error="intro: map layer";return;}
    index&=255u;
    if(!layer) {
        src=intro_world_table(w,INTRO_DATA,0x2ad300u+index*128u,128);
        dst=intro_world_video(w,destination,0x1000);if(!src || !dst) return;
        for(i=0;i<64;i++) {
            const uint8_t *block=intro_world_table(w,INTRO_DATA,0x2b1a80u+iw16(src+2*i)*64u,64);
            unsigned x,y;if(!block) return;
            for(x=0;x<4;x++) for(y=0;y<4;y++) memcpy(dst+(i/8)*0x200+(i%8)*16+x*128+y*4,block+(x*4+y)*4,4);
        }
        return;
    }
    count=layer==1 ? 256u : 64u;bias=layer==1 ? 0xa400u : 0x900u;
    src=intro_world_table(w,INTRO_DATA,(layer==1 ? 0x2d1f00u : 0x2f8700u)+index*count*4u,count*4u);
    dst=intro_world_video(w,destination,count*4u);if(!src || !dst) return;
    for(i=0;i<count;i++) {ip16(dst+4*i,iw16(src+4*i)+bias);ip16(dst+4*i+2,iw16(src+4*i+2));}
}
void intro_world_fill_map(struct intro_world *w,unsigned layer,unsigned pen,unsigned palette,uint32_t destination)
{
    unsigned cells,code,i;uint8_t *dst;
    if(layer>2) {w->error="intro: fill layer";return;}
    cells=1536u>>(2u*layer);code=(layer==0 ? 0x10u : layer==1 ? 0xa410u : 0x900u)+((pen+(layer==2))&15u);
    dst=intro_world_video(w,destination,cells*4u);if(!dst) return;
    for(i=0;i<cells;i++) {ip16(dst+4*i,code);ip16(dst+4*i+2,palette&31u);}
}
void intro_world_palette_copy(struct intro_world *w,uint32_t source,uint32_t destination,unsigned colors,int brightness)
{
    unsigned i;const uint8_t *src;uint8_t *dst;
    if(colors>2048) {w->error="intro: palette count";return;}
    src=intro_world_table(w,INTRO_DATA,source,colors*2u);dst=intro_world_video(w,destination,colors*2u);
    if(!src || !dst) return;
    for(i=0;i<colors;i++) ip16(dst+2*i,iw16(src+2*i)|(brightness<0 ? iw16(dst+2*i)&0xf000u : (unsigned)brightness));
}
void intro_world_palettes(struct intro_world *w,unsigned full)
{
    static const uint32_t tables[]={0x276dc0,0x2811c0,0x28b5c0,0x2959c0};
    unsigned i,set=gw(w,IG_PALETTE_SET);
    for(i=0;i<4;i++) {
        unsigned index=i==1 && set>6 ? 6 : set;
        int32_t offset=is16(((index>>7)|(index<<9))&65535u);
        intro_world_palette_copy(w,(uint32_t)(tables[i]+offset),0x90c000u+1024u*i,512,full ? 0xf000 : 0);
    }
}
unsigned intro_world_random(struct intro_world *w)
{
    uint16_t seed=(uint16_t)gw(w,IG_SEED);unsigned value=intro_particle_random(&seed);sw(w,IG_SEED,seed);return value;
}
