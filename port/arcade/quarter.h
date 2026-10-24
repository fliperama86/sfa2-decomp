/* One quarter of a 32 x 32 pixel arcade tile as a 16 x 16 pixel tile's bytes: the rule that sprite_tiles.c (the effect sprites of the
 * stage files) and stage.c (page slots of the stage files) share. A 32 pixel tile is 512 bytes, rows of 16 bytes (four groups of four
 * bitplane bytes of 8 pixels); a 16 pixel tile is 128 bytes, rows of 8 bytes. Project-authored, no byte of either game. */
#ifndef ARCADE_QUARTER_H
#define ARCADE_QUARTER_H

#include <stdint.h>
#include <string.h>

/* Quarter q (0 top left, 1 top right, 2 bottom left, 3 bottom right) of the 32 pixel tile at t32, as 128 bytes in the 16 pixel layout. */
static inline void arcade_quarter(const uint8_t *t32, unsigned q, uint8_t out[128])
{
    unsigned y;
    for (y = 0; y < 16; y++) memcpy(out + y * 8u, t32 + (y + 16u * (q >> 1)) * 16u + 8u * (q & 1u), 8);
}

#endif
