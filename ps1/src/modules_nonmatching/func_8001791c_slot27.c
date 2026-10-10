/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice and
 * instruction order. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file. The
 * differential test next to it (difftest.py) compares the behavior of this
 * C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): draws a scrolling tile map
 * as sprite records. A scroll record (data_8002f0c8_slot27) holds a vertical
 * and a horizontal scroll position, a pointer to the map and its width. The
 * function first pulls the scroll value at offset 0x12 up by 8 when the
 * value at 0x2a is smaller, and does nothing more when that value plus 0x40
 * is outside 0 to 0x1c0. Otherwise it walks 8 map rows and 0x30 columns of
 * 4-byte cells (a tile word and a texture page word). Each cell different
 * from the "empty" value at 0x1e gets a 0x1c-byte record in the buffer
 * chosen by data_801a27d0: position (8 pixels per cell, less the scroll
 * remainders), texture coordinates and a command word from the tile word,
 * the texture page from the second word; the record is linked into the list
 * head selected by the byte at 0x8a of the scroll record.
 *
 * Contract:
 *   No argument, no return value.
 *   Reads (scroll record S = data_8002f0c8_slot27): halfwords at 0x12 and
 *     0x2a, word 0x14, halfword 0x1e, pointer 0x50, words 0x58 and 0x5c,
 *     byte 0x8a; the buffer selector data_801a27d0 (0 or 1); the list base
 *     pointer data_801987c8 and the head words it points at; the map cells.
 *   Map: rows of 64 cells of 4 bytes; 32 rows form a strip of
 *     (S.0x58 >> 9) * 0x2000 bytes. A column index outside 0 to 0x3f is
 *     skipped, a row index outside 0 to (S.0x5c >> 3) is skipped.
 *   Records: data_8002b388_slot27, 2 buffers of 280 records of 28 bytes.
 *   Writes: S halfword 0x12 (when lowered), for each drawn cell one record
 *     (offsets 0, 4, 0x14 to 0x1b) and the head word of its list.
 *   Aliasing: the map, the list array and the record table are distinct from
 *     each other; the table's second buffer is followed in memory by S.
 *   Excluded inputs: a call that would draw more than 280 records with the
 *     selector at 1 (the records would overwrite S, which the function goes
 *     on to read); the setup keeps such grids sparse. A map pointer or scroll
 *     value that puts a cell outside RAM (the original faults).
 *   Not reached by any input: one instruction slot of the original, at
 *     offset 0x1c0, the correction (add 0x3f) of a division by 64 of a
 *     negative column index; the index is known to be 0 to 0x3f there, so
 *     the quotient is 0 and this C leaves the division out. The test that
 *     precedes it in the original is a branch that is always taken.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

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

extern Slot27Recf0c8 data_8002f0c8_slot27;
extern Slot27Recb388 data_8002b388_slot27[2][0x118];

void func_8001791c_slot27(void) {
    Slot27Scroll *scroll = (Slot27Scroll *)&data_8002f0c8_slot27;
    Slot27Prim *prim;
    u32 *ot;
    u16 *row;
    u16 *cell;
    s16 sx;
    s16 vy;
    s16 line;
    s16 lines;
    s16 col;
    s16 col0;
    s16 empty;
    s16 tile;
    int xoff;
    int yoff;
    int r;
    int i;

    if (scroll->field_2a < scroll->field_12) {
        scroll->field_12 = scroll->field_12 - 8;
    }
    sx = (u16)scroll->field_12 + 0x40;
    if ((u16)sx >= 0x1c1) return;

    xoff = sx & 7;
    vy = (scroll->field_14 - 0x100000) >> 16;
    yoff = vy & 7;
    col0 = (u32)(sx & (scroll->field_58 - 1)) >> 3;
    lines = scroll->field_5c >> 3;
    empty = scroll->field_1e;
    line = (vy >> 3) + 2;
    prim = (Slot27Prim *)data_8002b388_slot27[*(u8 *)&data_801a27d0];
    ot = (u32 *)data_801987c8 + scroll->field_8a;

    for (r = 2; r < 10; r++, line++) {
        if (line < 0 || line >= lines) continue;
        row = scroll->field_50 + ((u16)line >> 5) * ((scroll->field_58 >> 9) << 12) + ((line & 0x1f) << 7);
        col = col0;
        for (i = 0; i < 0x30; i++, col++) {
            if ((u16)col >= 0x40) continue;
            cell = row + (col & 0x3f) * 2;
            if (*cell == empty) continue;
            tile = *cell;
            prim->x = i * 8 - xoff;
            prim->y = r * 8 - yoff - 8;
            prim->u = (tile << 3) & -8;
            prim->v = (tile >> 2) & -8;
            prim->tpage = cell[1] + 0x37dd;
            prim->cmd = 0xe1000000 + (tile >> 10);
            ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
            ((PrimTag *)ot)->addr = (u32)prim;
            prim++;
        }
    }
}
