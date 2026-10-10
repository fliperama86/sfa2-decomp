/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * original allocates its registers differently in every block and writes
 * back two pointers with the value just loaded from them (read from the
 * listing); the code built from this C is smaller and is not the
 * original's bytes. The exact owner of the bytes in the PS1 build stays the raw bytes of the resident image; the
 * build does not use this file. The differential test next to it
 * (difftest.py) compares the behavior of this C with the original code on
 * random inputs of the contract below.
 *
 * What it does (inferred, not an original name): resets the object pools.
 * It calls func_8011ef34, then, for three arrays of records in turn (16
 * units of 0xc0 bytes at units_2c20, 40 records of 0xac bytes at
 * data_801a89f4, 16 records of 0xac bytes at data_801ac888), clears each
 * record, writes a type byte at offset 8 (8, 0xc and 0x10) and stores the
 * record's address in the table of that array (data_801a89b0,
 * table_80197f20, data_801a68f0), using ref_third as the running
 * allocation pointer. It also writes the last index of each array (0xf,
 * 0x27, 0xf into data_801a6960, data_80197f10, data_801a4fec). It clears
 * the 256 pool entries of data_8018e598 (16 bytes each), gives entry i the
 * buffer address data_801e0000 + 0x80 * i, and fills the pointer table
 * data_801abf10 with the entries; it clears the 16 pool entries of
 * data_8018f5e4 and fills the pointer table data_801ad358 with them; it
 * sets the counters data_801ac618 (0xff), data_801abefc (0), data_801ad348
 * (0xf), the pointers data_801a4fe4 (to the table data_801ac30c) and
 * data_801a6980 (to the last entry of data_801ad358), data_8018db14 (0)
 * and data_801a27d4 and data_801a27cc (8).
 *
 * Contract:
 *   No argument, no result.
 *   Reads: nothing that matters (the pointers it follows, ref_third
 *     after its own first store, are its own).
 *   Writes: all of the above, and the type bytes and clears of the records;
 *     bytes of the regions not named are left alone.
 *   Watched at the call (copied into the log): every region the function
 *     writes (the record arrays, the pointer tables, the pools and their
 *     tables, the counters and ref_third).
 *   Callee replaced by a recorder, the same in both runs: func_8011ef34
 *     (takes no argument, returns 0; it initialises the players and
 *     reaches the library: inferred, not tested).
 *   Aliasing: none intended; the regions are the program's own distinct
 *     arrays.
 *   Not written by this C, written by the original (read from the listing):
 *     the original stores back into the first entry of data_801ac30c and
 *     into the last entry
 *     of data_801ad358 the word it has just loaded from there (a store of
 *     the same value); the value does not change and no input could show a
 *     difference.
 *   Every instruction slot of the original is executed by every case.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations; not original ones. */
extern Pooled *data_801abf10[];
extern Pooled *data_801ac30c[];
extern Pooled *data_801ad358[];
extern u8 data_801e0000[];
void func_8011ef34(void);

/* Clears the record of `size` bytes at the allocation pointer, stores the
   type byte at offset 8, moves the pointer past the record and returns the
   record's address. */
static u8 *take_record(int size, u8 type) {
    u8 *start = (u8 *)ref_third.p;
    int n;

    for (n = 0; n < size; n++) {
        start[n] = 0;
    }
    ((u8 *)ref_third.p)[8] = type;
    ref_third.p = (Object *)(start + size);
    return start;
}

void func_8011eb4c(void) {
    int i;
    int n;

    func_8011ef34();

    data_801a6960 = 0xf;
    ref_third.p = (Object *)units_2c20;
    for (i = 0; i < 16; i++) {
        data_801a89b0[i] = (Object *)take_record(0xc0, 8);
    }

    data_80197f10 = 0x27;
    ref_third.p = (Object *)data_801a89f4;
    for (i = 0; i < 40; i++) {
        table_80197f20[i] = (Block172 *)take_record(0xac, 0xc);
    }

    data_801a4fec = 0xf;
    ref_third.p = (Object *)data_801ac888;
    for (i = 0; i < 16; i++) {
        data_801a68f0[i] = (Object *)take_record(0xac, 0x10);
    }

    for (i = 0; i < 256; i++) {
        for (n = 0; n < 16; n++) {
            ((u8 *)data_8018e598)[i * 16 + n] = 0;
        }
    }
    for (i = 0; i < 256; i++) {
        data_8018e598[i].field_04 = (u32)data_801e0000 + 0x80 * i;
        data_8018e598[i].field_00 = 0;
        data_801abf10[i] = &data_8018e598[i];
    }

    data_801ac618 = 0xff;
    data_801abefc = 0;
    data_801a4fe4 = data_801ac30c;
    for (i = 0; i < 16; i++) {
        for (n = 0; n < 16; n++) {
            ((u8 *)data_8018f5e4)[i * 16 + n] = 0;
        }
    }
    for (i = 0; i < 16; i++) {
        data_8018f5e4[i].field_00 = 0;
        data_801ad358[i] = &data_8018f5e4[i];
    }

    data_801ad348 = 0xf;
    data_801a6980 = &data_801ad358[15];
    data_8018db14 = 0;
    data_801a27d4 = 8;
    data_801a27cc = 8;
}
