/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern u8 data_80186004[];

void func_8015c150(u8 *record);
void func_80153088(void);
void init_strips(void);
void init_strips2(void);

/*
 * What it does (inferred, not an original name): sets up the screen of a
 * two-player display. It marks the first two bytes of the block
 * data_801ac6a8, looks up three words of table_80172144 for the block from
 * the two selectors data_801a8067 and data_801a83fb (the word at selector
 * + 13; the selectors 0x13 and 0x14 take words 30 and 15 instead of 32 and
 * 33, and 0x12 takes word 31, which is also its default), picks colour
 * bytes by the flags data_80198098 and data_8019842c, then
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
 * init_strips and init_strips2.
 */
void func_8013245c(void) {
    HudBlk *blk = (HudBlk *)data_801ac6a8;
    u8 *base = blk->head;
    int pa, pb;
    int i;
    int kd5, k3, k20, k4;
    int c8;
    Strip1c *s;
    u8 *b2;
    *base = 1;
    blk->head[1] = 1;
    pa = data_801a8067;
    pb = data_801a83fb;
    blk->field_1d4 = table_80172144[pa + 13];
    blk->field_1d8 = table_80172144[pb + 13];
    blk->field_1dc = table_80172144[32];
    data_801a6938 = 0;
    if (pa == 0x12) {
        blk->field_1d4 = table_80172144[31];
    }
    if (pa == 0x13) {
        blk->field_1d4 = table_80172144[30];
    }
    if (pa == 0x14) {
        blk->field_1d4 = table_80172144[15];
    }
    if (pb == 0x12) {
        blk->field_1d8 = table_80172144[31];
    }
    if (pb == 0x13) {
        blk->field_1d8 = table_80172144[30];
    }
    if (pb == 0x14) {
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
        func_8015c150(&base[(i << 4) + 4]);
        base[(i << 4) + 8] = data_80188d64;
        base[(i << 4) + 9] = data_80188d68;
        base[(i << 4) + 0xa] = 0;
        *(u16 *)&base[(i << 4) + 0xc] = 0x18;
        *(u16 *)&base[(i << 4) + 0xe] = 0x12;
        *(u16 *)&base[(i << 4) + 0x10] = player_left.health;
        *(u16 *)&base[(i << 4) + 0x12] = 5;
        func_8015c150(&base[(i << 4) + 0x44]);
        base[(i << 4) + 0x48] = 0xf0;
        base[(i << 4) + 0x49] = 0x30;
        base[(i << 4) + 0x4a] = 0;
        *(u16 *)&base[(i << 4) + 0x4c] = 8;
        *(u16 *)&base[(i << 4) + 0x4e] = 0x12;
        *(u16 *)&base[(i << 4) + 0x50] = 0;
        *(u16 *)&base[(i << 4) + 0x52] = 5;
        func_8015c150(&base[(i << 4) + 0x24]);
        base[(i << 4) + 0x28] = data_80188d65;
        base[(i << 4) + 0x29] = data_80188d69;
        base[(i << 4) + 0x2a] = 0;
        *(u16 *)&base[(i << 4) + 0x2c] = 0xd8;
        *(u16 *)&base[(i << 4) + 0x2e] = 0x12;
        *(u16 *)&base[(i << 4) + 0x30] = player_right.health;
        *(u16 *)&base[(i << 4) + 0x32] = 5;
        func_8015c150(&base[(i << 4) + 0x64]);
        base[(i << 4) + 0x68] = 0xf0;
        base[(i << 4) + 0x69] = 0x30;
        base[(i << 4) + 0x6a] = 0;
        *(u16 *)&base[(i << 4) + 0x6c] = 0x168;
        *(u16 *)&base[(i << 4) + 0x6e] = 0x12;
        *(u16 *)&base[(i << 4) + 0x70] = 0;
        *(u16 *)&base[(i << 4) + 0x72] = 5;
        func_8015c150(&base[(i << 4) + 0xc4]);
        base[(i << 4) + 0xc8] = 0x49;
        base[(i << 4) + 0xc9] = 0xc9;
        base[(i << 4) + 0xca] = 0xf3;
        *(u16 *)&base[(i << 4) + 0xcc] = 0xa6;
        /* Stand-in, not recovered source: one stand-in at 8 places, the four locals kd5, k3, k20 and k4, each set to its constant before its first store and set to 0 after its last use (the assignments of 0 are never read), reproduces the original's choice of registers. Without it this function differs from the original in 110 instruction slots. */
        kd5 = 0xd5;
        *(u16 *)&base[(i << 4) + 0xce] = kd5;
        *(u16 *)&base[(i << 4) + 0xd0] = 0;
        /* The second place of the stand-in above. The figure is the one above: the four locals kd5, k3, k20 and k4 taken out, each constant written at its uses. */
        k3 = 3;
        *(u16 *)&base[(i << 4) + 0xd2] = k3;
        func_8015c150(&base[(i << 4) + 0xe4]);
        /* The second place of the stand-in above. The figure is the one above: the four locals kd5, k3, k20 and k4 taken out, each constant written at its uses. */
        k20 = 0x20;
        base[(i << 4) + 0xe8] = k20;
        base[(i << 4) + 0xe9] = 0xf0;
        base[(i << 4) + 0xea] = k20;
        *(u16 *)&base[(i << 4) + 0xec] = 0x76;
        *(u16 *)&base[(i << 4) + 0xee] = kd5;
        *(u16 *)&base[(i << 4) + 0xf0] = 0;
        /* The second place of the stand-in above. The figure is the one above: the four locals kd5, k3, k20 and k4 taken out, each constant written at its uses. */
        k4 = 4;
        *(u16 *)&base[(i << 4) + 0xf2] = k4;
        func_8015c150(&base[(i << 4) + 0x104]);
        base[(i << 4) + 0x108] = 0xf0;
        base[(i << 4) + 0x109] = 0xf0;
        base[(i << 4) + 0x10a] = k20;
        *(u16 *)&base[(i << 4) + 0x10c] = 0x46;
        *(u16 *)&base[(i << 4) + 0x10e] = kd5;
        *(u16 *)&base[(i << 4) + 0x110] = 0;
        *(u16 *)&base[(i << 4) + 0x112] = k4;
        func_8015c150(&base[(i << 4) + 0x124]);
        base[(i << 4) + 0x128] = 0x49;
        base[(i << 4) + 0x129] = 0xc9;
        base[(i << 4) + 0x12a] = 0xf3;
        *(u16 *)&base[(i << 4) + 0x12c] = 0xc6;
        *(u16 *)&base[(i << 4) + 0x12e] = kd5;
        *(u16 *)&base[(i << 4) + 0x130] = 0;
        *(u16 *)&base[(i << 4) + 0x132] = k3;
        /* The second place of the stand-in above. The figure is the one above: the four locals kd5, k3, k20 and k4 taken out, each constant written at its uses. */
        k3 = 0;
        func_8015c150(&base[(i << 4) + 0x144]);
        base[(i << 4) + 0x148] = k20;
        base[(i << 4) + 0x149] = 0xf0;
        base[(i << 4) + 0x14a] = k20;
        *(u16 *)&base[(i << 4) + 0x14c] = 0xf6;
        *(u16 *)&base[(i << 4) + 0x14e] = kd5;
        *(u16 *)&base[(i << 4) + 0x150] = 0;
        *(u16 *)&base[(i << 4) + 0x152] = k4;
        func_8015c150(&base[(i << 4) + 0x164]);
        base[(i << 4) + 0x168] = 0xf0;
        base[(i << 4) + 0x169] = 0xf0;
        base[(i << 4) + 0x16a] = k20;
        /* The second place of the stand-in above. The figure is the one above: the four locals kd5, k3, k20 and k4 taken out, each constant written at its uses. */
        k20 = 0;
        *(u16 *)&base[(i << 4) + 0x16c] = 0x126;
        *(u16 *)&base[(i << 4) + 0x16e] = kd5;
        /* The second place of the stand-in above. The figure is the one above: the four locals kd5, k3, k20 and k4 taken out, each constant written at its uses. */
        kd5 = 0;
        *(u16 *)&base[(i << 4) + 0x170] = 0;
        *(u16 *)&base[(i << 4) + 0x172] = k4;
        func_8015c150(&base[(i << 4) + 0x184]);
        base[(i << 4) + 0x188] = 0x49;
        base[(i << 4) + 0x189] = 0xc9;
        base[(i << 4) + 0x18a] = 0xf3;
        *(u16 *)&base[(i << 4) + 0x18c] = 0xc0;
        *(u16 *)&base[(i << 4) + 0x18e] = 0xc2;
        *(u16 *)&base[(i << 4) + 0x190] = 0;
        *(u16 *)&base[(i << 4) + 0x192] = k4;
        func_8015c150(&base[(i << 4) + 0x1a4]);
        base[(i << 4) + 0x1a8] = 0x49;
        base[(i << 4) + 0x1a9] = 0xc9;
        base[(i << 4) + 0x1aa] = 0xf3;
        *(u16 *)&base[(i << 4) + 0x1ac] = 0xcb;
        *(u16 *)&base[(i << 4) + 0x1ae] = 0xc2;
        *(u16 *)&base[(i << 4) + 0x1b0] = 0;
        *(u16 *)&base[(i << 4) + 0x1b2] = k4;
        /* The second place of the stand-in above. The figure is the one above: the four locals kd5, k3, k20 and k4 taken out, each constant written at its uses. */
        k4 = 0;
    }
    for (i = 0; i < 2; i++) {
        /* One j for this loop and one for the last loop: with a single j for the function this differs from the original in 16 instruction slots. */
        int j;
        /* b2 holds the loaded address of data_80186004 for the loop: with the symbol written at each use the built size is 1768 bytes and 56 instruction slots differ. */
        b2 = data_80186004;
        for (j = 0; j < 0x90; j++) {
            /* Stand-in, not recovered source: the conversion of the pointer sum to a 64-bit integer in the argument below, which changes no value, reproduces the original's choice of registers. Without it this function differs from the original in 1 instruction slots. */
            /* The argument's address is formed as an integer sum, with the product first as the original adds it; as pointer arithmetic the compiler writes the pointer first. This assumes flat addresses: a pointer converted to a wide unsigned integer and back is that pointer, moved by the bytes added. It is not portable C in general. The integer has 64 bits, so that it holds a pointer of the PC build whole. */
            func_8015c09c((u8 *)(i * 0x1680 + (unsigned long long)(b2 + j * 0x28)));
            /* Stand-in, not recovered source: one stand-in at 2 places, the local c8, set to 0x80 inside the first colour store and to 0 after the last (never read), reproduces the original's order of instructions. Without it this function differs from the original in 13 instruction slots. */
            b2[i * 0x1680 + j * 0x28 + 4] = (c8 = 0x80, c8);
            b2[i * 0x1680 + j * 0x28 + 5] = c8;
            b2[i * 0x1680 + j * 0x28 + 6] = c8;
            /* The second place of the stand-in above. The figure is the one above: the local c8 taken out, 0x80 written at its three uses. */
            c8 = 0;
            *(u16 *)&data_80186004[i * 0x1680 + j * 0x28 + 0x16] = func_8015bd0c(0, 0, 0x340, 0);
        }
    }
    /* The order of these stores is the original's: with data_80171c5d set after table_80188d08[28] it differs in 17 instruction slots, with [2] and [6] before [1] in 10. */
    data_80171c5d = 4;
    table_80188d08[1] = 5;
    table_80188d08[2] = 9;
    table_80188d08[6] = 9;
    table_80188d08[5] = 5;
    table_80188d08[8] = 4;
    table_80188d08[12] = 4;
    table_80188d08[16] = 4;
    table_80188d08[20] = 4;
    table_80188d08[24] = 4;
    table_80188d08[28] = 4;
    data_80171c5c = 0;
    table_80188d08[0] = 0;
    table_80188d08[4] = 0;
    *(u16 *)&table_80188d08[0x24] = 1;
    *(u16 *)&table_80188d08[0x28] = 1;
    *(u16 *)&table_80188d08[0x2c] = 1;
    *(u16 *)&table_80188d08[0x30] = 1;
    *(u16 *)&table_80188d08[0x34] = 1;
    *(u16 *)&table_80188d08[0x38] = 1;
    /* The four words are written through the byte pointer base; written as the fields of blk the built size is 1764 bytes and 19 instruction slots differ. */
    *(u32 *)&base[0x1c4] = table_80172144[10];
    *(u32 *)&base[0x1cc] = table_80172144[8];
    *(u32 *)&base[0x1c8] = table_80172144[0];
    *(u32 *)&base[0x1d0] = table_80172144[4];
    for (i = 0; i < 2; i++) {
        int j;
        for (j = 0; j < 2; j++) {
            s = (Strip1c *)((u8 *)data_80188d6c + i * 0x38) + j;
            func_80136d1c((Tx *)s);
            s->pos_x = 0xa8 + j * 0x10;
            s->field_16 = 2;
            s->field_19 = 0xf0;
            s->field_10 = 0x80;
            s->field_11 = 0x80;
            s->field_12 = 0x80;
            s->field_1a = 0x7e07;
            s->field_04 = 0xe100001f;
        }
    }
    data_80188ebc = 0x7e07;
    data_80188ec0 = 0x7807;
    func_80153088();
    init_strips();
    init_strips2();
}



