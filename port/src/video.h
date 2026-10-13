/* Project-authored optional graphics policy, shared with the patched backend.
 * No asset input. Defaults/selection belong in settings.c and the GPU bridge. */
#ifndef PORT_VIDEO_H
#define PORT_VIDEO_H
#include <stdint.h>
#define PORT_VIDEO_PIXEL_GRID 0x2000
#define PORT_VIDEO_LIFE_FILL 1
#define PORT_ARCADE_W 384
#define PORT_ARCADE_H 224
#define PORT_ARCADE_TOP 8
struct port_video_corner { short x,y; unsigned short u,v; };
struct port_video_view { int x,y,w,h; };
static inline int port_video_canvas_allowed(int w,int h,int rgb24,int enabled)
{
    return enabled && w==384 && h==240 && !rgb24;
}
static inline int port_video_pixel_grid(struct port_video_corner p[4])
{
    unsigned short lo_u=p[0].u,hi_u=p[0].u,lo_v=p[0].v,hi_v=p[0].v;
    int i,dx=p[1].x-p[0].x,dy=p[2].y-p[0].y;
    if((dx!=16 && dx!=-16) || (dy!=16 && dy!=-16) ||
       p[0].x!=p[2].x || p[1].x!=p[3].x || p[0].y!=p[1].y || p[2].y!=p[3].y ||
       p[0].u!=p[2].u || p[1].u!=p[3].u || p[0].v!=p[1].v || p[2].v!=p[3].v)return 0;
    for(i=1;i<4;i++) {
        if(p[i].u<lo_u)lo_u=p[i].u;
        if(p[i].u>hi_u)hi_u=p[i].u;
        if(p[i].v<lo_v)lo_v=p[i].v;
        if(p[i].v>hi_v)hi_v=p[i].v;
    }
    if(hi_u>256 || hi_v>256 || (hi_u==256 && hi_u-lo_u==15) ||
       (hi_v==256 && hi_v-lo_v==15) ||
       (hi_u-lo_u!=15 && hi_u-lo_u!=16) || (hi_v-lo_v!=15 && hi_v-lo_v!=16))return 0;
    for(i=0;i<4;i++) {
        if(hi_u-lo_u==15 && p[i].u==hi_u)p[i].u++;
        if(hi_v-lo_v==15 && p[i].v==hi_v)p[i].v++;
    }
    return 1;
}
static inline int port_video_arcade_fit(int w,int h,struct port_video_view *out)
{
    int unit;
    *out=(struct port_video_view){0,0,0,0};
    if(w<384 || h<288)return 0;
    unit=w/4;if(h/3<unit)unit=h/3;
    out->w=unit*4;out->h=unit*3;out->x=(w-out->w)/2;out->y=(h-out->h)/2;
    return 1;
}
/* data_801987c8 stores a pointer, not an inline table. Initialization/frame
 * swap select these two 30-entry tables. Return -1 for unrelated addresses. */
static inline int port_video_ot_index(uint32_t ot)
{
    uint32_t physical=ot&0x1fffffffu,offset;
    if(physical>=0x1fc000u && physical<0x1fc078u)offset=physical-0x1fc000u;
    else if(physical>=0x1fc0e8u && physical<0x1fc160u)offset=physical-0x1fc0e8u;
    else return -1;
    return offset%4?-1:(int)(offset/4);
}
/* Inferred from the selected game's HUD producer: list indices8/14 and the 40-byte
 * tile record pool (the two initialized frame blocks). Other packets are excluded.
 * Address/list checks identify fixed RAM records before any packet reads. */
static inline int port_video_tag_tiles(uint32_t ot,uint32_t address,unsigned char *p,int enabled)
{
    uint32_t phys=address&0x1fffffffu;
    int list=port_video_ot_index(ot);
    struct port_video_corner corners[4];int i;
    if(!enabled || (list!=8 && list!=14) || phys<0x186004u ||
       phys>=0x188d04u || (phys-0x186004u)%40 || p[3]!=9 || (p[7]&0xfc)!=0x2c)return 0;
    for(i=0;i<4;i++) {
        unsigned x=p[8+i*8]|(unsigned)p[9+i*8]<<8;
        unsigned y=p[10+i*8]|(unsigned)p[11+i*8]<<8;
        corners[i]=(struct port_video_corner){(short)(((x&2047)^1024)-1024),
            (short)(((y&2047)^1024)-1024),p[12+i*8],p[13+i*8]};
    }
    if(!port_video_pixel_grid(corners))return 0;
    p[23]|=PORT_VIDEO_PIXEL_GRID>>8;return 1;
}
/* The one-to-one frame has a six-row interior. Keep game RAM/health logic
 * untouched: adjust only decoded render geometry, revalidated by the backend. */
static inline int port_video_life_fill(int x,int *y,int w,int *h)
{
    if(*y!=18 || *h!=5 || w<0 || w>144 ||
       !((x>=8 && x<=152-w) || (x>=216 && x<=360-w)))return 0;
    *y=17;*h=6;return 1;
}
/* Eight fixed TILE records, two display buffers per yellow/damage side.
 * Bit0 is ignored for untextured TILEs on the original rendering path.
 * Address/list checks precede all reads, so the selected RAM span is bounded. */
static inline int port_video_tag_life_fill(uint32_t ot,uint32_t address,unsigned char *p,int enabled)
{
    uint32_t phys=address&0x1fffffffu, offset;
    int list=port_video_ot_index(ot);
    int x,y,w,h;
    if(!enabled || phys<0x1ac6acu || phys>=0x1ac72cu)return 0;
    offset=phys-0x1ac6acu;
    if(offset%16 || list!=(offset<64?14:15) ||
       p[3]!=3 || (p[7]&0xfc)!=0x60)return 0;
    x=(int)(((p[8]|(unsigned)p[9]<<8)&2047)^1024)-1024;
    y=(int)(((p[10]|(unsigned)p[11]<<8)&2047)^1024)-1024;
    w=(int16_t)(p[12]|(unsigned)p[13]<<8);
    h=(int16_t)(p[14]|(unsigned)p[15]<<8);
    if((offset&32)?(x<216 || x>360-w):(x<8 || x>152-w))return 0;
    if(!port_video_life_fill(x,&y,w,&h))return 0;
    p[7]|=PORT_VIDEO_LIFE_FILL;return 1;
}
/* Extension supplied by the existing pinned SDL_GPU dependency patch. */
void Psyz_Sfa2SetArcadeCanvas(int enabled);
void Psyz_Sfa2SetPixelPerfectUi(int enabled);
int Psyz_Sfa2ExperimentalVideoAvailable(void);
void Psyz_Sfa2SetBatchedUploads(int on);
#endif
