/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the resident image; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): sets up the screen of a
 * two-player display. It marks the first two bytes of the block
 * data_801ac6a8, looks up three words of table_80172144 for the block from
 * the two selectors data_801a8067 and data_801a83fb (selectors 0x12, 0x13
 * and 0x14 take other table entries), picks colour bytes by the flags
 * data_80198098 and data_8019842c, then
 *   1. fills two rows (16 bytes apart) of twelve 0x20-byte records at the
 *      start of the block: each record is initialised by func_8015c150, then
 *      gets a colour triple, a position, a value and a size;
 *   2. fills two sets of 0x90 records of 0x28 bytes at data_80186004: each is
 *      initialised by func_8015c09c, gets three bytes of 0x80 and a halfword
 *      from func_8015bd0c;
 *   3. writes the constants of table_80188d08, data_80171c5c and
 *      data_80171c5d, and four words at 0x1c4 to 0x1d0 of the block;
 *   4. initialises four strips of 0x1c bytes in data_80188d6c with
 *      func_80136d1c (two per player, 0x38 bytes apart);
 * then writes data_80188ebc and data_80188ec0 and calls func_80153088,
 * func_80132b30 and func_80132cf0.
 *
 * Contract (what the code reads and writes; the roles are inferred):
 *   Arguments: none. No return value.
 *   Reads: data_801a8067, data_801a83fb (bytes), data_80198098,
 *     data_8019842c (bytes), table_80172144 words 0, 4, 8, 10, 13 to 33,
 *     player_left.field_5c and player_right.field_5c (halfwords), and what
 *     func_80153088, func_80132b30 and func_80132cf0 read.
 *   Writes: the block data_801ac6a8 (head bytes and words 0x1c4 to 0x1dc),
 *     data_801a6938, data_80188d64, 65, 68, 69, the table_80188d08 bytes and
 *     halfwords named in the C, data_80171c5c and data_80171c5d, the two
 *     record areas at data_80186004, the strips of data_80188d6c,
 *     data_80188ebc and data_80188ec0, and what the three last callees write:
 *     strips2 and data_80188e4c (func_80132cf0), the strips of strips and
 *     data_80190014 (func_80132b30; it reads game_state.field_42), and the
 *     two records of data_8018d210 (func_80153088).
 *   Callees replaced by recorders, the same in both runs: func_8015c150 (1
 *     argument), func_8015c09c (1), func_8015bd0c (4; the result is random
 *     per case), all in Sony's library; and func_80136d1c (1), which reaches
 *     the library through func_80158a2c and func_8015c23c. func_80153088,
 *     func_80132b30 and func_80132cf0 are game code and run as the
 *     original in both runs (the last two call func_80136d1c, a recorder).
 *   Aliasing: the data named above are distinct from each other.
 *   Inputs excluded: none.
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* data_801ac6a8 is declared by the shared header as an array of bytes; the
   function reads it as this block. */
extern PolyBlk data_80186004[];

void func_8015c150(u8 *record);
void func_80153088(void);
void func_80132b30(void);
void func_80132cf0(void);

/* One record of the first area: initialised by func_8015c150, then three
   colour bytes at 4 and halfwords at 8 to 0xe. */
static void init_record(u8 *rec, int c0, int c1, int c2, int x, int y, int z, int w)
{
    func_8015c150(rec);
    rec[4] = c0;
    rec[5] = c1;
    rec[6] = c2;
    *(u16 *)(rec + 8) = x;
    *(u16 *)(rec + 0xa) = y;
    *(u16 *)(rec + 0xc) = z;
    *(u16 *)(rec + 0xe) = w;
}

void func_8013245c(void)
{
    HudBlk *blk = (HudBlk *)data_801ac6a8;
    u8 *base = blk->head;
    int sel_left, sel_right;
    int i, j;
    int x;
    u8 *row;
    u8 *rec;
    Strip1c *strip;

    base[0] = 1;
    base[1] = 1;
    sel_left = data_801a8067;
    sel_right = data_801a83fb;
    blk->field_1d4 = table_80172144[sel_left + 13];
    blk->field_1d8 = table_80172144[sel_right + 13];
    blk->field_1dc = table_80172144[32];
    data_801a6938 = 0;
    if (sel_left == 0x12) {
        blk->field_1d4 = table_80172144[31];
    }
    if (sel_left == 0x13) {
        blk->field_1d4 = table_80172144[30];
    }
    if (sel_left == 0x14) {
        blk->field_1d4 = table_80172144[15];
    }
    if (sel_right == 0x12) {
        blk->field_1d8 = table_80172144[31];
    }
    if (sel_right == 0x13) {
        blk->field_1d8 = table_80172144[30];
    }
    if (sel_right == 0x14) {
        blk->field_1d8 = table_80172144[15];
    }
    if (data_80198098) {
        data_80188d64 = 0;
        data_80188d68 = 0xf0;
    } else {
        data_80188d64 = 0xf0;
        data_80188d68 = 0xf0;
    }
    if (data_8019842c) {
        data_80188d65 = 0;
        data_80188d69 = 0xf0;
    } else {
        data_80188d65 = 0xf0;
        data_80188d69 = 0xf0;
    }

    for (i = 0; i < 2; i++) {
        row = base + (i << 4);
        init_record(row + 4, data_80188d64, data_80188d68, 0, 0x18, 0x12, player_left.field_5c, 5);
        init_record(row + 0x44, 0xf0, 0x30, 0, 8, 0x12, 0, 5);
        init_record(row + 0x24, data_80188d65, data_80188d69, 0, 0xd8, 0x12, player_right.field_5c, 5);
        init_record(row + 0x64, 0xf0, 0x30, 0, 0x168, 0x12, 0, 5);
        init_record(row + 0xc4, 0x49, 0xc9, 0xf3, 0xa6, 0xd5, 0, 3);
        init_record(row + 0xe4, 0x20, 0xf0, 0x20, 0x76, 0xd5, 0, 4);
        init_record(row + 0x104, 0xf0, 0xf0, 0x20, 0x46, 0xd5, 0, 4);
        init_record(row + 0x124, 0x49, 0xc9, 0xf3, 0xc6, 0xd5, 0, 3);
        init_record(row + 0x144, 0x20, 0xf0, 0x20, 0xf6, 0xd5, 0, 4);
        init_record(row + 0x164, 0xf0, 0xf0, 0x20, 0x126, 0xd5, 0, 4);
        init_record(row + 0x184, 0x49, 0xc9, 0xf3, 0xc0, 0xc2, 0, 4);
        init_record(row + 0x1a4, 0x49, 0xc9, 0xf3, 0xcb, 0xc2, 0, 4);
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 0x90; j++) {
            rec = (u8 *)data_80186004 + i * 0x1680 + j * 0x28;
            func_8015c09c(rec);
            rec[4] = 0x80;
            rec[5] = 0x80;
            rec[6] = 0x80;
            *(u16 *)(rec + 0x16) = func_8015bd0c(0, 0, 0x340, 0);
        }
    }

    table_80188d08[2] = 9;
    table_80188d08[6] = 9;
    table_80188d08[1] = 5;
    table_80188d08[5] = 5;
    data_80171c5d = 4;
    table_80188d08[8] = 4;
    table_80188d08[12] = 4;
    table_80188d08[16] = 4;
    table_80188d08[20] = 4;
    table_80188d08[24] = 4;
    table_80188d08[28] = 4;
    data_80171c5c = 0;
    table_80188d08[0] = 0;
    table_80188d08[4] = 0;
    *(u16 *)(table_80188d08 + 0x24) = 1;
    *(u16 *)(table_80188d08 + 0x28) = 1;
    *(u16 *)(table_80188d08 + 0x2c) = 1;
    *(u16 *)(table_80188d08 + 0x30) = 1;
    *(u16 *)(table_80188d08 + 0x34) = 1;
    *(u16 *)(table_80188d08 + 0x38) = 1;
    blk->field_1c4 = table_80172144[10];
    blk->field_1cc = table_80172144[8];
    blk->field_1c8 = table_80172144[0];
    blk->field_1d0 = table_80172144[4];

    for (i = 0; i < 2; i++) {
        x = 0xa8;
        for (j = 0; j < 2; j++) {
            strip = (Strip1c *)((u8 *)data_80188d6c + i * 0x38 + j * 0x1c);
            func_80136d1c((Tx *)strip);
            strip->field_14 = x;
            x += 0x10;
            strip->field_16 = 2;
            strip->field_19 = 0xf0;
            strip->field_10 = 0x80;
            strip->field_11 = 0x80;
            strip->field_12 = 0x80;
            strip->field_1a = 0x7e07;
            strip->field_04 = 0xe100001f;
        }
    }
    data_80188ebc = 0x7e07;
    data_80188ec0 = 0x7807;
    func_80153088();
    func_80132b30();
    func_80132cf0();
}
