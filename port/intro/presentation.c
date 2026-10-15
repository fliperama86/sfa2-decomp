/* Reconstructed intro subset of 127a4/12932/13028 and UI consumer 12f80.
 * Original caller scratch registers are not game state and are not retained. */
#include "presentation.h"
#include "sprites.h"
#include <string.h>

static const uint8_t *data(struct intro_world *w,uint32_t address,size_t n,int ui)
{
    if(ui && (address>>16==0xffffu || address>>16==0xffu)) {
        unsigned at=address&65535u;
        if(at>=0xbf00u && at<=0xcf00u && n<=0xcf00u-at) return w->ram+at;
        w->error="intro: UI art outside its pool";return NULL;
    }
    return intro_world_table(w,INTRO_DATA,address,n);
}
static int art(struct intro_world *w,uint32_t address,int ui,
               struct intro_sprite_art *out,struct intro_sprite_block *blocks)
{
    const uint8_t *p,*coordinates=NULL,*codes;uint32_t cp=0;unsigned i,header,mode;
    p=data(w,address,2,ui);if(!p) return -1;
    mode=iw16(p);memset(out,0,sizeof *out);out->mode=mode;out->block=blocks;
    if(mode==10 && !ui) return 0;
    if(mode>8 || (mode&1u) || (ui && mode!=4)) {w->error="intro: art format";return -1;}
    header=mode==4 ? 6u : mode==6 ? 12u : mode==8 ? 10u : 8u;
    p=data(w,address,header,ui);if(!p) return -1;
    out->cells=iw16(p+2);out->blocks=mode==0 ? out->cells : (unsigned)iw16(p+4)+1u;
    if(!out->cells || out->cells>INTRO_VIDEO_OBJECTS || !out->blocks || out->blocks>INTRO_VIDEO_OBJECTS) {
        w->error="intro: art cell/block count";return -1;
    }
    if(mode==0 || mode==2) {
        const uint8_t *ptr=data(w,(uint32_t)(0x9f260+is16(iw16(p+6))),4,0);
        if(!ptr) return -1;
        cp=iw32(ptr);
    } else if(mode==6) cp=iw32(p+8);
    else if(mode==8) cp=iw32(p+6);
    if(mode!=4) {coordinates=data(w,cp,out->blocks*4u,0);if(!coordinates) return -1;}
    codes=data(w,address+header,out->blocks*(mode==4 ? 8u : (mode==0 || mode==6) ? 2u : 4u),ui);
    if(!codes) return -1;
    for(i=0;i<out->blocks;i++) {
        struct intro_sprite_block *b=blocks+i;
        if(mode==4) {
            b->code=iw16(codes+i*8);b->attributes=iw16(codes+i*8+2);
            b->x=(int16_t)is16(iw16(codes+i*8+4));b->y=(int16_t)is16(iw16(codes+i*8+6));
        } else {
            b->x=(int16_t)is16(iw16(coordinates+i*4));b->y=(int16_t)is16(iw16(coordinates+i*4+2));
            b->code=iw16(codes+i*((mode==0 || mode==6) ? 2u : 4u));
            b->attributes=(mode==0 || mode==6) ? iw16(p+(mode==0 ? 4u : 6u)) : iw16(codes+i*4+2);
        }
    }
    return 0;
}
static int draw(struct intro_world *w,struct intro_sprite_list *list,uint8_t *a,int ui)
{
    struct intro_sprite_style s;struct intro_sprite_art shape;
    struct intro_sprite_block blocks[INTRO_VIDEO_OBJECTS];
    const uint8_t *p;uint32_t address=iw32(a+IA_SEQUENCE);unsigned camera=a[IA_CAMERA],i;
    memset(&s,0,sizeof s);s.x=(uint16_t)(iw16(a+IA_X)+64u);s.y=(uint16_t)(256u-iw16(a+IA_Y));
    s.x_flags=iw16(a+IA_PRIORITY);s.y_flags=iw16(a+IA_BANK);
    s.palette_mode=a[IA_PALETTE_MODE];s.palette=a[IA_PALETTE];
    if(!ui) {
        if(a[IA_CLASS]!=16 && a[IA_CLASS]!=20) {w->error="intro: renderer actor class";return -1;}
        if(a[IA_SHAKE]) s.y=(uint16_t)(s.y-gw(w,0x14e));
        if(camera) {
            if(camera!=4 && camera!=8 && camera!=12) {w->error="intro: renderer camera";return -1;}
            s.x=(uint16_t)(s.x-gw(w,0x210+(camera-4)*32));
            s.y=(uint16_t)(s.y+gw(w,0x214+(camera-4)*32));
            if(camera==8 && iw16(a+IA_ROW)) {w->error="intro: unobserved actor row correction";return -1;}
        }
        s.bypass=a[IA_BYPASS];s.flip=a[IA_FLIP];
        s.expand_x=(int16_t)is16(iw16(a+IA_SCALE_X));s.expand_y=(int16_t)is16(iw16(a+IA_SCALE_Y));
        p=data(w,address+4,4,0);if(!p) return -1;address=iw32(p);
    }
    if(art(w,address,ui,&shape,blocks)) return -1;
    if(!ui && !s.bypass && (shape.mode==6 || shape.mode==8) && iw16(a+IA_ANGLE)) {
        uint32_t matrix=(uint32_t)(0x13122+is16(iw16(a+IA_ANGLE)*8u));
        p=data(w,matrix,8,0);if(!p) return -1;s.rotate=1;
        for(i=0;i<4;i++) s.matrix[i]=(int16_t)is16(iw16(p+2*i));
    }
    if(intro_sprites_append(list,&shape,&s)<0) {w->error="intro: object-list expansion";return -1;}
    return 0;
}
static int linked(struct intro_world *w,struct intro_sprite_list *list,uint8_t *a,unsigned at)
{
    uint16_t id=iw16(a+at);uint8_t *child;if(!id) return 0;
    child=intro_world_actor(w,1,id);if(!child) return -1;
    return child[IA_ACTIVE] && child[IA_VISIBLE] ? draw(w,list,child,0) : 0;
}
static int layer(struct intro_world *w,struct intro_sprite_list *list,unsigned number)
{
    static const unsigned tops[]={0x62de,0x6336,0x638e,0x63e6,0x643e,0x6496,0x64ee,0x6546};
    unsigned i,count=*ig(w,0x611e + number),top=tops[number];
    if(count>44) {w->error="intro: renderer queue count";return -1;}
    for(i=0;i<count;i++) {
        uint16_t id=(uint16_t)gw(w,top-2*(i+1));uint8_t *a;
        /* The intro queues primary actors, not the optional four-slot pool. */
        a=intro_world_actor(w,0,id);if(!a) return -1;
        if(linked(w,list,a,0x2e) || linked(w,list,a,0x2c) || draw(w,list,a,0) ||
           linked(w,list,a,0x2a) || linked(w,list,a,0x28)) return -1;
    }
    return 0;
}
static int ui_layer(struct intro_world *w,struct intro_sprite_list *list,int front)
{
    unsigned i;if(*ig(w,0x124)) return 0;
    for(i=0;i<8;i++) {
        uint8_t *a=ig(w,0x3f00+i*512);
        if(a[IA_ACTIVE] && a[IA_VISIBLE] && ((!a[IA_LAYER])==front) && draw(w,list,a,1)) return -1;
    }
    return 0;
}
void intro_present_clear(struct intro_world *w,struct intro_presentation *out)
{
    if(w->clear_objects) {
        ip32(out->objects+32,0x80008000u);ip32(out->objects+36,0x80008000u);
        out->count=4;w->clear_objects=0;
    }
}
int intro_present_objects(struct intro_world *w,struct intro_presentation *out)
{
    struct intro_sprite_list list;unsigned i;
    if(!w || !out || w->error) return -1;
    intro_present_clear(w,out);
    if(*ig(w,0x118) || *ig(w,0x95)) {intro_world_reset_draw_lists(w);return 0;}
    if(*ig(w,0x611c) || *ig(w,0x611d) || *ig(w,0x4f00) || gw(w,0x654e) || gw(w,0x6550) || gw(w,0x138)) {
        w->error="intro: non-intro object list";return -1;
    }
    for(i=0x400;i<=0x1000;i+=0x400) if(*ig(w,i)) {w->error="intro: gameplay actor in intro renderer";return -1;}
    memset(out->objects,0,32);list.objects=out->objects+32;list.capacity=INTRO_VIDEO_OBJECTS;list.count=0;list.cells_left=INTRO_VIDEO_OBJECTS;
    if(layer(w,&list,7) || layer(w,&list,6) || layer(w,&list,5) || layer(w,&list,4) || ui_layer(w,&list,0) ||
       layer(w,&list,3) || layer(w,&list,2) || layer(w,&list,1) || layer(w,&list,0) || ui_layer(w,&list,1)) return -1;
    out->count=list.count+4;ip32(out->objects+out->count*8,0x8000);
    sw(w,0x11a,INTRO_VIDEO_OBJECTS-list.cells_left);*ig(w,0x94)^=1;*ig(w,0x95)=255;
    intro_world_reset_draw_lists(w);return 1;
}
void intro_present_video(struct intro_world *w,const struct intro_presentation *p,
                         struct intro_video *v,intro_video_tile tile,void *context)
{
    unsigned i;memset(v,0,sizeof *v);v->memory=w->video;v->memory_size=sizeof w->video;
    for(i=0;i<3;i++) v->map_base[i]=(gw(w,0x18+2*i)<<8)&0x3c000u;
    v->palette_base=(gw(w,0x20)<<8)&0x3ffffu;v->row_base=(gw(w,0x1e)<<8)&0x3f800u;
    v->scroll_x[0]=(uint16_t)(gw(w,0x22)-64u);v->scroll_x[1]=(uint16_t)(gw(w,0x26)-256u);v->scroll_x[2]=(uint16_t)(gw(w,0x2a)-64u);
    v->scroll_y[0]=(uint16_t)(256u-gw(w,0x24));v->scroll_y[1]=(uint16_t)(768u-gw(w,0x28));v->scroll_y[2]=(uint16_t)(1792u-gw(w,0x2c));
    v->row_offset=(uint16_t)(768u-gw(w,0x2e));v->layer_control=(uint16_t)gw(w,0x32);
    v->priority_control=(uint16_t)gw(w,0x3a);v->video_control=(uint16_t)gw(w,0x30);
    v->objects=p->objects+32;v->object_count=p->count>=4 ? p->count-4 : 0;
    v->object_x_offset=(uint16_t)gw(w,0x3c);v->object_y_offset=(uint16_t)gw(w,0x3e);v->tile=tile;v->tile_context=context;
}
