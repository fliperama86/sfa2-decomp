/* Build-time native resource closure, never original-code execution or playback.
 * The importer runs the same C player and records only requested asset ranges.
 * No state snapshots, frame hashes or original program images enter the pack. */
#include "../player.h"
#include "../resources.h"
#include <stdlib.h>
#include <string.h>
#define HASH_SLOTS 65536u
struct tile_entry {uint32_t key;uint8_t *pixels;};
static struct intro_player player;
static struct intro_resources packed;
static const uint8_t *planes[2],*graphics;
static size_t sizes[2],graphics_size;
static uint8_t mask[2][0x300000];
static struct tile_entry tiles[HASH_SLOTS];
static uint16_t pixels[INTRO_VIDEO_WIDTH*INTRO_VIDEO_HEIGHT];
static int packed_mode;
static const uint8_t *read_data(void *context,unsigned plane,uint32_t address,size_t n)
{
    (void)context;
    if(plane>1 || address>sizes[plane] || n>sizes[plane]-address) return NULL;
    memset(mask[plane]+address,1,n);return planes[plane]+address;
}
static const uint8_t *read_tile(void *context,unsigned side,uint32_t code,unsigned half)
{
    uint32_t key;unsigned slot,attempt,x,y,i;size_t start,stride;struct tile_entry *entry=NULL;
    (void)context;
    if((side!=8 && side!=16 && side!=32) || code>=0x800000u || half>1 || (side!=8 && half)) return NULL;
    key=(side<<24)|(code<<1)|half;slot=(key*2654435761u)&(HASH_SLOTS-1);
    for(attempt=0;attempt<HASH_SLOTS;attempt++,slot=(slot+1u)&(HASH_SLOTS-1)) {
        entry=tiles+slot;if(entry->key==key) return entry->pixels;if(!entry->key) break;
    }
    if(attempt==HASH_SLOTS) return NULL;
    stride=side==8 ? 8u : side/2u;start=(size_t)code*(side==8 ? 64u : side*side/2u);
    if(start>graphics_size || side*stride>graphics_size-start) return NULL;
    entry->pixels=malloc(side*side);if(!entry->pixels) return NULL;entry->key=key;
    for(y=0;y<side;y++) for(x=0;x<side;x++) {
        unsigned pen=0;
        for(i=0;i<4;i++) pen|=((graphics[start+y*stride+(x/8)*4+i+(side==8 ? half*4 : 0)]>>(7-x%8))&1u)<<i;
        entry->pixels[y*side+x]=(uint8_t)pen;
    }
    return entry->pixels;
}
int intro_probe_start(const uint8_t *data,size_t data_size,const uint8_t *inlined,size_t inline_size,
                      const uint8_t *gfx,size_t gfx_size,unsigned seed)
{
    unsigned i;
    if(!data || !inlined || !gfx || data_size!=0x300000u || inline_size!=0x300000u || gfx_size!=0x1400000u) return -1;
    for(i=0;i<HASH_SLOTS;i++) free(tiles[i].pixels);
    memset(tiles,0,sizeof tiles);memset(mask,0,sizeof mask);planes[0]=data;planes[1]=inlined;sizes[0]=data_size;sizes[1]=inline_size;
    graphics=gfx;graphics_size=gfx_size;packed_mode=0;intro_player_init(&player,read_data,NULL,(uint16_t)seed);return 0;
}
int intro_probe_packed_start(const uint8_t *data,size_t size,unsigned seed)
{
    if(intro_resources_init(&packed,data,size)) return -1;
    packed_mode=1;intro_player_init(&player,intro_resources_read,&packed,(uint16_t)seed);return 0;
}
int intro_probe_step(void)
{
    int result=intro_player_step(&player);
    if(result>=0 && intro_player_render(&player,packed_mode ? intro_resources_tile : read_tile,
                                       packed_mode ? &packed : NULL,pixels,sizeof pixels/sizeof *pixels)) return -1;
    return result;
}
const char *intro_probe_error(void) {return player.world.error;}
const uint8_t *intro_probe_reads(unsigned plane) {return plane<2 ? mask[plane] : NULL;}
const uint8_t *intro_probe_tile(unsigned slot,uint32_t *key)
{
    if(slot>=HASH_SLOTS || !key || !tiles[slot].key) return NULL;
    *key=tiles[slot].key;return tiles[slot].pixels;
}
const uint8_t *intro_probe_ram(void) {return player.world.ram;}
const uint8_t *intro_probe_video(void) {return player.world.video;}
const uint8_t *intro_probe_objects(size_t *size)
{if(!size) return NULL;*size=player.display.count*8+4;return player.display.objects;}
const uint16_t *intro_probe_pixels(void) {return pixels;}
uint64_t intro_probe_frame_hash(void)
{
    size_t i;uint64_t hash=14695981039346656037ULL;
    for(i=0;i<sizeof pixels/sizeof *pixels;i++) {hash^=pixels[i];hash*=1099511628211ULL;}
    return hash;
}
