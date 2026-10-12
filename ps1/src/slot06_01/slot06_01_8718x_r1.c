/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f29dc_slot06_01[2][300];

/* Draws the second layer: tile records of the visible 16-pixel cells into the
   primitive list, one row of 25 cells per pass, and a last partial row when
   the vertical position is not a multiple of 16 (inferred).
   The tile mask and the column counter cc are 16-bit locals, and the locals r
   and yy are kept, as in the model draw_tile_layer_sprites_with_partial_row_slot06_04. */
void draw_tile_layer_sprites_with_partial_row_slot06_01(void) {
    Slot06Draw *c = (Slot06Draw *)data_801aa5d4;
    int q;
    int x;
    int pos;
    int x12;
    s16 y;
    s16 t;
    s16 row;
    u32 col;
    int i;
    int k;
    s16 n;
    u16 *line;
    u16 *e;
    Slot06Tile *rec;
    Slot06Tile *tp;
    u32 *ot;
    s16 blank;
    s16 rows;
    s16 sy;
    s16 sx;
    u16 mask;
    s16 yy;
    s16 r;
    s16 tile;
    q = (c->end_x_fixed - c->start_x_fixed)
        * (c->field_4c - c->range_start)
        / (c->range_end - c->range_start);
    blank = c->blank_value;
    x = c->base_x_fixed;
    x -= 0x100000;
    pos = -x;
    t = (pos >> 16) - game_state.scroll_y;
    pos &= 0xffff;
    pos |= t << 16;
    c->pos_fixed = pos;
    y = pos >> 16;
    sy = y & 0xf;
    x12 = (q + c->start_x_fixed) >> 16;
    sx = x12 & 0xf;
    col = (x12 & (c->width - 1)) >> 4;
    if (y >= 0) {
        row = y >> 4;
    } else {
        row = (y - 15) / 16;
    }
    rows = c->height >> 4;
    mask = (c->width >> 4) - 1;
    rec = data_801f29dc_slot06_01[data_801a27d0];
    n = (u32)(c->field_4c - ((pos >> 16) & -16)) >> 4;
    ot = (u32 *)((u8 *)data_801987c8 + c->ot_index * 4);
    for (i = 0; i < n; i++, row++) {
        u16 cc = col;
        if (row < 0 || row > rows) {
            continue;
        }
        r = row;
        line = c->pixel_data + ((u16)r >> 4) * ((c->width >> 9) << 10) + ((r & 0xf) << 6);
        yy = (i << 4) - sy;
        for (k = 0; k < 25; k++) {
            e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
            tile = *e;
            if (tile != blank) {
                tp = rec;
                tp->sprt.x = (k << 4) - sx;
                tp->sprt.y = yy;
                tp->sprt.u = (tile << 3) & -8;
                tp->sprt.v = (tile >> 2) & -8;
                tp->sprt.clut = e[1];
                tp->mode[0] = 0xe1000000 + (tile >> 10);
                ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)rec;
                rec++;
            }
            cc = mask & (cc + 1);
        }
    }
    if (c->field_4c & 0xf) {
        u16 cc = col;
        if (row > 0 || row <= rows) {
            r = row;
            line = c->pixel_data + ((u16)r >> 4) * ((c->width >> 9) << 10) + ((r & 0xf) << 6);
            yy = (i << 4) - sy;
            for (k = 0; k < 25; k++) {
                e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
                tile = *e;
                if (tile != blank) {
                    tp = rec;
                    tp->sprt.x = (k << 4) - sx;
                    tp->sprt.y = yy;
                    tp->sprt.u = (tile << 3) & -8;
                    tp->sprt.v = (tile >> 2) & -8;
                    tp->sprt.clut = e[1];
                    tp->mode[0] = 0xe1000000 + (tile >> 10);
                    ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                    ((PrimTag *)ot)->addr = (u32)rec;
                    rec++;
                }
                cc = mask & (cc + 1);
            }
        }
    }
}
