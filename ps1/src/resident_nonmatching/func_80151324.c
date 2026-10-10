/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice (read from the original's listing, not tested). The
 * exact owner of the bytes in the PS1 build stays the raw bytes of the
 * executable; the build does not use this file. The differential test next
 * to it (difftest.py) compares the behavior of this C with the original
 * code on random inputs of the contract below.
 *
 * What it does (inferred, not original names): draws a box of tiles as
 * sprite records. The object's sequence step names a frame record of the
 * table; its field_05 selects (1-based) a tile map. The map is a byte
 * string: width, height, two offsets, then width * height tile bytes,
 * height rows of width cells each. A non-zero tile byte (bits 0 to 3 the
 * tile column, bits 4 to 6 the tile row, bit 7 a horizontal flip) becomes
 * one 40-byte record of the buffer chosen by data_801a27d0: texture
 * coordinates, and screen positions that are spaced 16 apart per cell
 * (mirrored when bit 0 of the object's field_0b is set). The x positions
 * start from the object's x position less box_margin and the map's first
 * offset, the y positions from data_801aa5ea and the map's second offset.
 * Each record is handed to func_8015bf34 together with the list head 0x38
 * bytes into data_801987c8.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = table of frame records (16 bytes each).
 *     No return value.
 *   Returns at once, writing nothing, when game_state.field_64 is not 0,
 *     when the frame record's field_05 is 0, or when width * height is 0.
 *   Reads: the object's sequence (field 0x18) and its halfword at 0xa, the
 *     object's field_02, field_0b and the halfword at 0x12; the frame
 *     record's field_05; the pointer data_8017ff68[field_05 - 1] and the
 *     map it points at; box_margin and data_801aa5ea (halfwords); the
 *     buffer selector data_801a27d0; the pointer data_801987c8.
 *   Writes: for each non-zero tile, in the buffer data_8018947c, at byte
 *     offset object.field_02 * 65 * 32 + n * 80 + selector * 40 (n counts
 *     the non-zero tiles from 0) the bytes 0xc, 0xd, 0x14, 0x15, 0x1c, 0x1d,
 *     0x24, 0x25 and the halfwords at 0x8, 0xa, 0x10, 0x12, 0x18, 0x1a,
 *     0x20, 0x22; other bytes of the record are left alone.
 *   Callee: func_8015bf34 (two arguments: the list head address and the
 *     record's address) is replaced by a recorder; it is above the start of
 *     Sony's library in the image. What it does is outside the test.
 *     The log copies the 10 words of the record at each call and watches
 *     the first 2900 words of data_8018947c. The original reads field_0b
 *     once, before the loops, and data_801a27d0 for each cell (read from
 *     the original's listing, not tested); in half of
 *     the cases the recorder stores new values for both (as a callee might;
 *     inferred) at its first call.
 *   Aliasing: the object, frame table, sequence step and tile map are
 *     blocks of their own; the record buffer and the list pointer's target
 *     lie in the image and the setup's blocks.
 *   Excluded: object.field_02 is kept small and the map small so that the
 *     records stay inside RAM (the original faults past its end; inferred).
 *   Not reached: none planned; the coverage line says.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 *data_8017ff68[];

void func_80151324(Object *object, FrameRecord *table) {
    FrameRecord *rec = &table[object->sequence->frame_index];
    int map_index = rec->field_05 - 1;
    u8 *map;
    u8 width;
    u8 height;
    int tex_x;
    int tex_y;
    int base;
    int left;
    u16 x0;
    u16 x1;
    u16 x2;
    u16 x3;
    int y0;
    int y1;
    int col;
    int row;
    int n;
    u8 flags;

    if (game_state.field_64 != 0 || map_index == -1) {
        return;
    }
    map = data_8017ff68[map_index];
    width = *map++;
    height = *map++;
    if (width * height == 0) {
        return;
    }
    tex_x = *map++;
    tex_y = *map++;

    left = object->pos_x - box_margin[0];
    x0 = left + (tex_x - 16);
    x1 = left - tex_x;
    x2 = left + tex_x;
    x3 = left - (tex_x - 16);
    y0 = data_801aa5ea[0] - (tex_y - 200);
    y1 = data_801aa5ea[0] - (tex_y - 216);
    base = object->field_02 * 65;
    flags = object->field_0b;

    n = 0;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            int tile = *map++;
            Poly28 *q;
            int lo;
            int hi;
            int near;
            int far;
            int xa;
            int xb;

            if (tile == 0) {
                continue;
            }
            lo = (tile & 0xf) << 4;
            hi = tile & 0x70;
            near = lo;
            far = lo + 15;
            if (tile & 0x80) {
                near = lo + 15;
                far = lo;
            }
            if ((flags & 1) == 0) {
                xa = x1 + (col << 4);
                xb = x3 + (col << 4);
            } else {
                xa = x2 - (col << 4);
                xb = x0 - (col << 4);
            }
            q = &((Poly28 *) ((u8 *)data_8018947c + (base << 5) + n * 80))[data_801a27d0];
            q->field_0c = near;
            q->field_0d = hi;
            q->field_14 = far;
            q->field_15 = hi;
            q->field_1c = near;
            q->field_1d = hi + 15;
            q->field_24 = far;
            q->field_25 = hi + 15;
            q->field_08 = xa;
            q->field_0a = y0 + (row << 4);
            q->field_10 = xb;
            q->field_12 = y0 + (row << 4);
            q->field_18 = xa;
            q->field_1a = y1 + (row << 4);
            q->field_20 = xb;
            q->field_22 = y1 + (row << 4);
            n++;
            func_8015bf34((int) ((u8 *) data_801987c8 + 0x38), (Cmd *) q);
        }
    }
}
