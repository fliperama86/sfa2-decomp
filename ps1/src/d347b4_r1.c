/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

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
    int col;
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
                data_80186004[idx].polys[data_80188d04].field_16 = tile / 256 + 13;
                data_80188d28 = 0;
                /* The byte tests on kind in this function stay in these forms: kind == 2 || kind == 3 here
                   differs in 21 instruction lines, (u8)kind != 5 for the flag test below in 118, and
                   kind - 2 for the test after the loops in 2. */
                if ((unsigned)((kind + 0xfe) & 0xff) < 2) {
                    func_80134e94(kind, col);
                }
                if ((u8)kind < 4) {
                    data_80186004[idx].polys[data_80188d04].field_0e = (page + map->field_03 + data_80188d28) * 64 + 0x7807;
                } else if ((u8)kind == 6) {
                    data_80186004[idx].polys[data_80188d04].field_0e = 0x7f07;
                } else {
                    data_80186004[idx].polys[data_80188d04].field_0e = (page + map->field_03) * 64 + 0x7806;
                }
                if ((flag == 0 && (kind & 0xff) != 5) || tile == 0x3b || tile == 0x3e) {
                    /* The record is addressed as (&...)->field in these stores; the plain data_80186004[idx].polys[data_80188d04].field_08 form differs in 212 instruction lines for all 32 of them, and in 50 to 202 for any one group of eight. */
                    (&data_80186004[idx].polys[data_80188d04])->field_08 = (x0 + col * 16) - map->field_04;
                    (&data_80186004[idx].polys[data_80188d04])->field_0a = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->field_10 = (x0 + col * 16) - map->field_04 + 0x10;
                    (&data_80186004[idx].polys[data_80188d04])->field_12 = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->field_18 = (x0 + col * 16) - map->field_04;
                    (&data_80186004[idx].polys[data_80188d04])->field_1a = (y_base + row * 16) - map->field_06 + 0x10;
                    (&data_80186004[idx].polys[data_80188d04])->field_20 = (x0 + col * 16) - map->field_04 + 0x10;
                    (&data_80186004[idx].polys[data_80188d04])->field_22 = (y_base + row * 16) - map->field_06 + 0x10;
                } else if ((u8)kind != 5) {
                    (&data_80186004[idx].polys[data_80188d04])->field_08 = (x0 + col * 16) - (s16)(map->field_04 - 0x10);
                    (&data_80186004[idx].polys[data_80188d04])->field_0a = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->field_10 = (x0 + col * 16) - map->field_04;
                    (&data_80186004[idx].polys[data_80188d04])->field_12 = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->field_18 = (x0 + col * 16) - (s16)(map->field_04 - 0x10);
                    (&data_80186004[idx].polys[data_80188d04])->field_1a = (y_base + row * 16) - (s16)(map->field_06 - 0x10);
                    (&data_80186004[idx].polys[data_80188d04])->field_20 = (x0 + col * 16) - map->field_04;
                    (&data_80186004[idx].polys[data_80188d04])->field_22 = (y_base + row * 16) - (s16)(map->field_06 - 0x10);
                } else {
                    (&data_80186004[idx].polys[data_80188d04])->field_08 = map->field_04 + (x0 - col * 16) + 0xf;
                    (&data_80186004[idx].polys[data_80188d04])->field_0a = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->field_10 = (x0 - col * 16) + map->field_04 - 1;
                    (&data_80186004[idx].polys[data_80188d04])->field_12 = (y_base + row * 16) - map->field_06;
                    (&data_80186004[idx].polys[data_80188d04])->field_18 = map->field_04 + (x0 - col * 16) + 0xf;
                    (&data_80186004[idx].polys[data_80188d04])->field_1a = (y_base + row * 16) - (s16)(map->field_06 - 0x10);
                    (&data_80186004[idx].polys[data_80188d04])->field_20 = (x0 - col * 16) + map->field_04 - 1;
                    (&data_80186004[idx].polys[data_80188d04])->field_22 = (y_base + row * 16) - (s16)(map->field_06 - 0x10);
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
                (&data_80186004[idx].polys[data_80188d04])->field_0c = u1;
                (&data_80186004[idx].polys[data_80188d04])->field_0d = v1;
                (&data_80186004[idx].polys[data_80188d04])->field_14 = u1 + 0x10 - dx;
                (&data_80186004[idx].polys[data_80188d04])->field_15 = v1;
                (&data_80186004[idx].polys[data_80188d04])->field_1c = u1;
                (&data_80186004[idx].polys[data_80188d04])->field_1d = v1 + 0x10 - dy;
                (&data_80186004[idx].polys[data_80188d04])->field_24 = u1 + 0x10 - dx;
                (&data_80186004[idx].polys[data_80188d04])->field_25 = v1 + 0x10 - dy;
                func_8015bf34((int)data_801987c8 + list * 4, (Cmd *)&data_80186004[idx].polys[data_80188d04]);
                data_80188d04++;
            }
        }
    }
    /* col is not set when the map has no rows. The original then passes to func_801350c0 whatever its caller
       left in the register that holds col (a saved register that this function has not written yet), a value
       C cannot express; with col set to 0 at its declaration 58 instruction lines differ. The PC program runs
       port/overrides/func_801347b4.c in place of this function, which is this C with col set to 0. */
    if ((unsigned)((kind - 2) & 0xff) < 2) {
        func_801350c0(kind, col);
    }
}
