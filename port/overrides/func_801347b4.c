/*
 * The port's C for func_801347b4. It runs in the PC program in place of the
 * C of the unit d347b4_r1.c; the PS1 build does not use this file, and the
 * unit keeps the exact C.
 *
 * What is different from the unit's C: the local `col` is set to 0 at its
 * declaration. The unit declares it without a value.
 *
 * Why: after its two loops the function calls func_801350c0(kind, col) when
 * the kind is 2 or 3. When the map has no rows the loops do not run, and
 * `col` was never set. On the console the register that holds it (s7, first
 * written at 8013483c, inside the loop over the rows) then still has what
 * the caller left in it, and the low byte of that goes to the callee (andi
 * a1,s7,0xff at 80134e5c): a value that depends on the caller's code, read
 * from the listing, not tested. Compiled for a PC, a local without a value
 * is whatever the compiler's code happens to hold, and nothing ties it to
 * the console's value. The readable C that stood for this function before it
 * was exact passed 0 in that case, and this file keeps that. Whether a map
 * without rows reaches this function with the kind 2 or 3 in the game is not
 * known; with a row or more, `col` is the number of columns after the loops
 * in both files, and this file and the unit do the same.
 *
 * What the function does (inferred): see the comment above the function.
 *
 * Contract (the roles named for the fields are inferred):
 *   Arguments: a0 = a, a1 = map (pointer to the header), a2 = c (x base,
 *     only its low 16 bits count), a3 = d (y base, a full word), the fifth
 *     argument e (low 16 bits count) and the sixth f (low 8 bits count) on
 *     the stack at sp + 0x10 and sp + 0x14. No return value.
 *   Reads: the header and cells; data_801a27d0 (low halfword);
 *     data_80188d04; data_80188d28 after func_80134e94; data_801987c8.
 *   Writes: for each non-zero tile, the page word and the corner and
 *     texture fields of one record (offsets 0x08 to 0x25), data_80188d28
 *     (set to 0), data_80188d04 (plus 1); whatever func_80134e94 and
 *     func_801350c0 write.
 *   Variables the callees reach by address (the function's own text names
 *     none of them; what the callees reach is inferred): the byte table at
 *     data_80171bf8 + 0x60 (0x108 bytes of which the callees read the bytes
 *     around +0x5f to +0x68 and what those index), the halfword at offset
 *     0xc6 and the byte at 0xd8 of player_left and of player_right (the
 *     object 0x394 bytes after player_left), and the stretch of data from
 *     data_80188d28 - 0x20.
 *   Callees: func_80134e94 and func_801350c0 are game code that only reads
 *     and writes variables (inferred); they run as the original code in both
 *     runs (the setup randomizes the variables they read). func_8015bf34 (a
 *     library call, inferred) is replaced by a recorder taking two arguments
 *     and returning 0.
 *   Watched by the test at every call of func_8015bf34: the words holding
 *     data_80188d04 and data_80188d28, and the first 48 records that the run
 *     can write (the whole table, 11520 bytes, at every one of up to 144
 *     calls does not fit the test's memory); the record handed over is also
 *     copied as that call's pointee (10 words). Records after the 48th of a
 *     run are seen at their own call and in the final state only.
 *   Aliasing: the header, the list array and the record table are distinct
 *     blocks; nothing else is written.
 *   Excluded: data_80188d04 plus the number of non-zero tiles must be at
 *     most 144 (the record buffer ends there; beyond it the original would
 *     overwrite the variables after the table). The buffer selector (the low
 *     halfword of data_801a27d0) is 0, 1 or 0xffff (-1). Rows equal to 0
 *     with the kind 2 or 3: the original then passes its caller's untouched
 *     register as the column count to func_801350c0, a value no C can
 *     express (read from the original's listing, not tested); with rows 0
 *     this C passes 0, so the test does not use this case.
 *   Not reached by any input: four instruction slots of the original, at
 *     offsets 0xec, 0xf0, 0x104 and 0x134, which adjust a division or a
 *     shift for a negative value; the tile is masked to 14 bits, so it and
 *     its quotient are never negative.
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

extern PolyBlk data_80186004[];
extern s16 data_80188d04;
void func_80134e94(u8 a, u8 b);
void func_801350c0(u8 a, u8 b);

/*
 * What it does (inferred, not an original name): draws a tile map as sprite
 * records in the table data_80186004, one record per non-zero tile at the next
 * free index data_80188d04 of the buffer data_801a27d0, and hands each record
 * to func_8015bf34 with a pointer into the list array at data_801987c8. The
 * kind changes the page word and the corner layout. Kinds 2 and 3 call
 * func_80134e94 for each tile and func_801350c0 once at the end.
 * Parameters: the kind (low 8 bits count), the map header (an address, columns
 * at byte 0, rows at byte 2, then rows * columns halfword cells), the x base
 * (low 16 bits count), the y base, the list index (low 16 bits count) and the
 * page base (low 8 bits count).
 */
void func_801347b4(int kind_arg, u32 map_addr, int x_arg, int y_base, int list_arg, int page_arg) {
    /* The list index as 16 bits and the page base as 8 bits; the shared prototype has int. */
    u16 list = list_arg;
    u8 page = page_arg;
    /* The kind, tested as bytes below (only its low 8 bits count). */
    int kind = kind_arg;
    /* The map header, from the address argument. */
    Tilemap *map = (Tilemap *)map_addr;
    /* The x base as 16 bits. */
    u16 x0 = x_arg;
    s16 idx = data_801a27d0;
    int row;
    int col = 0; /* the one change against the unit: see the header */
    int dx = 1;
    int dy = 1;
    s16 tile;
    int flag;
    int u1;
    int v1;
    u8 cu;
    u8 cv;
    for (row = 0; row < map->rows; row++) {
        for (col = 0; col < map->cols; col++) {
            /* Plain map->cells[col + row * map->cols] differs in 1 instruction line (the operand order of one addition). */
            flag = *(u16 *)((u8 *)map + ((col + row * map->cols) << 1) + 8) >> 14;
            tile = map->cells[col + row * map->cols] & 0x3fff;
            if (tile != 0) {
                cu = tile % 16;
                cv = tile / 16 % 16;
                data_80186004[idx].polys[data_80188d04].tpage = tile / 256 + 13;
                data_80188d28 = 0;
                /* The byte tests on kind in this function stay in these forms: kind == 2 || kind == 3 here
                   differs in 21 instruction lines, (u8)kind != 5 for the flag test below in 118, and
                   kind - 2 for the test after the loops in 2. */
                if ((unsigned)((kind + 0xfe) & 0xff) < 2) {
                    func_80134e94(kind, col);
                }
                if ((u8)kind < 4) {
                    data_80186004[idx].polys[data_80188d04].clut = (page + map->page_offset + data_80188d28) * 64 + 0x7807;
                } else if ((u8)kind == 6) {
                    data_80186004[idx].polys[data_80188d04].clut = 0x7f07;
                } else {
                    data_80186004[idx].polys[data_80188d04].clut = (page + map->page_offset) * 64 + 0x7806;
                }
                if ((flag == 0 && (kind & 0xff) != 5) || tile == 0x3b || tile == 0x3e) {
                    /* The record is addressed as (&...)->field in these stores; the plain data_80186004[idx].polys[data_80188d04].x0 form differs in 212 instruction lines for all 32 of them, and in 50 to 202 for any one group of eight. */
                    (&data_80186004[idx].polys[data_80188d04])->x0 = (x0 + col * 16) - map->field_04;
                    (&data_80186004[idx].polys[data_80188d04])->y0 = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->x1 = (x0 + col * 16) - map->field_04 + 0x10;
                    (&data_80186004[idx].polys[data_80188d04])->y1 = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->x2 = (x0 + col * 16) - map->field_04;
                    (&data_80186004[idx].polys[data_80188d04])->y2 = (y_base + row * 16) - map->field_06 + 0x10;
                    (&data_80186004[idx].polys[data_80188d04])->x3 = (x0 + col * 16) - map->field_04 + 0x10;
                    (&data_80186004[idx].polys[data_80188d04])->y3 = (y_base + row * 16) - map->field_06 + 0x10;
                } else if ((u8)kind != 5) {
                    (&data_80186004[idx].polys[data_80188d04])->x0 = (x0 + col * 16) - (s16)(map->field_04 - 0x10);
                    (&data_80186004[idx].polys[data_80188d04])->y0 = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->x1 = (x0 + col * 16) - map->field_04;
                    (&data_80186004[idx].polys[data_80188d04])->y1 = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->x2 = (x0 + col * 16) - (s16)(map->field_04 - 0x10);
                    (&data_80186004[idx].polys[data_80188d04])->y2 = (y_base + row * 16) - (s16)(map->field_06 - 0x10);
                    (&data_80186004[idx].polys[data_80188d04])->x3 = (x0 + col * 16) - map->field_04;
                    (&data_80186004[idx].polys[data_80188d04])->y3 = (y_base + row * 16) - (s16)(map->field_06 - 0x10);
                } else {
                    (&data_80186004[idx].polys[data_80188d04])->x0 = map->field_04 + (x0 - col * 16) + 0xf;
                    (&data_80186004[idx].polys[data_80188d04])->y0 = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->x1 = (x0 - col * 16) + map->field_04 - 1;
                    (&data_80186004[idx].polys[data_80188d04])->y1 = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->x2 = map->field_04 + (x0 - col * 16) + 0xf;
                    (&data_80186004[idx].polys[data_80188d04])->y2 = (y_base + row * 16) - (s16)(map->field_06 - 0x10);
                    (&data_80186004[idx].polys[data_80188d04])->x3 = (x0 - col * 16) + map->field_04 - 1;
                    (&data_80186004[idx].polys[data_80188d04])->y3 = (y_base + row * 16) - (s16)(map->field_06 - 0x10);
                }
                if ((u8)kind == 4 || (u8)kind == 5) {
                    dx = 1;
                    dy = 0;
                }
                if (tile == 0x3b || tile == 0x3e) {
                    dx = 0;
                    dy = 1;
                }
                u1 = cu << 4;
                v1 = cv << 4;
                (&data_80186004[idx].polys[data_80188d04])->u0 = u1;
                (&data_80186004[idx].polys[data_80188d04])->v0 = v1;
                (&data_80186004[idx].polys[data_80188d04])->u1 = u1 + 0x10 - dx;
                (&data_80186004[idx].polys[data_80188d04])->v1 = v1;
                (&data_80186004[idx].polys[data_80188d04])->u2 = u1;
                (&data_80186004[idx].polys[data_80188d04])->v2 = v1 + 0x10 - dy;
                (&data_80186004[idx].polys[data_80188d04])->u3 = u1 + 0x10 - dx;
                (&data_80186004[idx].polys[data_80188d04])->v3 = v1 + 0x10 - dy;
                func_8015bf34((int)data_801987c8 + list * 4, (Cmd *)&data_80186004[idx].polys[data_80188d04]);
                data_80188d04++;
            }
        }
    }
    /* With no rows col is still the 0 of its declaration here; in the unit it is not set (see the header). */
    if ((unsigned)((kind - 2) & 0xff) < 2) {
        func_801350c0(kind, col);
    }
}
