/* Native expansion of imported intro metasprites into object records. */
#ifndef PORT_INTRO_SPRITES_H
#define PORT_INTRO_SPRITES_H
#include <stddef.h>
#include <stdint.h>

struct intro_sprite_block {
    int16_t x,y;
    uint16_t code,attributes;
};
struct intro_sprite_art {
    unsigned mode; /* Original data format: 0, 2, 4, 6, 8, or 10 (no draw). */
    unsigned cells,blocks;
    const struct intro_sprite_block *block;
};
struct intro_sprite_style {
    uint16_t x,y; /* Resolved hardware anchor, before ten-bit wrapping. */
    uint16_t x_flags,y_flags;
    uint8_t bypass,flip,palette_mode,palette;
    int16_t expand_x,expand_y;
    uint8_t rotate;
    int16_t matrix[4]; /* Imported signed coefficients, used only if rotate. */
};
struct intro_sprite_list {
    uint8_t *objects; /* Big-endian eight-byte records. */
    size_t capacity,count;
    unsigned cells_left;
};
/* 1: emitted; 0: no-draw format or original cell-budget rejection; -1: invalid
 * input. Rejection leaves the destination/list unchanged. Mode 8 plus bypass
 * is outside the observed intro contract and is explicitly unsupported. */
int intro_sprites_append(struct intro_sprite_list *list,
                         const struct intro_sprite_art *art,
                         const struct intro_sprite_style *style);
#endif
