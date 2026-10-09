/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the resident image; the build does not use this file.
 * The differential test next to it (difftest.py with func_8011d74c.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): builds the draw commands
 * of a strip of 16-byte cells, once per change. The object's field_02 and a
 * frame distance d (from func_801250c0 and a table) pick a size value and
 * a stream header from tables, as in func_8011db78. A cache of three small
 * tables, indexed by the object's side, the column and the buffer selector,
 * keeps the last header pointer, size and a byte of the second object
 * (field_0d); when all three are unchanged the function returns. Otherwise
 * it stores them, then for each of "count" cells (the header's first
 * halfword) fills the cell's two bytes at 0xc and 0xd (the size value * 16
 * and the position in a row of 16), its halfword at 0xe (a library result)
 * and registers the cell with a library call. Every 16th cell it also
 * builds a primitive for the row and registers that, and the size value
 * moves on to the next one in a chain table; a last primitive is built for
 * a partial row.
 *
 * Contract (what the code reads and writes; roles are inferred):
 *   Arguments: a0 = object, a1 = column (16 bits, 0 to 3 here), a2 = frame
 *     number (16 bits), a3 = second object (field_0d, field_7a, field_7c
 *     are read). Upper halves of a1 and a2 are zero. No return value.
 *   Callees, all replaced by recorders (the first one is a small game
 *     function whose tables are not built here; the others are Sony's
 *     library, from 0x80157090 on):
 *       func_801250c0(object, frame): 2 arguments, returns a block the
 *         setup made, of which the first halfword is read;
 *       func_80158150(list, 2): 2 arguments, no result used;
 *       func_8015bdd4(x, y): 2 arguments, returns a 32-bit value of which
 *         16 bits are stored (the same value in every call of a case);
 *       func_8015bf34(list, cell): 2 arguments, no result used;
 *       func_8015bd0c(0, 0, v, 0): 4 arguments, returns a 32-bit value of
 *         which 16 bits are passed on;
 *       func_80158a2c(prim, 1, 1, v, 0): 5 arguments, no result used.
 *   Reads: object field_02, side (0xa6); data_801900f8[field_02]; the tables
 *     data_80183d9c, data_80183dc4 and data_80183dec at [field_02][d] with d
 *     from 0 to 9 (and [d - 1] when the flag entry is 0); data_801a27d0
 *     (low halfword, 0 or 1); the cache tables data_80185c84, data_80185cc4
 *     and data_80185ce4; data_80185504 (16 bytes per size value) and the
 *     chain table data_80183c90; the header's first halfword.
 *   Returns at once, writing nothing but the recorders' log, in these
 *     cases: d is 10 or more; the size entry, the header pointer or the
 *     header count is 0; the flag entry is 0 and the size entry or the
 *     header pointer differs from the one before; the cache already holds
 *     the three values.
 *   Writes: the three cache cells; one cell of 16 bytes per header count at
 *     data_801987cc + (side * 320 + column * 80 + 640) * 16 + selector *
 *     0x5000 (bytes 0xc, 0xd, halfword 0xe); the log. The primitives and
 *     the list slot are only addressed, not written, by this function.
 *   Watched at every recorded call (copied into the log): the object (0x394
 *     bytes), the second object (0xac bytes), the three cache tables whole
 *     (64, 32 and 16 bytes) and 656 bytes of the cell buffer from its first
 *     cell. Pointee: the second argument of func_8015bf34 (a cell the
 *     function filled, or a primitive), 16 bytes. The other pointer
 *     arguments (list, primitive for func_80158a2c) are not filled by the
 *     function.
 *   Aliasing: the object, second object, header and call block are distinct
 *     blocks of the setup; the table cells read for one case are distinct.
 *   Excluded inputs: size values above 110 (the table data_80185504 has 112
 *     rows before the next table, and the chain table's entries lead only
 *     to rows below 111), and a count above 40 (so that the cell buffer
 *     stays clear of the globals above it); side 0 or 1, column 0 to 3 and
 *     selector 0 or 1 (the sizes of the cache and buffer tables).
 *   Not reached by any input: one instruction slot of the original, at
 *     offset 0x2ac, which adds 15 to a value before it is divided by 16 when
 *     the value is negative; the value is a sum of non-negative products
 *     (side, column and the sizes of the arrays are small non-negative
 *     numbers), so no input reaches it.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Quad data_80185c04[][8];
extern u8 data_80185504[][16];
void func_80158150(Quad *list, int n);
int func_8015bdd4(int x, int y);

void func_8011d74c(void *a, int b, int c, Block172 *d) {
    Object *o = a;
    u16 column = b;
    u16 *r;
    u16 idx, dist, size, count, sel;
    u16 *hdr;
    Quad *list;
    Cell16 *cell;
    Prim *prim;
    int i, col;

    idx = o->field_02;
    r = func_801250c0(o, c & 0xffff);
    dist = data_801900f8[idx] - *r + 1;
    if (dist >= 10)
        return;
    size = data_80183d9c[idx][dist];
    if (size == 0)
        return;
    hdr = (u16 *)data_80183dec[idx][dist];
    if (hdr == 0)
        return;
    count = *hdr;
    if (count == 0)
        return;
    if (data_80183dc4[idx][dist] == 0) {
        if (size != data_80183d9c[idx][dist - 1])
            return;
        if ((u32)hdr != data_80183dec[idx][dist - 1])
            return;
    }
    sel = data_801a27d0;
    if (data_80185c84[o->side][column][sel] == (int)hdr
        && data_80185cc4[o->side][column][sel] == size
        && data_80185ce4[o->side][column][sel] == d->field_0d)
        return;
    data_80185c84[o->side][column][sel] = (int)hdr;
    data_80185cc4[o->side][column][sel] = size;
    data_80185ce4[o->side][column][sel] = d->field_0d;
    list = &data_80185c04[sel][o->side * 4 + column];
    cell = (Cell16 *)(data_801987cc + (o->side * 320 + column * 80 + 640) * 16 + sel * 0x5000);
    prim = (Prim *)(data_8019056c + sel * 0x1e0) + (o->side * 320 + column * 80) / 16;
    func_80158150(list, 2);
    col = 0;
    for (i = 0; i < count; i++) {
        cell->field_0c = size << 4;
        cell->field_0d = col << 4;
        cell->field_0e = func_8015bdd4(d->field_7a, d->field_7c + d->field_0d + data_80185504[size][col]);
        func_8015bf34((int)list, (Cmd *)cell);
        cell++;
        col = (col + 1) & 0xf;
        if (col == 0) {
            func_80158a2c(prim, 1, 1, func_8015bd0c(0, 0, ((size >> 4) << 6) + 0x180, 0), 0);
            func_8015bf34((int)list, (Cmd *)prim);
            prim++;
            size = data_80183c90[size];
        }
    }
    if (col != 0) {
        func_80158a2c(prim, 1, 1, func_8015bd0c(0, 0, ((size >> 4) << 6) + 0x180, 0), 0);
        func_8015bf34((int)list, (Cmd *)prim);
    }
}
