/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern u8 data_80185504[][16];
u16 get_free_list_head(void);
u16 func_8011a604(int a, u16 b);

/*
 * What it does (inferred, not an original name): unpacks a compressed list
 * of 16-bit texture cells (q) into the cell tables, for the entry p->stream_index
 * of the sprite entry tables. The cells go into blocks of 16 taken from a
 * free list (func_8011a604 appends a block to the entry's chain; the old
 * blocks of the entry are given back first with func_8011a6b8). q holds a
 * count n at 0, a mode at 1 (0, 2, 4 or 6; another value stores no cells),
 * a byte source at 2 (low 5 bits) and the data from index 5:
 *   mode 0: n cells, each one word; mode 2: n pairs (word, flags);
 *   mode 4: pairs (word, run); mode 6: triples (word, flags, run); in the
 *   run modes groups are read until their run + 1 add up to n or more.
 * The word is xored with a mask built from the object's stream_key (bit 0 to
 * 0x8000, bit 1 to 0x4000), and in modes 2 and 6 also with flag bits 0x20
 * and 0x40 of the flags word moved to 0x8000 and 0x4000; the low 5 bits of
 * the flags word (mode 2, 6) or of q[2] (mode 0, 4) go to the byte table.
 * In the run modes a word is written run + 1 times, counting up by one each
 * time, and the entries consumed count run + 1. The new entry (a free
 * entry taken with get_free_list_head) records three fields of the old entry
 * (column 0 of data_80183ffc, data_801841bc and data_8018437c, into
 * column 1), q, the number of blocks used and the number of cells
 * written; p->stream_index then points at it. When the free blocks plus the
 * entry's own are fewer than (n + 15) / 16 + 1, the work is left to
 * func_8011c568.
 */
void func_8011bf70(Slab172 *p, u16 *q) {
    u16 count;
    u16 mask;
    u16 base;
    u16 n;
    u16 entry;
    u16 slot;
    u16 k;
    u16 reps;
    u16 flags;
    u16 word;
    u16 blocks;
    u16 *src;
    unsigned int i;
    unsigned int j;

    if ((q[0] + 15) / 16 + 1 > data_80184700 + data_801841bc[p->stream_index][1]) {
        func_8011c568(p, q);
        return;
    }
    k = 0;
    count = 0;
    src = q + 5;
    func_8011a6b8(data_80183ffc[p->stream_index][1], data_801841bc[p->stream_index][1]);
    blocks = 0;
    entry = get_free_list_head();
    n = q[0];
    base = ((p->stream_key & 1) << 15) | ((p->stream_key & 2) << 13);
    /* slot gets a first value here: the compiler cannot see that the first cell always sets it (k is 0 there). Without this store the build has 2 instructions more and differs in 48 instruction lines. */
    slot = 0;
    if (q[1] == 0) {
        mask = base;
        flags = q[2] & 0x1f;
        for (i = 0; i < n; i++) {
            word = *src++;
            if (k == 0) {
                slot = func_8011a604(entry, blocks);
                blocks++;
            }
            data_80184704[slot][k] = word ^ mask;
            data_80185504[slot][k] = flags;
            k = (k + 1) & 0xf;
            count++;
        }
    }
    if (q[1] == 2) {
        for (i = 0; i < n; i++) {
            word = *src++;
            flags = *src++;
            /* The (u8) changes no value: bit 0x20 lies in the low byte. Without it the build differs in 69 instruction lines (flags declared u8: 6; the cast on the read of flags: 70). */
            mask = base ^ ((((u8) flags & 0x20) << 10) | ((flags & 0x40) << 8));
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
    }
    if (q[1] == 4) {
        mask = base;
        flags = q[2] & 0x1f;
        for (i = 0; i < n; i++) {
            word = *src++;
            reps = *src++ + 1;
            /* The outer counter counts the entries read: it is stepped back by one here and forward once per repeat below. */
            i--;
            for (j = 0; j < reps; j++) {
                i++;
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
    if (q[1] == 6) {
        for (i = 0; i < n; i++) {
            word = *src++;
            flags = *src++;
            mask = base ^ (((flags & 0x20) << 10) | ((flags & 0x40) << 8));
            flags &= 0x1f;
            reps = *src++ + 1;
            /* The outer counter counts the entries read: it is stepped back by one here and forward once per repeat below. */
            i--;
            for (j = 0; j < reps; j++) {
                i++;
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
    data_80183ffc[entry][1] = data_80183ffc[p->stream_index][0];
    data_801841bc[entry][1] = data_801841bc[p->stream_index][0];
    data_8018437c[entry][1] = data_8018437c[p->stream_index][0];
    data_8018437c[entry][0] = (u32) q;
    data_80183ffc[entry][0] = entry;
    data_801841bc[entry][0] = blocks;
    p->stream_index = entry;
    data_80183910[entry] = entry;
    data_801839f0[entry] = 0;
    data_80183ad0[entry] = 0;
    data_80183bb0[entry] = count;
}
