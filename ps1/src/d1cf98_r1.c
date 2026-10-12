/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern u8 data_80185504[][16];
u16 get_free_list_head(void);
u16 alloc_slot_after(int a, u16 b);

/*
 * What it does (inferred, not an original name): unpacks a compressed list
 * of 16-bit texture cells (q) into the cell tables, in blocks of 16 taken
 * from the free list, with a history of 10 records per channel
 * (p->field_02); the new entry is p->stream_index. When the free blocks plus the
 * channel's last record's blocks are fewer than (n + 15) / 16 + 1, the work
 * is left to push_front_of_cell_history.
 */
void unpack_cell_list_with_history(Slab172 *p, u16 *q) {
    u16 slot;
    u16 written;
    u16 xorv;
    u16 base;
    u16 total;
    u16 head;
    u16 col;
    u16 val;
    u16 run6;
    u16 run;
    unsigned int i;
    unsigned int j;
    u16 pos;
    u16 *src;
    u8 *cp;
    u16 color;
    u16 blocks;
    u16 idx;

    idx = p->field_02;
    if ((q[0] + 15) / 16 + 1 > data_80184700 + data_80183dc4[idx][9]) {
        push_front_of_cell_history(p, (u32)q);
        return;
    }
    pos = 0;
    written = 0;
    free_slot_chain(data_80183d9c[idx][9], data_80183dc4[idx][9]);
    src = q + 5;
    blocks = 0;
    head = get_free_list_head();
    base = ((p->stream_key & 1) << 15) | ((p->stream_key & 2) << 13);
    total = q[0];
    slot = 0;
    if (q[1] == 0) {
        xorv = base;
        color = q[2] & 0x1f;
        for (i = 0; i < total; i++) {
            val = *src++;
            col = pos;
            if (col == 0) {
                slot = alloc_slot_after(head, blocks);
                blocks++;
            }
            pos = (pos + 1) & 0xf;
            data_80184704[slot][col] = val ^ xorv;
            data_80185504[slot][col] = color;
            written++;
        }
    }
    if (q[1] == 2) {
        for (i = 0; i < total; i++) {
            val = *src++;
            color = *src++;
            col = pos;
            xorv = base ^ (((color & 0x20) << 10) | ((color & 0x40) << 8));
            color &= 0x1f;
            if (col == 0) {
                slot = alloc_slot_after(head, blocks);
                blocks++;
            }
            pos = (pos + 1) & 0xf;
            cp = data_80185504[slot];
            data_80184704[slot][col] = val ^ xorv;
            cp[col] = color;
            written++;
        }
    }
    if (q[1] == 4) {
        xorv = base;
        color = q[2] & 0x1f;
        for (i = 0; i < total; i++) {
            val = *src++;
            run = *src++ + 1;
            i--;
            for (j = 0; j < run; j++) {
                col = pos;
                i++;
                if (col == 0) {
                    slot = alloc_slot_after(head, blocks);
                    blocks++;
                }
                data_80184704[slot][col] = val++ ^ xorv;
                pos = (pos + 1) & 0xf;
                data_80185504[slot][col] = color;
                written++;
            }
        }
    }
    if (q[1] == 6) {
        for (i = 0; i < total; i++) {
            val = *src++;
            color = *src++;
            run6 = *src++ + 1;
            i--;
            xorv = base ^ (((color & 0x20) << 10) | ((color & 0x40) << 8));
            color &= 0x1f;
            /* Stand-in, not recovered source: a second local that copies run6 reproduces the original's choice of registers. Without it this function differs from the original in 27 instruction slots. */
            run = run6;
            for (j = 0; j < run; j++) {
                col = pos;
                i++;
                if (col == 0) {
                    slot = alloc_slot_after(head, blocks);
                    blocks++;
                }
                data_80184704[slot][col] = val++ ^ xorv;
                pos = (pos + 1) & 0xf;
                data_80185504[slot][col] = color;
                written++;
            }
        }
    }
    for (i = 10; i > 1; i--) {
        data_80183d74[idx][i - 1] = data_80183d74[idx][i - 2];
        data_80183d9c[idx][i - 1] = data_80183d9c[idx][i - 2];
        data_80183dc4[idx][i - 1] = data_80183dc4[idx][i - 2];
        data_80183dec[idx][i - 1] = data_80183dec[idx][i - 2];
    }
    data_80183d74[idx][0] = p->stream_key;
    data_80183d9c[idx][0] = head;
    data_80183dc4[idx][0] = blocks;
    data_80183dec[idx][0] = (u32)q;
    p->stream_index = head;
    data_80183910[head] = head;
    data_801839f0[head] = 0;
    data_80183ad0[head] = 0;
    data_80183bb0[head] = written;
}
