/* Reconstructed scheduling + bounded adapter, see particles-contract.md. */
#include "embers.h"
#include <string.h>
#define IMAGES (INTRO_EMBER_TABLE_BYTES+INTRO_EMBER_SEQUENCE_BYTES)
static unsigned be16(const uint8_t *p) { return (unsigned)p[0]*256u+p[1]; }
static uint32_t be32(const uint8_t *p) { return (uint32_t)be16(p)*65536u+be16(p+2); }

int intro_embers_init(struct intro_embers *s, const uint8_t *data, size_t size)
{
    struct intro_embers next;
    unsigned type,i;
    const uint8_t *seq;
    if(!s || !data || size!=INTRO_EMBER_BYTES) return -1;
    seq=data+INTRO_EMBER_TABLE_BYTES;
    for(type=0;type<6;type++) {
        unsigned start=12+type*100, count=type==0 ? 5 : 6;
        if(be16(seq+type*2)!=start) return -1;
        for(i=0;i<count;i++) {
            const uint8_t *r=seq+start+i*16;
            if(be16(r)!=(i+1==count ? 4095u : 8u) ||
               be16(r+2)!=(i+1==count ? 0x8000u : 0) ||
               be32(r+4)!=type*6+i || be32(r+8) || be32(r+12)) return -1;
        }
        if(be32(seq+start+count*16)!=start+(count-1)*16) return -1;
        for(i=count*16+4;i<100;i++) if(seq[start+i]) return -1;
    }
    for(i=IMAGES;i<INTRO_EMBER_BYTES;i++) if(data[i]>15) return -1;
    for(i=IMAGES+5*256;i<IMAGES+6*256;i++) if(data[i]!=15) return -1;
    memset(&next,0,sizeof next);
    next.data=data;next.seed=0x1234;next.interval=4;next.available=INTRO_EMBER_SLOTS;
    for(i=0;i<INTRO_EMBER_SLOTS;i++) next.free_slots[i]=(uint8_t)i;
    *s=next;
    return 0;
}
int intro_embers_frame(struct intro_embers *s, uint16_t *pixels,
                        const uint16_t palette[16])
{
    struct intro_particle_spawn spawn;
    const uint8_t *seq=s->data+INTRO_EMBER_TABLE_BYTES;
    unsigned i;
    if(intro_particle_spawn(&s->interval,&s->seed,2,s->available,&spawn)) {
        struct intro_ember *p=&s->slots[s->free_slots[--s->available]];
        p->state=1; p->subtype=(uint8_t)spawn.subtype;
    }
    for(i=0;i<INTRO_EMBER_SLOTS;i++) {
        struct intro_ember *p=&s->slots[i];
        const uint8_t *image;
        unsigned x,y,pose;
        int left,top;
        if(p->state==0) continue;
        if(p->state==6) {
            p->state=0;s->free_slots[s->available++]=(uint8_t)i;continue;
        }
        if(p->state==1) {
            if(intro_particle_init(&p->motion,p->subtype,&s->seed,s->data) ||
               intro_sequence_select(seq,INTRO_EMBER_SEQUENCE_BYTES,0,p->subtype-1,&p->sequence)) return -1;
            p->state=2;
        } else {
            if(intro_particle_move(&p->motion)) p->state=6;
            if(intro_sequence_tick(seq,INTRO_EMBER_SEQUENCE_BYTES,&p->sequence)) return -1;
        }
        pose=be32(seq+p->sequence.record+4);
        image=s->data+IMAGES+pose*256u;
        /* Mode-0 one-cell sprites, original camera 0 and (-8,-8) layout.
         * Hardware wraps the anchor to ten bits before viewport clipping. */
        left=(int)(((p->motion.x>>16)+64u-8u)&1023u)-64;
        top=(int)((256u-(p->motion.y>>16)-8u)&1023u)-16;
        for(y=0;y<16;y++) for(x=0;x<16;x++) {
            int px=left+(int)x,py=top+(int)y;
            unsigned pen=image[y*16+x];
            if(pen!=15 && px>=0 && px<384 && py>=0 && py<224)
                pixels[py*384+px]=palette[pen];
        }
    }
    return 0;
}
