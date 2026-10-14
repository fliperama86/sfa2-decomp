/* Native C composition of the bounded Gouki/fire scene, not captured video.
 * Scene sources and explicit omissions: docs/tasks/intro-first-scene.md. */
#include "scene.h"
#include <string.h>

#define PALETTES INTRO_SEQUENCE_BYTES
#define FOREGROUND (PALETTES+576u*2u)
#define BACKGROUND (FOREGROUND+6u*INTRO_W*INTRO_FORE_H)
#define PORTRAIT (BACKGROUND+4u*256u*256u*2u)
static unsigned be16(const uint8_t *p) { return (unsigned)p[0]*256u+p[1]; }
static uint32_t be32(const uint8_t *p) { return (uint32_t)be16(p)*65536u+be16(p+2); }

int intro_scene_init(struct intro_scene *scene,const uint8_t *data,size_t size)
{
    struct intro_scene next;
    unsigned i;
    if(!scene || !data || size!=INTRO_PAYLOAD_BYTES || be16(data)!=2) return -1;
    /* This pack version is deliberately one bounded six-pose loop. Validate
     * all records, not just the first, before rendering can inspect art IDs. */
    for(i=0;i<6;i++) {
        const uint8_t *r=data+2+i*16;
        if(be16(r)!=4 || be16(r+2)!=(i==5 ? 0x8000u : 0) || be32(r+4)!=i) return -1;
        if(be32(r+8) || be32(r+12)) return -1;
    }
    if(be32(data+98)!=2) return -1;
    for(i=PALETTES;i<FOREGROUND;i+=2) if(be16(data+i)>0xfff) return -1;
    for(i=FOREGROUND;i<BACKGROUND;i++) if(data[i]>15) return -1;
    for(i=BACKGROUND;i<PORTRAIT;i+=2) if(be16(data+i)>511) return -1;
    for(i=PORTRAIT;i<INTRO_PAYLOAD_BYTES;i++) if(data[i]>15) return -1;
    memset(&next,0,sizeof next);
    next.data=data;
    if(intro_sequence_select(data,INTRO_SEQUENCE_BYTES,0,0,&next.foreground)) return -1;
    *scene=next;
    return 0;
}

/* Full-bright RGB12 to the PS1 upload format (red in the low five bits).
 * Full-bright only is this slice's boundary, not the original fade routine. */
static uint16_t color(const uint8_t *palette,unsigned index)
{
    unsigned c=be16(palette+index*2);
    unsigned r=((c>>8)&15)*17u, g=((c>>4)&15)*17u, b=(c&15)*17u;
    return (uint16_t)((r>>3)|((g>>3)<<5)|((b>>3)<<10));
}
int intro_scene_frame(struct intro_scene *scene,uint16_t *pixels)
{
    unsigned x,y,pose,phase,palette,background;
    const uint8_t *data=scene->data,*fore,*back,*portrait;
    if(scene->tick>=INTRO_SCENE_TICKS) return 0;
    pose=be32(data+scene->foreground.record+4);
    fore=data+FOREGROUND+pose*INTRO_W*INTRO_FORE_H;
    background=(scene->tick/6)&3u;
    back=data+BACKGROUND+background*256u*256u*2u;
    portrait=data+PORTRAIT;
    phase=scene->tick&3u;
    palette=phase<=2 ? phase : 4-phase;
    for(y=0;y<INTRO_H;y++) {
        /* 44e0's eight-entry triangular rowscroll, and 4510's phase step.
         * The original -256 scroll plus viewport +64 plus row base 192
         * cancel. Duplicated 256-wide maps wrap the sample horizontally. */
        unsigned row=(y+scene->tick)&7u;
        unsigned dx=row<4 ? row : 7-row;
        for(x=0;x<INTRO_W;x++) {
            unsigned v=be16(back+((y+16)*256+((x+dx)&255u))*2);
            uint16_t c=(v&15)==15 ? 0 : color(data+PALETTES+32,v);
            v=portrait[y*512+x];
            if(v!=15) c=color(data+PALETTES+32+1024,palette*16+v);
            if(y>=INTRO_H-INTRO_FORE_H) {
                v=fore[(y-(INTRO_H-INTRO_FORE_H))*INTRO_W+x];
                if(v!=15) c=color(data+PALETTES,v);
            }
            pixels[y*INTRO_W+x]=c;
        }
    }
    scene->tick++;
    /* Already validated, no runtime-ROM branch can escape this small table. */
    return intro_sequence_tick(data,INTRO_SEQUENCE_BYTES,&scene->foreground) ? -1 : 1;
}
