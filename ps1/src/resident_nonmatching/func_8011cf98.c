/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build of this C is 1,520 bytes (the original has 1,568); the bytes differ.
 * The exact owner of the bytes in the PS1 build stays the raw bytes of the
 * resident executable; the build does not use this file. The differential
 * test next to it (difftest.py, with func_8011cf98.py as the contract) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): the same unpacking as
 * func_8011cf98, for a second family of tables. It unpacks a compressed list
 * of 16-bit texture cells (q) into the cell tables, in blocks of 16 taken
 * from the same free list (func_8011a604 appends a block to the chain of the
 * new entry; the blocks the object's channel p->field_02 held last are given
 * back first with func_8011a6b8). q holds a count n at 0, a mode at 1 (0, 2,
 * 4 or 6; another value stores no cells), a byte source at 2 (low 5 bits)
 * and the data from index 5:
 *   mode 0: n cells, each one word; mode 2: n pairs (word, flags);
 *   mode 4: n pairs (word, extra run); mode 6: n triples (word, flags, run).
 * The word is xored with a mask built from the object's field_0b (bit 0 to
 * 0x8000, bit 1 to 0x4000), and in modes 2 and 6 also with flag bits 0x20
 * and 0x40 of the flags word moved to 0x8000 and 0x4000; the low 5 bits of
 * the flags word (modes 2, 6) or of q[2] (modes 0, 4) go to the byte table.
 * In the run modes a word is written run + 1 times, counting up by one each
 * time. The channel keeps a history of 10 records in four tables; each call
 * moves entries 0 to 8 up by one and puts the new record at 0: field_0b, the
 * new entry, the number of blocks used and q. The new entry (a free entry
 * taken with func_8011a5f4) also gets its group-table words, and p->field_94
 * points at it. When the free blocks plus the channel's last record's blocks
 * are fewer than (n + 15) / 16 + 1, the work is left to func_8011d5b8.
 *
 * Contract (the roles named for fields are inferred):
 *   Arguments: a0 = p (the object: field_02, field_0b, field_94), a1 = q
 *     (halfwords). No return value.
 *   Reads: p->field_02 and p->field_0b; q; entry 8 of the history tables
 *     data_80183d9c and data_80183dc4 of the channel (first block id and
 *     block count); the free list state in data_801846fc (head),
 *     data_80184700 (free count) and the next-link table data_80183c90.
 *   Writes: the cell tables data_80184704 (halfwords, 16 per block) and
 *     data_80185504 (bytes, 16 per block); the history tables data_80183d74,
 *     data_80183d9c, data_80183dc4, data_80183dec; p->field_94;
 *     data_80183910, data_801839f0, data_80183ad0, data_80183bb0 at the new
 *     entry; the free list.
 *   Callees run as the original code, the same in both runs (game code with
 *     no hardware): func_8011a5f4, func_8011a604, func_8011a6b8,
 *     func_8011d5b8. The setup gives them a valid free list.
 *   Aliasing: p, q and the free-list nodes (ids below 64) are distinct; q is
 *     never inside the tables.
 *   Excluded inputs: p->field_02 is 0 or 1 (the history tables hold two
 *     channels of 10 words each; the u32 table is 2 channels too); the free
 *     list must hold enough blocks for every cell written (the original takes
 *     0xffff for a block when it runs dry and then writes far outside the
 *     tables); run counts are bounded; the entry index p->field_94 on entry
 *     is below 64 (func_8011d5b8, which takes the delegated work, indexes
 *     the group tables with it and checks no bound).
 *   Not reached by any input: one instruction slot of the original, at
 *     offset 0x48, which adjusts the sum for a negative value before the
 *     division by 16; the count is a 16-bit unsigned value.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u16 func_8011a5f4(void);
u16 func_8011a604(int a, u16 b);

extern u8 data_80185504[][16];

void func_8011cf98(Slab172 *p, u16 *q) {
    u16 *src;
    u16 count;
    u16 blocks;
    u16 entry;
    u16 chan;
    unsigned h;
    u16 slot;
    u16 k;
    u16 mask;
    u16 base;
    u16 word;
    u16 flags;
    u16 reps;
    u16 n;
    u16 mode;
    unsigned i;
    unsigned j;

    chan = p->field_02;
    if ((q[0] + 15) / 16 + 1 > data_80184700 + data_80183dc4[chan][9]) {
        func_8011d5b8(p, (u32)q);
        return;
    }
    count = 0;
    k = 0;
    src = q + 5;
    func_8011a6b8(data_80183d9c[chan][9], data_80183dc4[chan][9]);
    blocks = 0;
    entry = func_8011a5f4();
    n = q[0];
    mode = q[1];
    base = ((p->field_0b & 1) << 15) | ((p->field_0b & 2) << 13);
    slot = 0;
    if (mode == 0) {
        flags = q[2] & 0x1f;
        for (i = 0; i < n; i++) {
            word = *src++;
            if (k == 0) {
                slot = func_8011a604(entry, blocks);
                blocks++;
            }
            data_80184704[slot][k] = word ^ base;
            data_80185504[slot][k] = flags;
            k = (k + 1) & 0xf;
            count++;
        }
    } else if (mode == 2) {
        for (i = 0; i < n; i++) {
            word = *src++;
            flags = *src++;
            mask = base ^ (((flags & 0x20) << 10) | ((flags & 0x40) << 8));
            flags &= 0x1f;
            if (k == 0) {
                slot = func_8011a604(entry, blocks);
                blocks++;
            }
            data_80184704[slot][k] = word ^ mask;
            data_80185504[slot][k] = flags;
            k = (k + 1) & 0xf;
            count++;
        }
    } else if (mode == 4) {
        flags = q[2] & 0x1f;
        for (i = 0; i < n; i += reps) {
            word = *src++;
            reps = *src++ + 1;
            for (j = 0; j < reps; j++) {
                if (k == 0) {
                    slot = func_8011a604(entry, blocks);
                    blocks++;
                }
                data_80184704[slot][k] = word++ ^ base;
                data_80185504[slot][k] = flags;
                k = (k + 1) & 0xf;
                count++;
            }
        }
    } else if (mode == 6) {
        for (i = 0; i < n; i += reps) {
            word = *src++;
            flags = *src++;
            mask = base ^ (((flags & 0x20) << 10) | ((flags & 0x40) << 8));
            flags &= 0x1f;
            reps = *src++ + 1;
            for (j = 0; j < reps; j++) {
                if (k == 0) {
                    slot = func_8011a604(entry, blocks);
                    blocks++;
                }
                data_80184704[slot][k] = word++ ^ mask;
                data_80185504[slot][k] = flags;
                k = (k + 1) & 0xf;
                count++;
            }
        }
    }
    for (h = 10; h > 1; h--) {
        data_80183d74[chan][h - 1] = data_80183d74[chan][h - 2];
        data_80183d9c[chan][h - 1] = data_80183d9c[chan][h - 2];
        data_80183dc4[chan][h - 1] = data_80183dc4[chan][h - 2];
        data_80183dec[chan][h - 1] = data_80183dec[chan][h - 2];
    }
    data_80183d74[chan][0] = p->field_0b;
    data_80183d9c[chan][0] = entry;
    data_80183dc4[chan][0] = blocks;
    data_80183dec[chan][0] = (u32)q;
    p->field_94 = entry;
    data_80183910[entry] = entry;
    data_801839f0[entry] = 0;
    data_80183ad0[entry] = 0;
    data_80183bb0[entry] = count;
}
