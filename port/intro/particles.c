/* Project-authored C, never a runtime instruction interpreter. */
#include "particles.h"
static uint32_t word(const uint8_t *p) { return (uint32_t)p[0]*256u+p[1]; }
static uint32_t longword(const uint8_t *p) { return word(p)*65536u+word(p+2); }

uint8_t intro_particle_random(uint16_t *seed)
{
    unsigned high=((uint16_t)(*seed*3u))>>8;
    uint8_t low=(uint8_t)(*seed+high);
    *seed=(uint16_t)(high*256u+low);
    return low;
}
int intro_particle_spawn(uint16_t *interval, uint16_t *seed, unsigned phase,
                         unsigned available, struct intro_particle_spawn *out)
{
    unsigned subtype, layer=10, priority=0xa000;
    *interval=(uint16_t)(*interval-1u);
    if(*interval) return 0;
    *interval=(uint16_t)((intro_particle_random(seed)&3u)+1u);
    subtype=*interval;
    if(phase==1 && subtype>2) {
        subtype+=2; layer=subtype==5 ? 8 : 4; priority=0xe000;
    } else if(phase==2) {
        subtype=subtype<=2 ? subtype+2 : 1; layer=0; priority=0xe000;
    }
    if(available<=5) return 0;
    out->subtype=(uint16_t)subtype;
    out->layer=(uint16_t)layer;
    out->priority=(uint16_t)priority;
    return 1;
}
int intro_particle_init(struct intro_particle_motion *p, unsigned subtype,
                        uint16_t *seed, const uint8_t *tables)
{
    unsigned at;
    if(subtype<1 || subtype>6) return -1;
    at=(subtype-1u)*16u+(intro_particle_random(seed)&8u);
    p->vx=longword(tables+at); p->vy=longword(tables+at+4);
    at=96u+(intro_particle_random(seed)&127u)*4u;
    p->x=word(tables+at)<<16; p->y=word(tables+at+2)<<16;
    p->sway=0; p->acceleration=0xffffc000u; p->limit=0x24000u;
    return 0;
}
int intro_particle_move(struct intro_particle_motion *p)
{
    uint32_t magnitude;
    p->x+=p->vx+p->sway;
    p->y+=p->vy;
    magnitude=p->sway&0x80000000u ? 0u-p->sway : p->sway;
    if(magnitude>=p->limit) p->acceleration=0u-p->acceleration;
    p->sway+=p->acceleration;
    return (uint16_t)((p->x>>16)+32u)>448u ||
           (uint16_t)((p->y>>16)+32u)>288u;
}
