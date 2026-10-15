/* Project-authored composition for the intro's three scrolling tile planes
 * and object list. Contract and hardware-reference provenance: video-contract.md. */
#include "video.h"
#include <string.h>

static unsigned word(const uint8_t *p) { return (unsigned)p[0]*256u+p[1]; }

uint16_t intro_video_color(uint16_t color)
{
    unsigned brightness=15u+2u*(color>>12);
    unsigned red=((color>>8)&15u)*17u*brightness/45u;
    unsigned green=((color>>4)&15u)*17u*brightness/45u;
    unsigned blue=(color&15u)*17u*brightness/45u;
    return (uint16_t)((red>>3)|((green>>3)<<5)|((blue>>3)<<10));
}

unsigned intro_video_map_index(unsigned layer,unsigned column,unsigned row)
{
    unsigned shift;
    if(layer>=3) return 4096;
    shift=5u-layer;
    return (row&((1u<<shift)-1u))+((column&63u)<<shift)
           +((row&(63u^((1u<<shift)-1u)))<<6);
}

static int valid(const struct intro_video *v,const uint16_t *pixels,size_t n)
{
    unsigned i,seen=0;
    if(!v || !pixels || n<INTRO_VIDEO_WIDTH*INTRO_VIDEO_HEIGHT || !v->memory ||
       v->memory_size!=INTRO_VIDEO_MEMORY || !v->tile ||
       v->palette_base>INTRO_VIDEO_MEMORY-4096u || (v->palette_base&1u) ||
       v->row_base>INTRO_VIDEO_MEMORY-2048u || (v->row_base&1u) ||
       v->object_count>INTRO_VIDEO_OBJECTS || (v->object_count && !v->objects) ||
       (v->video_control&0x8000u)) return 0;
    for(i=0;i<3;i++) if(v->map_base[i]>INTRO_VIDEO_MEMORY-16384u ||
                         (v->map_base[i]&16383u)) return 0;
    /* The original intro uses each layer exactly once, including the unused
     * CPS1 sprite position. Reject ambiguous orders instead of guessing. */
    for(i=0;i<4;i++) seen|=1u<<((v->layer_control>>(6u+2u*i))&3u);
    return seen==15u;
}

static int background(const struct intro_video *v,unsigned layer,
                      uint16_t *pixels,uint8_t *priority,const uint16_t *palette)
{
    unsigned side=8u<<layer,mask=side*64u-1u,y;
    unsigned rank=(v->priority_control>>(4u*(layer+1u)))&15u;
    if(!(v->layer_control&(2u<<layer))) return 0;
    if(layer && !(v->video_control&(2u<<layer))) return 0;
    for(y=0;y<INTRO_VIDEO_HEIGHT;y++) {
        unsigned sy=(y+16u+v->scroll_y[layer])&mask;
        unsigned scroll=v->scroll_x[layer],x=0;
        if(layer==1 && (v->video_control&1u))
            scroll+=word(v->memory+v->row_base+2u*((y+16u+v->row_offset)&1023u));
        while(x<INTRO_VIDEO_WIDTH) {
            unsigned sx=(x+64u+scroll)&mask;
            unsigned column=sx/side,index=intro_video_map_index(layer,column,sy/side);
            const uint8_t *entry=v->memory+v->map_base[layer]+4u*index;
            unsigned code=word(entry),attr=word(entry+2),local_x=sx&(side-1u);
            unsigned local_y=sy&(side-1u),count=side-local_x,j;
            const uint8_t *tile;
            if(layer==0) code+=0x20000u;
            else if(layer==1) code+=0x10000u;
            else code=(code&0x3fffu)+0x4000u;
            tile=v->tile(v->tile_context,side,code,layer==0 ? column&1u : 0u);
            if(!tile) return -1;
            if(count>INTRO_VIDEO_WIDTH-x) count=INTRO_VIDEO_WIDTH-x;
            if(attr&64u) local_y=side-1u-local_y;
            for(j=0;j<count;j++) {
                unsigned tx=local_x+j,pen,at=y*INTRO_VIDEO_WIDTH+x+j;
                if(attr&32u) tx=side-1u-tx;
                pen=tile[local_y*side+tx];
                if(pen>15u) return -1;
                if(pen!=15u) {
                    pixels[at]=palette[(layer+1u)*512u+(attr&31u)*16u+pen];
                    priority[at]=(uint8_t)rank;
                }
            }
            x+=count;
        }
    }
    return 0;
}

static int object(const struct intro_video *v,const uint8_t *entry,
                  uint16_t *pixels,uint8_t *priority,const uint16_t *palette)
{
    unsigned ox=word(entry),oy=word(entry+2),attr=word(entry+6);
    unsigned code=word(entry+4)+((oy&0x6000u)<<3),rank=(ox>>13)&7u;
    unsigned width=((attr>>8)&15u)+1u,height=(attr>>12)+1u,cy,cx;
    unsigned xoff=64u-v->object_x_offset,yoff=16u-v->object_y_offset;
    if(attr&128u) { xoff+=v->object_x_offset; yoff+=v->object_y_offset; }
    for(cy=0;cy<height;cy++) for(cx=0;cx<width;cx++) {
        unsigned sx=(ox+16u*cx+xoff)&1023u,sy=(oy+16u*cy+yoff)&1023u;
        unsigned tx=(attr&32u) ? width-1u-cx : cx;
        unsigned ty=(attr&64u) ? height-1u-cy : cy;
        uint32_t cell;
        const uint8_t *tile;
        unsigned x,y;
        if(sx>=448u || sx+16u<=64u || sy>=240u || sy+16u<=16u) continue;
        /* In the unflipped blocked format the low tile nibble wraps without
         * carrying. Flipped blocks use ordinary addition on this hardware. */
        cell=(attr&96u) ? code+tx+16u*ty
                        : (code&~15u)+((code+tx)&15u)+16u*ty;
        tile=v->tile(v->tile_context,16,cell,0);
        if(!tile) return -1;
        for(y=0;y<16;y++) for(x=0;x<16;x++) {
            unsigned px=sx+x,py=sy+y,pen,at;
            if(px<64u || px>=448u || py<16u || py>=240u) continue;
            pen=tile[((attr&64u) ? 15u-y : y)*16u+((attr&32u) ? 15u-x : x)];
            if(pen>15u) return -1;
            if(pen==15u) continue;
            at=(py-16u)*INTRO_VIDEO_WIDTH+px-64u;
            if(priority[at]&128u) continue;
            if(rank>priority[at]) pixels[at]=palette[(attr&31u)*16u+pen];
            /* Even an object hidden by a background claims this pixel and
             * masks earlier objects. Transparent pens never claim pixels. */
            priority[at]|=128u;
        }
    }
    return 0;
}

int intro_video_render(const struct intro_video *v,uint16_t *pixels,size_t n)
{
    uint8_t priority[INTRO_VIDEO_WIDTH*INTRO_VIDEO_HEIGHT];
    uint16_t palette[2048];
    unsigned i;
    size_t remaining;
    if(!valid(v,pixels,n)) return -1;
    memset(pixels,0,INTRO_VIDEO_WIDTH*INTRO_VIDEO_HEIGHT*sizeof *pixels);
    memset(priority,0,sizeof priority);
    for(i=0;i<2048;i++) palette[i]=intro_video_color((uint16_t)word(v->memory+v->palette_base+2u*i));
    for(i=0;i<4;i++) {
        unsigned layer=(v->layer_control>>(6u+2u*i))&3u;
        if(layer && background(v,layer-1u,pixels,priority,palette)) return -1;
    }
    remaining=v->object_count;
    while(remaining) if(object(v,v->objects+8u*--remaining,pixels,priority,palette)) return -1;
    return 0;
}
