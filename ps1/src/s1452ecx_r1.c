/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* Inferred declarations; not original ones. */
extern PolySide data_801a4ff0[];
void func_8015bfe8(Poly28 *quad, int flag);

/* What it does (inferred, not an original name): turns a grid of tile cells
 * into textured quads. The object's sequence leads to a header describing
 * the grid; for every cell whose low 14 bits are not 0 the function takes
 * the next entry (idx) of a table of quad pairs, selected by the object's
 * kind (table_index picks one of two sides) and by the double buffer selector,
 * clears the quad with a library call, asks two library functions for a
 * texture page word and a clut word, sets the vertex positions (the corners
 * depend on whether the cell's top two bits are 0), sets the texture
 * coordinates from the low bits of the cell, and links the quad into a
 * list.
 */
void draw_tilemap_as_textured_quads(Object *object) {
    Tilemap *m;
    int col;
    u8 lo;
    u8 hi;
    s16 px;
    s16 py;
    s16 idx;
    int flag;
    u8 side;
    int tile_size;
    int l;
    int h;
    int row;
    int n;
    s16 dy;

    /* Four separate tests, one per kind; with 0 and 9 joined, 1 and 10 joined, this function differs from the original in 6 instruction slots. */
    if (object->table_index == 0) {
        side = 0;
    } else if (object->table_index == 1) {
        side = 1;
    } else if (object->table_index == 9) {
        side = 0;
    } else if (object->table_index == 10) {
        side = 1;
    }
    idx = 0;
    m = (Tilemap *)object->sequence->data_ptr;
    px = object->pos_x;
    py = object->pos_y;
    for (row = 0; row < m->rows; row++) {
        for (col = 0; col < m->cols; col++) {
            /* Stand-in, not recovered source: one stand-in at 2 places, the byte offset from the pointer cast to u8 that reads the cell, which is m->cells[col + row * m->cols], reproduces the original's choice of registers. Without it this function differs from the original in 1 instruction slots. */
            flag = (*(u16 *)(((u8 *)m) + ((col + (row * m->cols)) << 1) + 8)) >> 14;
            /* The second place of the stand-in above. The figure is the one above: the cell read as m->cells[col + row * m->cols] at both places. */
            n = (*(u16 *)(((u8 *)m) + ((col + (row * m->cols)) << 1) + 8)) & 0x3fff;
            if (n != 0) {
                /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
                /* Without it this function differs from the original in 5 instruction slots. */
                do {
                    lo = n % 16;
                } while (0);
                hi = n / 16;
                func_8015c09c(&data_801a4ff0[side].pairs[idx].polys[data_801a27d0]);
                /* The (u8) cast narrows the quotient (at most 63) and makes the shift unsigned; without it this function differs from the original in 1 instruction slots. */
                data_801a4ff0[side].pairs[idx].polys[data_801a27d0].tpage = func_8015bd0c(0, 0, (s16)object->base_x + ((u8)(n / 256)) * 64, (s16)object->base_y);
                data_801a4ff0[side].pairs[idx].polys[data_801a27d0].clut = func_8015bdd4((s16)object->spawn_x, (s16)object->spawn_y + object->image_row * object->field_0c + m->page_offset);
                if (object->table_index < 2) {
                    /* Stand-in, not recovered source: one stand-in at 27 places, the byte offset from the pointer cast to u8 (side * 0xc80 + idx * 0x50, which is the address of data_801a4ff0[side].pairs[idx]) written out for each store of the quad, reproduces the original's order of instructions. Without it this function differs from the original in 322 instruction slots. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].r0 = 0x80;
                    /* The second place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].g0 = 0x80;
                    /* The third place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].b0 = 0x80;
                    func_8015bfe8(&data_801a4ff0[side].pairs[idx].polys[data_801a27d0], 1);
                }
                dy = object->health;
                if (object->table_index == 9 || object->table_index == 10) {
                    dy = 0;
                }
                if (flag == 0) {
                    /* The fourth place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].x0 = px + col * 16 - m->field_04;
                    /* The fifth place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].y0 = py + row * (dy + 16) - m->field_06;
                    /* The sixth place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].x1 = px + col * 16 - m->field_04 + 16;
                    /* The seventh place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].y1 = py + row * (dy + 16) - m->field_06;
                    /* The eighth place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].x2 = px + col * 16 - m->field_04;
                    /* The ninth place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].y2 = dy + 16 + (py + row * (dy + 16) - m->field_06);
                    /* The tenth place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].x3 = px + col * 16 - m->field_04 + 16;
                    /* The 11th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].y3 = dy + 16 + (py + row * (dy + 16) - m->field_06);
                } else {
                    /* Stand-in, not recovered source: the local tile_size, which only holds the constant 16 where the original shows no register of its own for it, reproduces the original's choice of registers. Without it this function differs from the original in 11 instruction slots. */
                    tile_size = 16;
                    /* The 12th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].x0 = px + col * tile_size - (m->field_04 - tile_size);
                    /* The 13th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].y0 = py + row * tile_size - m->field_06;
                    /* The 14th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].x1 = px + col * tile_size - m->field_04;
                    /* The 15th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].y1 = py + row * tile_size - m->field_06;
                    /* The 16th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].x2 = px + col * tile_size - (m->field_04 - tile_size);
                    /* The 17th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].y2 = py + row * (dy + tile_size) - (m->field_06 - tile_size);
                    /* The 18th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].x3 = px + col * tile_size - m->field_04;
                    /* The 19th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                    ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].y3 = py + row * (dy + tile_size) - (m->field_06 - tile_size);
                }
                /* Stand-in, not recovered source: the local l, which is only a copy of lo, and the local h, which is only a copy of hi, reproduce the original's choice of registers. Without it this function differs from the original in 53 instruction slots. */
                /* The 20th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].u0 = (l = lo) << 4;
                /* The 21st place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].v0 = (h = hi) << 4;
                /* The 22nd place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].u1 = (l << 4) + 15;
                /* The 23rd place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].v1 = h << 4;
                /* The 24th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].u2 = l << 4;
                /* The 25th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].v2 = (h << 4) + 15;
                /* The 26th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].u3 = (l << 4) + 15;
                /* The 27th place of the stand-in above. The figure is the one above: the byte offset written as data_801a4ff0[side].pairs[idx] at all 27 places. */
                ((Poly28 *)(((u8 *)data_801a4ff0) + (side * 0xc80) + (idx * 0x50)))[data_801a27d0].v3 = (h << 4) + 15;
                func_8015bf34((int)((u8 *)data_801987c8 + (object->ot_depth * 4 + 0x20)), (Cmd *)&data_801a4ff0[side].pairs[idx].polys[data_801a27d0]);
                idx++;
            }
        }
    }
}
