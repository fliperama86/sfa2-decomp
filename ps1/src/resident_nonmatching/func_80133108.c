/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the resident image; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): fills in the 0x20-byte
 * records of two bars, a left one (records at offsets 0xc8 to 0x168 of the
 * row) and a right one (0x128 to 0x188), in a table whose row is selected by
 * the global data_801a27d0 (16 bytes per row). Each bar is three segments of
 * 0x30 units. A value field_c6 of the left and of the right object (tripled
 * while the object's field_d8 is set, and divided back at the end) sets the
 * length of each segment (field_c6 below 0x30 in the first, 0x30 to 0x5f
 * in the second, 0x60 to 0x8f in the third) and, when it passes a segment's
 * end, the colour bytes of the records. The state bytes data_80171c5c
 * (left) and data_80171c5d (right) step 0 -> 1 -> 2 -> 3 and 4 -> 5 -> 6
 * -> 7 as the segments are passed; data_80171c5e and data_80171c5f flag a
 * full bar and make the helpers at 0x801340e4 and 0x8013411c run. At the
 * end the function calls func_80133848, which draws from the records.
 *
 * Contract (what the code reads and writes; the roles are inferred):
 *   Arguments: a0 = base of the record table, a1 = left object,
 *     a2 = right object. No return value.
 *   Reads: data_801a27d0 (low halfword, signed: the row index), and of each
 *     object field_c6 (s16), field_d8 (u8) and field_165 (read by the first
 *     callee), plus whatever the callees read.
 *   Writes: the globals data_80171c5c to data_80171c5f and the six bytes
 *     data_80188d4c, 50, 54, 58, 5c, 60; in the row (base + 16 * index): the
 *     halfwords at 0xcc, 0xd0, 0xec, 0xf0, 0x10c, 0x110, 0x130, 0x150, 0x170,
 *     the bytes at 0xc8 to 0xca, 0xe8 to 0xea, 0x128 to 0x12a, 0x148 to
 *     0x14a; the objects' field_c6 (restored to a third of the tripled
 *     value), and what the callees write.
 *   Callees: func_801336a0 (called with base, left, right and the row
 *     index in a3, which it reads), func_801340e4, func_8013411c and
 *     func_80133848 are game code and run as the original in both runs; none
 *     reaches Sony's library.
 *   Aliasing: base, the two objects are distinct blocks.
 *   Inputs excluded: the row index is 0 to 2 so that the row stays in the
 *     block the setup allocates.
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80171c5d;
extern u8 data_80171c5f;
extern u8 data_80188d50, data_80188d54;
extern u8 data_80188d58, data_80188d5c, data_80188d60;
void func_801336a0(u8 *base, Object *left, Object *right, int index);
void func_801340e4(u8 *base, s16 index);
void func_8013411c(u8 *base, s16 index);
void func_80133848(u8 *base, Object *left, Object *right);

void func_80133108(void *table, Object *left, Object *right)
{
    u8 *base = table;
    s16 index = data_801a27d0;
    u8 *row;
    u16 c;

    data_80171c5c = 0;
    data_80171c5d = 4;
    data_80171c5e = 0;
    data_80171c5f = 0;
    data_80188d4c = 0;
    data_80188d50 = 0;
    data_80188d54 = 0;
    data_80188d58 = 0;
    data_80188d5c = 0;
    data_80188d60 = 0;
    row = base + (index << 4);
    *(u16 *)(row + 0xd0) = 0;
    *(u16 *)(row + 0xf0) = 0;
    *(u16 *)(row + 0x110) = 0;
    *(u16 *)(row + 0x130) = 0;
    *(u16 *)(row + 0x150) = 0;
    *(u16 *)(row + 0x170) = 0;
    func_801336a0(base, left, right, index);

    /* left bar */
    if (left->field_d8 != 0) {
        left->field_c6 = (s16)left->field_c6 * 3;
    }
    if ((s16)left->field_c6 < 0x30) {
        *(s16 *)(row + 0xcc) = 0xa6 - (s16)left->field_c6;
        *(u16 *)(row + 0xd0) = left->field_c6;
    } else {
        if (left->field_d8 == 0) {
            row[0xc8] = 0x59;
            row[0xc9] = 0xd9;
            row[0xca] = 0xff;
            data_80171c5c = 1;
        }
        *(s16 *)(row + 0xcc) = 0x76;
        *(u16 *)(row + 0xd0) = 0x30;
    }
    c = left->field_c6;
    if ((u32)(c - 0x30) < 0x30) {
        *(s16 *)(row + 0xec) = 0xa6 - c;
        *(s16 *)(row + 0xf0) = left->field_c6 - 0x30;
    } else if (data_80171c5c == 1) {
        row[0xc8] = 0x20;
        row[0xc9] = 0xf0;
        row[0xca] = 0x20;
        row[0xe8] = 0x20;
        row[0xe9] = 0xf0;
        row[0xea] = 0x20;
        *(s16 *)(row + 0xec) = 0x46;
        *(s16 *)(row + 0xf0) = 0x30;
        data_80171c5c = 2;
    } else if (left->field_d8 != 0 && (s16)c >= 0x60) {
        *(s16 *)(row + 0xec) = 0x46;
        *(s16 *)(row + 0xf0) = 0x30;
    }
    c = left->field_c6;
    if ((u32)(c - 0x60) < 0x30) {
        *(s16 *)(row + 0x10c) = 0xa6 - c;
        *(s16 *)(row + 0x110) = left->field_c6 - 0x60;
    } else if (data_80171c5c == 2) {
        row[0xc8] = 0xf0;
        row[0xc9] = 0xf0;
        row[0xca] = 0x20;
        row[0xe8] = 0xf0;
        row[0xe9] = 0xf0;
        *(s16 *)(row + 0x10c) = 0x16;
        row[0xea] = 0x20;
        *(s16 *)(row + 0x110) = 0x30;
        data_80171c5c = 3;
    } else if (left->field_d8 != 0 && (s16)c >= 0x60) {
        *(s16 *)(row + 0x10c) = 0x16;
        *(s16 *)(row + 0x110) = 0x30;
        data_80171c5e = 1;
    }
    if (data_80171c5e != 0) {
        func_801340e4(base, index);
    }
    if (left->field_d8 != 0) {
        left->field_c6 = (s16)left->field_c6 / 3;
    }

    /* right bar */
    if (right->field_d8 != 0) {
        right->field_c6 = (s16)right->field_c6 * 3;
    }
    if ((s16)right->field_c6 < 0x30) {
        *(s16 *)(row + 0x130) = right->field_c6;
    } else {
        if (right->field_d8 == 0) {
            row[0x128] = 0x59;
            row[0x129] = 0xd9;
            row[0x12a] = 0xff;
            data_80171c5d = 5;
        }
        *(s16 *)(row + 0x130) = 0x30;
    }
    c = right->field_c6;
    if ((u32)(c - 0x30) < 0x30) {
        *(s16 *)(row + 0x150) = c - 0x30;
    } else if (data_80171c5d == 5) {
        row[0x128] = 0x20;
        row[0x129] = 0xf0;
        row[0x12a] = 0x20;
        row[0x148] = 0x20;
        row[0x149] = 0xf0;
        row[0x14a] = 0x20;
        *(s16 *)(row + 0x150) = 0x30;
        data_80171c5d = 6;
    } else if (right->field_d8 != 0 && (s16)c >= 0x60) {
        *(s16 *)(row + 0x150) = 0x30;
    }
    c = right->field_c6;
    if ((u32)(c - 0x60) < 0x30) {
        *(s16 *)(row + 0x170) = c - 0x60;
    } else if (data_80171c5d == 6) {
        row[0x128] = 0xf0;
        row[0x129] = 0xf0;
        row[0x12a] = 0x20;
        row[0x148] = 0xf0;
        row[0x149] = 0xf0;
        row[0x14a] = 0x20;
        *(s16 *)(row + 0x170) = 0x30;
        data_80171c5d = 7;
    } else if (right->field_d8 != 0 && (s16)c >= 0x90) {
        *(s16 *)(row + 0x170) = 0x30;
        data_80171c5f = 1;
    }
    if (data_80171c5f != 0) {
        func_8013411c(base, index);
    }
    if (right->field_d8 != 0) {
        right->field_c6 = (s16)right->field_c6 / 3;
    }
    func_80133848(base, left, right);
}
