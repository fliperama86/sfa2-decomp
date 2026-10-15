#include "../world.h"
#include <string.h>
static struct intro_world world;
static const uint8_t *source;
static size_t source_size;
static const uint8_t *read_source(void *context,unsigned plane,uint32_t address,size_t n)
{
    (void)context;
    return plane==INTRO_DATA && address>=0x200000 && address-0x200000<=source_size && n<=source_size-(address-0x200000) ? source+address-0x200000 : NULL;
}
const char *intro_maps_test(const uint8_t *input,size_t size,uint8_t *video,unsigned layer,uint32_t destination,unsigned visible)
{
    memset(&world,0,sizeof world);world.read=read_source;source=input;source_size=size;memcpy(world.video,video,sizeof world.video);
    intro_map_write(&world,layer,0x200000,destination,visible);memcpy(video,world.video,sizeof world.video);return world.error;
}
