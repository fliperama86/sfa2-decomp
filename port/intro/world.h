/* Native intro state. Big-endian fields preserve original byte/word aliases;
 * this is game data, not CPU registers, an instruction decoder or a VM. */
#ifndef PORT_INTRO_WORLD_H
#define PORT_INTRO_WORLD_H
#include <stddef.h>
#include <stdint.h>
#include "video.h"

enum intro_table_plane { INTRO_DATA, INTRO_INLINE_DATA };
enum intro_field {
    IA_ACTIVE=0,IA_VISIBLE=1,IA_KIND=2,IA_SUBTYPE=3,IA_STATE=4,IA_STEP=5,
    IA_CLASS=8,IA_LAYER=9,IA_BYPASS=10,IA_FLIP=11,IA_PALETTE_MODE=12,
    IA_PALETTE=13,IA_CAMERA=14,IA_SHAKE=15,IA_X=16,IA_Y=20,
    IA_PRIORITY=24,IA_BANK=26,IA_SEQUENCE=28,IA_SCALE_X=32,IA_SCALE_Y=34,
    IA_ANGLE=36,IA_ROW=38,IA_TICKS=50,IA_FLAGS=52,IA_LINK=54,
    IA_TIMER=58,IA_DELAY=60,IA_VX=64,IA_VY=68,IA_AX=72,IA_AY=76,
    IA_VARIANT=90
};
enum intro_global {
    IG_EXIT=0,IG_SCENE=4,IG_DELAY=6,IG_STEP=8,IG_TIMER=10,IG_AUX=12,
    IG_INTERVAL=14,IG_SIGNAL=18,IG_FLAGS=20,IG_LINK=22,
    IG_PALETTE=32,IG_SCROLL=34,IG_ROW=46,IG_VIDEO=48,IG_LAYERS=50,
    IG_PRIORITY=58,IG_SEED=174,IG_STAGE=256,IG_PALETTE_SET=296,
    IG_CAMERA1=512,IG_CAMERA2=640,IG_CAMERA3=768,
    IG_FADE_ACTIVE=0x4f41,IG_FADE_WAIT=0x4f42,IG_FREE=0x6116
};
struct intro_world;
typedef const uint8_t *(*intro_table_reader)(void *,unsigned,uint32_t,size_t);
typedef int (*intro_actor_step)(struct intro_world *,unsigned,uint16_t);
struct intro_world {
    uint8_t ram[65536],video[INTRO_VIDEO_MEMORY];
    intro_table_reader read;
    void *read_context;
    intro_actor_step actor_step;
    int (*present)(struct intro_world *);
    int (*title)(struct intro_world *);
    const char *error;
    unsigned rendered,sound_command,fade_mode,fade_target,fade_delay;
};
static inline uint16_t iw16(const uint8_t *p) { return (uint16_t)((unsigned)p[0]*256u+p[1]); }
static inline uint32_t iw32(const uint8_t *p) { return (uint32_t)iw16(p)*65536u+iw16(p+2); }
static inline void ip16(uint8_t *p,unsigned v) { p[0]=(uint8_t)(v>>8);p[1]=(uint8_t)v; }
static inline void ip32(uint8_t *p,uint32_t v) { ip16(p,v>>16);ip16(p+2,v); }
static inline uint8_t *ig(struct intro_world *w,unsigned offset) { return w->ram+((0x8000u+offset)&65535u); }
static inline int32_t is16(unsigned v) { return (v&65535u)<32768u ? (int32_t)(v&65535u) : (int32_t)(v&65535u)-65536; }
static inline unsigned gw(struct intro_world *w,unsigned field) { return iw16(ig(w,field)); }
static inline uint32_t gl(struct intro_world *w,unsigned field) { return iw32(ig(w,field)); }
static inline void sw(struct intro_world *w,unsigned field,unsigned v) { ip16(ig(w,field),v); }
static inline void sl(struct intro_world *w,unsigned field,uint32_t v) { ip32(ig(w,field),v); }
static inline int countdown(struct intro_world *w,unsigned field) { unsigned n=(gw(w,field)-1u)&65535u;sw(w,field,n);return n==0; }
const uint8_t *intro_world_table(struct intro_world *,unsigned,uint32_t,size_t);
uint8_t *intro_world_video(struct intro_world *,uint32_t,size_t);
void intro_world_reset_actors(struct intro_world *);
uint16_t intro_world_alloc(struct intro_world *,unsigned pool);
int intro_world_delete(struct intro_world *,unsigned pool,uint16_t actor);
uint8_t *intro_world_actor(struct intro_world *,unsigned pool,uint16_t actor);
void intro_world_update_actors(struct intro_world *,unsigned pool);
void intro_world_clear_maps(struct intro_world *);
void intro_world_load_map(struct intro_world *,unsigned layer,unsigned index,uint32_t destination);
void intro_world_fill_map(struct intro_world *,unsigned layer,unsigned pen,unsigned palette,uint32_t destination);
void intro_world_palette_copy(struct intro_world *,uint32_t source,uint32_t destination,unsigned colors,int brightness);
void intro_world_palettes(struct intro_world *,unsigned full);
unsigned intro_world_random(struct intro_world *);
int intro_director_step(struct intro_world *);
#endif
