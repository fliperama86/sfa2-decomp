/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size and instruction
 * order. The PS1 build keeps the original bytes of the resident executable
 * for it and does not use this file. The differential test next to it
 * (difftest.py with func_801347b4.py) compares the behavior of this C with
 * the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): draws a tile map as
 * sprite records in a table of two buffers of 144 records of 40 bytes
 * (data_80186004). The map is a header (columns at byte 0, rows at byte 2,
 * a texture page offset at byte 3, an x and a y origin as halfwords at 4
 * and 6) followed by rows * columns halfword cells from offset 8. A cell's
 * low 14 bits are the tile (0 is skipped), its top two bits a flag. For each
 * tile a record is filled in at the next free index (data_80188d04) of the
 * buffer data_801a27d0 (low 16 bits, signed): its page word, the four
 * corners (positions x, y), the texture coordinates of the tile (from its
 * low and next four bits), and it is handed to func_8015bf34 together with
 * a pointer into the list array at data_801987c8, indexed by e. Only the
 * low 8 bits of the kind (a) count. The kind changes the page word (kinds 0
 * to 3, kind 6, or any other) and the corner layout: the plain layout for
 * flag 0 (any kind but 5) and for the tiles 0x3b and 0x3e, a layout with
 * the horizontal sides swapped for flag not 0 (any kind but 5), a mirrored
 * layout for kind 5. The right and lower texture coordinates of a tile are
 * its origin plus 16 minus dx (right) and minus dy (lower); dx and dy are
 * both 1 at the start of the call. Kinds 4 and 5 set dx = 1 and dy = 0
 * (width shortened by one, height full); tiles 0x3b and 0x3e set dx = 0
 * and dy = 1 (width full, height shortened by one), after the kind rule,
 * so for these tiles the tile rule wins. The values persist to the
 * following tiles, rows included. Kinds 2 and 3 call func_80134e94 for
 * each tile (before the page word, which then adds data_80188d28) and
 * func_801350c0 once at the end, with the column count of the last row.
 *
 * Contract (the roles named for the fields are inferred):
 *   Arguments: a0 = a, a1 = map (pointer to the header), a2 = c (x base,
 *     only its low 16 bits count), a3 = d (y base, a full word), the fifth
 *     argument e (low 16 bits count) and the sixth f (low 8 bits count) on
 *     the stack at sp + 0x10 and sp + 0x14. No return value.
 *   Reads: the header and cells; data_801a27d0 (low halfword);
 *     data_80188d04; data_80188d28 after func_80134e94; data_801987c8.
 *   Writes: for each non-zero tile, the page word and the corner and
 *     texture fields of one record (offsets 0x0c to 0x25), data_80188d28
 *     (set to 0), data_80188d04 (plus 1); whatever func_80134e94 and
 *     func_801350c0 write. data_80188d04 is read once per tile because
 *     nothing the function calls changes it (inferred).
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
 *     the C passes 0, so the test does not use this case.
 *   Not reached by any input: four instruction slots of the original, at
 *     offsets 0xec, 0xf0, 0x104 and 0x134, which adjust a division or a
 *     shift for a negative value; the tile is masked to 14 bits, so it and
 *     its quotient are never negative.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declarations that the published headers lack (inferred types, not
   original declarations). */
extern PolyBlk data_80186004[];
extern s16 data_80188d04;
void func_80134e94(u8 a, u8 b);
void func_801350c0(u8 a, u8 b);

void func_801347b4(int a, u32 b, int c, int d, int e, int f) {
    Tilemap *map = (Tilemap *)b;
    u8 kind = a;
    s16 idx = *(u16 *)&data_801a27d0;
    int dx = 1;
    int dy = 1;
    int row;
    int col = 0;
    int cell;
    int flag;
    int tile;
    int u;
    int v;
    int x;
    int y;
    int xr;
    Poly28 *p;

    for (row = 0; row < map->rows; row++) {
        for (col = 0; col < map->cols; col++) {
            cell = map->cells[col + row * map->cols];
            flag = cell >> 14;
            tile = cell & 0x3fff;
            if (tile == 0) continue;
            p = &data_80186004[idx].polys[data_80188d04];
            p->field_16 = tile / 256 + 13;
            data_80188d28 = 0;
            if (kind == 2 || kind == 3) func_80134e94(kind, col);
            if (kind < 4) {
                p->field_0e = (((u8)f + map->field_03 + data_80188d28) << 6) + 0x7807;
            } else if (kind == 6) {
                p->field_0e = 0x7f07;
            } else {
                p->field_0e = (((u8)f + map->field_03) << 6) + 0x7806;
            }
            x = (u16)c + col * 16 - map->field_04;
            y = d + row * 16 - map->field_06;
            if ((flag == 0 && kind != 5) || tile == 0x3b || tile == 0x3e) {
                p->field_08 = x;
                p->field_0a = y;
                p->field_10 = x + 16;
                p->field_12 = y;
                p->field_18 = x;
                p->field_1a = y + 16;
                p->field_20 = x + 16;
                p->field_22 = y + 16;
            } else if (kind != 5) {
                p->field_08 = x + 16;
                p->field_0a = y;
                p->field_10 = x;
                p->field_12 = y;
                p->field_18 = x + 16;
                p->field_1a = y + 16;
                p->field_20 = x;
                p->field_22 = y + 16;
            } else {
                xr = (u16)c - col * 16 + map->field_04;
                p->field_08 = xr + 15;
                p->field_0a = y;
                p->field_10 = xr - 1;
                p->field_12 = y;
                p->field_18 = xr + 15;
                p->field_1a = y + 16;
                p->field_20 = xr - 1;
                p->field_22 = y + 16;
            }
            if (kind == 4 || kind == 5) {
                dx = 1;
                dy = 0;
            }
            if (tile == 0x3b || tile == 0x3e) {
                dx = 0;
                dy = 1;
            }
            u = (tile % 16) << 4;
            v = (tile / 16 % 16) << 4;
            p->field_0c = u;
            p->field_0d = v;
            p->field_14 = u + 16 - dx;
            p->field_15 = v;
            p->field_1c = u;
            p->field_1d = v + 16 - dy;
            p->field_24 = u + 16 - dx;
            p->field_25 = v + 16 - dy;
            func_8015bf34((int)data_801987c8 + (u16)e * 4, (Cmd *)p);
            data_80188d04++;
        }
    }
    if (kind == 2 || kind == 3) func_801350c0(kind, col);
}
