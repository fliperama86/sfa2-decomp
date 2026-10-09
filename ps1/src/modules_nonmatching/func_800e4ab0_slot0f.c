/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register allocation and
 * instruction order. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (func_800e4ab0_slot0f.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): draws a window of a tile
 * map as sprite records. The map layer c has a pixel width (field_58), a
 * pixel height (field_5c), a data block (field_50) and a scroll position
 * (field_12 horizontal, field_14 a 16.16 vertical value, whose negation is
 * used). The map is stored in blocks of 32 by 16 cells of 4 bytes (a cell
 * word, then a texture-page word). The function walks 15 rows and `count`
 * columns from the scroll position; a row outside the map is skipped, and
 * the column index wraps with the map's width. For every cell whose first
 * halfword is not field_1e (compared as a signed halfword, so a field_1e
 * of 0x8000 or more never equals any cell) it fills the next record: x and
 * y from the grid position minus the scroll remainder, u and v from bits of
 * the cell, the command word from its top bits (arithmetic shift), the
 * texture page from the second halfword, increments the record counter
 * data_800f7264_slot0f (16 bits), and links the record into the list head
 * selected by c->field_8a in the array data_801987c8 points at.
 *
 * Contract (what the code reads and writes; the roles named are inferred):
 *   Arguments: a0 = layer, a1 = first record (records are 0x1c bytes),
 *     a2 = count (only its low 16 bits, as signed). No return value.
 *   Reads: the layer's field_12, field_14, field_1e, field_50, field_58,
 *     field_5c, field_8a; the cells it reaches; the counter; data_801987c8
 *     and the list head word it selects.
 *   Writes: for each drawn cell one record (offsets 0, 4, 0x14 to 0x1b),
 *     the list head word, and the counter.
 *   Aliasing: layer, map data, records and list array are distinct blocks.
 *   Inputs excluded: a width (field_58) other than 512 or 1024 and a
 *     height (field_5c) above 256 (the setup's map block holds that much);
 *     count above 40 as a signed halfword (the setup's record block holds
 *     15 * 40 records). Counts at or below 0 are included.
 *   Not reached by any input: none; every slot is executed. For a negative
 *     vertical tile index the original computes the shift twice (a test
 *     whose first result is overwritten at once); the C computes it once as
 *     an arithmetic shift, which is what the final value is.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_800f7264_slot0f;

void func_800e4ab0_slot0f(Chan *c, Strip1c *buf, int count) {
    int row;
    int col;
    u16 ty;
    u16 ix;
    int xoff;
    int yoff;
    int fy;
    int h;
    u32 wmask;
    u32 *ot;
    u16 *p;
    u16 cell;
    u8 *base;
    u32 x;

    h = (s16)(c->field_5c >> 4);
    wmask = (c->field_58 >> 4) - 1;
    xoff = c->field_12 & 0xf;
    x = ((s16)c->field_12 & (c->field_58 - 1)) >> 4;
    fy = (0x100000 - c->field_14) >> 16;
    yoff = fy & 0xf;
    ty = (s16)fy >> 4;
    ot = (u32 *)data_801987c8 + c->field_8a;
    for (row = 0; row < 15; row++, ty++) {
        ix = x;
        if ((s16)ty < 0 || (s16)ty >= h) {
            continue;
        }
        base = (u8 *)c->field_50 + ((ty >> 4) * ((c->field_58 >> 9) << 10)) * 2 + (ty & 0xf) * 128;
        for (col = 0; col < (s16)count; col++) {
            p = (u16 *)(base + ((ix >> 5) << 11) + ((ix & 0x1f) << 2));
            if (*p != (s16)c->field_1e) {
                cell = *p;
                buf->field_1a = p[1];
                buf->field_14 = col * 16 - xoff;
                buf->field_16 = row * 16 - yoff;
                buf->field_18 = (cell << 3) & -8;
                buf->field_19 = ((s16)cell >> 2) & -8;
                buf->field_04 = ((s16)cell >> 10) + 0xe1000000;
                data_800f7264_slot0f++;
                ((PrimTag *)buf)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)buf;
                buf++;
            }
            ix = (ix + 1) & wmask;
        }
    }
}
