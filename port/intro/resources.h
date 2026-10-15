/* Validated sparse intro data and indexed graphics. Own the payload externally. */
#ifndef PORT_INTRO_RESOURCES_H
#define PORT_INTRO_RESOURCES_H
#include "world.h"
#define INTRO_RESOURCE_LIMIT (64u*1024u*1024u)
struct intro_resources {
    const uint8_t *payload,*spans,*tiles;
    size_t size;
    unsigned span_count,tile_count;
};
/* V3 payload only, after the file magic and SHA-256. No CPU image is loaded. */
int intro_resources_init(struct intro_resources *,const uint8_t *,size_t);
const uint8_t *intro_resources_read(void *,unsigned,uint32_t,size_t);
const uint8_t *intro_resources_tile(void *,unsigned,uint32_t,unsigned);
#endif
