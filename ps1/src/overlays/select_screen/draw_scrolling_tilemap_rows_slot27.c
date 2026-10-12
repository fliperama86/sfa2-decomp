/* Reconstruction. Names/roles inferred, not original symbols. */
/*
 * What it does (inferred, not an original name): draws a scrolling tile map
 * as sprite records. The scroll record (data_8002f0c8_slot27) holds a
 * horizontal scroll position (halfword at 0x12), a vertical one (word at
 * 0x14, 16.16), a pointer to the map and its width. It lowers the scroll
 * value at 0x12 by 8 when the value at 0x2a is smaller, and stops when that
 * value plus 0x40 is outside 0 to 0x1c0. Otherwise it walks 8 map rows and
 * 0x30 columns of 4-byte cells (a tile halfword and a texture page
 * halfword); each cell different from the "empty" value at 0x1e gets a
 * 0x1c-byte record in the buffer chosen by data_801a27d0 (position, texture
 * coordinates, command word, texture page), linked into the list head
 * selected by the byte at 0x8a of the scroll record.
 */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* The scroll record, as far as the function uses it. Inferred, not an
   original declaration. */
typedef struct {
    u8 pad00[0x12];
    s16 field_12;
    s32 field_14;
    u8 pad18[6];
    s16 field_1e;
    u8 pad20[0xa];
    s16 field_2a;
    u8 pad2c[0x24];
    u16 *field_50;
    u8 pad54[4];
    u32 field_58;
    u32 field_5c;
    u8 pad60[0x2a];
    u8 field_8a;
    u8 pad8b[9];
} Slot27Scroll;

/* A record of the table: a list link word, a command word, then position,
   texture coordinates and texture page at 0x14. Inferred from the code. */
typedef struct {
    u32 link;
    u32 cmd;
    u8 pad08[0xc];
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    u16 tpage;
} Slot27Prim;

extern Slot27Recb388 data_8002b388_slot27[2][0x118];
extern Slot27Recf0c8 data_8002f0c8_slot27;

void draw_scrolling_tilemap_rows_slot27(void) {
    Slot27Scroll *scroll = (Slot27Scroll *)&data_8002f0c8_slot27;
    Slot27Prim *prim;
    u32 *ot;
    u16 *row;
    u16 *cell;
    s16 tile;
    s16 line;
    int r;
    int i;
    s16 col;
    int xoff;
    s16 lines;
    int col0;
    int yoff;
    s16 empty;
    s16 vy;
    s16 sx;

    if (scroll->field_2a < scroll->field_12) {
        scroll->field_12 -= 8;
    }
    sx = scroll->field_12 + 0x40;
    if ((u16)sx <= 0x1c0) {
        xoff = sx & 7;
        lines = scroll->field_5c >> 3;
        vy = (scroll->field_14 - 0x100000) >> 16;
        empty = scroll->field_1e;
        yoff = vy & 7;
        col0 = (sx & (scroll->field_58 - 1)) >> 3;
        /* The plain spelling `vy >> 3` differs in 60 instruction lines, `vy / 8` in 46;
           the original has the three-arm division by 8 of a signed value. */
        /* Merged into one statement (`... / 8 + 2`) it differs in 23 instruction lines. */
        line = (vy < 0 ? vy - 7 : vy) / 8;
        line += 2;
        prim = (Slot27Prim *)data_8002b388_slot27[data_801a27d0];
        ot = (u32 *)data_801987c8 + scroll->field_8a;
        for (r = 2; r < 10; r++, line++) {
            /* Set before the row test: inside it, 2 instruction lines differ. */
            col = col0;
            if (line >= 0 && line < lines) {
                row = scroll->field_50 + ((u16)line >> 5) * ((scroll->field_58 >> 9) << 12) + ((line & 0x1f) << 7);
                for (i = 0; i < 0x30; i++, col++) {
                    if (col >= 0 && col < 0x40) {
                        /* Division of a column index known to be 0 to 0x3f by 64: the
                           readable version calls this a dead correction; without it 31
                           instruction lines differ. */
                        cell = row + (col / 64) * 0x1000 + (col & 0x3f) * 2;
                        if (*cell != empty) {
                            tile = *cell;
                            prim->x = i * 8 - xoff;
                            prim->y = r * 8 - yoff - 8;
                            prim->u = (tile << 3) & 0xf8;
                            prim->v = (tile >> 2) & 0xf8;
                            prim->tpage = cell[1] + 0x37dd;
                            prim->cmd = 0xe1000000 + (tile >> 10);
                            ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                            ((PrimTag *)ot)->addr = (u32)prim;
                            prim++;
                        }
                    }
                }
            }
        }
    }
}
