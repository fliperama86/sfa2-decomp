/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size, instruction
 * scheduling and register choice. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the original; the build does not use this
 * file. The differential test next to it (func_801452ec.py, run by
 * difftest.py) compares the behavior of this C with the original code on
 * random inputs of the contract below.
 *
 * What it does (inferred, not an original name): turns a grid of tile cells
 * into textured quads. The object's sequence leads to a header describing
 * the grid; for every cell whose low 14 bits are not 0 the function takes
 * the next entry (idx) of a table of quad pairs, selected by the object's
 * kind (field_03 picks one of two sides) and by the double buffer selector,
 * clears the quad with a library call, asks two library functions for a
 * texture page word and a clut word, sets the vertex positions (the corners
 * depend on the cell's top two bits), sets the texture coordinates from the
 * low bits of the cell, and links the quad into a list.
 *
 * Contract (the roles named for the fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_03, field_09, field_0c, field_0d, pos_x,
 *     pos_y, field_5c, field_76, field_78, field_7a, field_7c and sequence;
 *     the header at sequence->field_04: byte 0 columns, byte 2 rows, byte 3
 *     a clut offset, u16 at 4 and 6 an x and y origin, then rows * columns
 *     u16 cells from offset 8; the selector data_801a27d0 (a word, 0 or 1);
 *     the pointer data_801987c8.
 *   Table data_801a4ff0: 2 sides of 40 pairs of 2 quads of 0x28 bytes. The
 *     function writes, for each non-zero cell, the quad idx of the side
 *     for the selector: bytes 4 to 6 (only when field_03 is below 2), the
 *     vertex halfwords at 0x8 to 0x22, the texture bytes at 0xc, 0xd,
 *     0x14, 0x15, 0x1c, 0x1d, 0x24, 0x25, and the halfwords at 0xe and 0x16
 *     from two callee results. Nothing else is written.
 *   field_03 must be 0, 1, 9 or 10: it picks the side (0 and 9 side 0, 1
 *     and 10 side 1). For any other value the original uses a register
 *     left over from its caller; that input is excluded. For 9 and 10 the
 *     vertical stretch dy is 0, otherwise field_5c. The quad bytes 4 to 6
 *     are set and the call to func_8015bfe8 made only for field_03 0 and 1.
 *   idx is a 16-bit counter that starts at 0 and counts the drawn cells;
 *     the contract keeps it at 40 or below (a pair per cell in the side).
 *   Callees, all replaced by recorders (the log shows each call in order
 *     with its arguments):
 *       func_8015c09c(quad)          1 argument, clears the quad
 *       func_8015bd0c(0, 0, x, y)    4 arguments, returns a value that
 *                                    is stored at 0x16
 *       func_8015bdd4(a, b)          2 arguments, returns a value stored
 *                                    at 0xe
 *       func_8015bfe8(quad, 1)       2 arguments
 *       func_8015bf34(link, quad)    2 arguments, links the quad
 *     The three that get a quad pointer also copy its 0x28 bytes into the
 *     log at every call (pointees), because the function filled it in for
 *     that call. Besides, every recorder copies the written pairs of the
 *     side in use (watch), so the order of each store against each call is
 *     compared. The two result-returning recorders return a value that the
 *     setup chose for the case.
 *   Aliasing: the object, header, sequence step and table are distinct
 *     blocks; the list base pointer is only passed on, never read through.
 *   Not reached by any input: two instruction slots of the original, at
 *     offsets 0x114 and 0x178, which round a negative value before the
 *     division by 16 and the division by 256; the cell is masked to 14
 *     bits and is never negative.
 *   Size: the build is 860 bytes against the original's 1,632; the original
 *     recomputes each quad address for every store.
 *   Reads of data_801a27d0 and the object are done once per cell here; the
 *     original reloads the selector after each call. The recorders do not
 *     change it, so the test cannot tell the two apart.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations; not original ones. */
extern PolySide data_801a4ff0[];
int func_8015bdd4(int a, int b);
void func_8015bfe8(Poly28 *quad, int flag);

void func_801452ec(Object *object) {
    Tilemap *map;
    Poly28 *quad;
    u8 side;
    s16 idx;
    s16 dy;
    int row;
    int col;
    int cell;
    int flag;
    int n;
    int x;
    int y;
    int ystep;
    int lo;
    int hi;

    if (object->field_03 == 0 || object->field_03 == 9) {
        side = 0;
    } else if (object->field_03 == 1 || object->field_03 == 10) {
        side = 1;
    }
    idx = 0;
    map = (Tilemap *)object->sequence->field_04;
    for (row = 0; row < map->rows; row++) {
        for (col = 0; col < map->cols; col++) {
            cell = map->cells[col + row * map->cols];
            flag = cell >> 14;
            n = cell & 0x3fff;
            if (n == 0) {
                continue;
            }
            lo = (n % 16) << 4;
            hi = (n / 16) << 4;
            quad = &data_801a4ff0[side].pairs[idx].polys[data_801a27d0];
            func_8015c09c(quad);
            quad->field_16 = func_8015bd0c(0, 0, (s16)object->field_76 + (n / 256) * 64, (s16)object->field_78);
            quad->field_0e = func_8015bdd4((s16)object->field_7a,
                                           (s16)object->field_7c + object->field_0d * object->field_0c + map->field_03);
            if (object->field_03 < 2) {
                quad->field_04 = 0x80;
                quad->field_05 = 0x80;
                quad->field_06 = 0x80;
                func_8015bfe8(quad, 1);
            }
            dy = object->field_5c;
            if (object->field_03 == 9 || object->field_03 == 10) {
                dy = 0;
            }
            x = object->pos_x + col * 16 - map->field_04;
            if (flag == 0) {
                y = object->pos_y + row * (dy + 16) - map->field_06;
                quad->field_08 = x;
                quad->field_0a = y;
                quad->field_10 = x + 16;
                quad->field_12 = y;
                quad->field_18 = x;
                quad->field_1a = dy + (y + 16);
                quad->field_20 = x + 16;
                quad->field_22 = dy + (y + 16);
            } else {
                y = object->pos_y + row * 16 - map->field_06;
                ystep = object->pos_y + row * (dy + 16) - map->field_06 + 16;
                quad->field_08 = x + 16;
                quad->field_0a = y;
                quad->field_10 = x;
                quad->field_12 = y;
                quad->field_18 = x + 16;
                quad->field_1a = ystep;
                quad->field_20 = x;
                quad->field_22 = ystep;
            }
            quad->field_0c = lo;
            quad->field_0d = hi;
            quad->field_14 = lo + 15;
            quad->field_15 = hi;
            quad->field_1c = lo;
            quad->field_1d = hi + 15;
            quad->field_24 = lo + 15;
            quad->field_25 = hi + 15;
            idx++;
            func_8015bf34((int)data_801987c8 + object->field_09 * 4 + 0x20, (Cmd *)quad);
        }
    }
}
