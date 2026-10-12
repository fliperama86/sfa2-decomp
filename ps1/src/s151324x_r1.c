/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern u8 *data_8017ff68[];

/* What it does (inferred, not original names): draws a box of tiles as
 * sprite records. The object's sequence step names a frame record of the
 * table; its value_b selects (1-based) a tile map. The map is a byte
 * string: width, height, two offsets, then width * height tile bytes,
 * height rows of width cells each. A non-zero tile byte (bits 0 to 3 the
 * tile column, bits 4 to 6 the tile row, bit 7 a horizontal flip) becomes
 * one 40-byte record of the buffer chosen by data_801a27d0: texture
 * coordinates, and screen positions that are spaced 16 apart per cell
 * (mirrored when bit 0 of the object's facing_side is set). The x positions
 * start from the object's x position less box_margin and the map's first
 * offset, the y positions from data_801aa5ea and the map's second offset.
 * Each record is handed to func_8015bf34 together with the list head 0x38
 * bytes into data_801987c8.
 */
void draw_object_tile_grid_prims(Object *object, FrameRecord *table) {
    u16 x0, x1, x2, x3;
    u8 flags;
    u16 width;
    int base;
    FrameRecord *rec = &table[object->sequence->frame_index];
    int idx = rec->value_b;
    u8 *p;
    u8 height;
    u16 n;
    int c0, c1, ca, cb, cc;
    unsigned row;
    unsigned col;
    int y0, y1;
    int ycur;
    int ty;
    u8 *pb;
    int f2;

    if (game_state.field_64 != 0) {
        return;
    }
    idx -= 1;
    if (idx == -1) {
        return;
    }
    p = data_8017ff68[idx];
    height = *p++;
    width = *p++;
    if (height * width == 0) {
        return;
    }
    c0 = *p++;
    c1 = *p++;
    {
        int xx = (u16)object->pos_x - box_margin[0];
        ca = c0 - 16;
        x0 = xx + ca;
        x1 = xx - c0;
        x2 = c0 + xx;
        x3 = xx - ca;
    }
    cb = c1 - 200;
    y0 = data_801aa5ea[0] - cb;
    cc = c1 - 216;
    y1 = data_801aa5ea[0] - cc;
    flags = object->facing_side;
    f2 = object->field_02;
    n = 0;
    for (row = 0; row < width; row++) {
        /* Stand-in, not recovered source: the sum written in two halves below, which has the value y1 + row * 16, reproduces the original's order of instructions. Without it this function differs from the original in 10 instruction slots. */
        ycur = (y1 + row * 8) + row * 8;
        /* Stand-in, not recovered source: the local pb, which is only a second name of the address of data_8018947c, reproduces the original's order of instructions. Without it this function differs from the original in 4 instruction slots. */
        for (pb = (u8 *)data_8018947c, col = 0, base = f2 * 65; col < height; col++) {
            int off = base << 5;
            u16 yy;
            u8 ch = *p++;
            ty = row << 4;
            yy = ycur;
            if (ch != 0) {
                int a2 = ch & 0xf;
                int t0 = (ch & 0x70) >> 4;
                if ((ch & 0x80) == 0) {
                    /* The address of every store below is written as a sum with two products, base * 32 and data_801a27d0 * 40, in the original's order. Written as off + n * 80 + data_801a27d0 * 40, this function differs from the original in 4 instruction slots. */
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->u0 = (a2 << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->v0 = (t0 << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->u1 = ((a2 << 4) + 15);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->v1 = (t0 << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->u2 = (a2 << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->v2 = ((t0 << 4) + 15);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->u3 = ((a2 << 4) + 15);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->v3 = ((t0 << 4) + 15);
                } else {
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->u0 = ((a2 << 4) + 15);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->v0 = (t0 << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->u1 = (a2 << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->v1 = (t0 << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->u2 = ((a2 << 4) + 15);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->v2 = ((t0 << 4) + 15);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->u3 = (a2 << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->v3 = ((t0 << 4) + 15);
                }
                if ((flags & 1) == 0) {
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->x0 = x1 + (col << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->y0 = y0 + ty;
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->x1 = x3 + (col << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->y1 = y0 + ty;
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->x2 = x1 + (col << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->y2 = yy;
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->x3 = x3 + (col << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->y3 = yy;
                } else {
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->x0 = x2 - (col << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->y0 = y0 + ty;
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->x1 = x0 - (col << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->y1 = y0 + ty;
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->x2 = x2 - (col << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->y2 = yy;
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->x3 = x0 - (col << 4);
                    ((Poly28 *)&((u8 *)data_8018947c)[n * 80 + base * 32 + data_801a27d0 * 40])->y3 = yy;
                }
                /* Stand-in, not recovered source: the conversion of pb to a 64-bit integer in the argument below, which changes no value, reproduces the original's choice of registers. Without it this function differs from the original in 2 instruction slots. */
                /* The argument's address is formed as an integer sum in the order in which the original adds its parts; as pointer arithmetic the compiler regroups it. This assumes flat addresses: a pointer converted to a wide unsigned integer and back is that pointer, moved by the bytes added. It is not portable C in general. The integer has 64 bits, so that it holds a pointer of the PC build whole. */
                func_8015bf34((int)((u8 *)data_801987c8 + 0x38), (Cmd *)&((Poly28 *)(off + (n * 80 + (unsigned long long)pb)))[data_801a27d0]);
                n++;
            }
        }
    }
}
