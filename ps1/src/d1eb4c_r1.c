/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern Pooled *data_801abf10[];
extern Pooled *data_801ac30c[];
extern Pooled *data_801ad358[];
void func_8011ef34(void);

/* A second name for ref_third (the shared header declares it as a one-member struct, ObjectRef), here as a plain pointer: the
   allocation pointer that the function moves over the records. With ref_third.p written in its place this function differs from
   the original in 23 instruction slots (inferred: cse then reads the pointer through the cursor's register). The name is inferred. */
extern Object *alloc_cursor_80190478;

/* Resets the object pools (inferred): clears and numbers the records of the
   three record arrays (16 of 0xc0 bytes, 40 and 16 of 0xac bytes), each with
   a type byte at offset 8, then clears the 256 and the 16 pool entries and
   fills their pointer tables. */
void func_8011eb4c(void) {
    unsigned int i;
    unsigned int j;
    u8 *p;
    Object **slot;
    Object **slot2;
    Object **slot3;
    Object **cursor;
    Object **cursor2;
    Object **cursor3;
    Object *cur;
    u8 blank;
    /* fill is the byte the clearing loops store; the original copies it from the zero register in loops 2 and 3. */
    u8 fill;
    /* The local holds a constant: the original keeps these values (8, 0xc, 0x10 of the three loops) in a register of its own. The name is inferred. */
    /* With the constants written at its uses this function differs from the original in 20 instruction slots. */
    u8 kind;
    Object *rt;
    Pooled **q;
    Pooled *t;
    Pooled **q2;
    Pooled *t2;

    func_8011ef34();
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
    /* With 0 written at its uses (and in fill) this function differs from the original in 5 instruction slots. */
    blank = 0;
    i = 0;
    cursor = &alloc_cursor_80190478;
    data_801a6960 = 0xf;
    *cursor = (Object *)units_2c20;
    do {
        slot = cursor;
        fill = blank;
        p = (u8 *)(*slot);
        for (j = 0; j < 0xc0; j++) {
            *p++ = fill;
        }
        kind = 8;
        rt = alloc_cursor_80190478;
        rt->field_08 = kind;
        cur = *slot;
        *slot = (Object *)((Unit *)cur + 1);
        data_801a89b0[i] = cur;
        i++;
    } while (i < 16);
    i = 0;
    cursor2 = &alloc_cursor_80190478;
    data_80197f10 = 0x27;
    *cursor2 = (Object *)data_801a89f4;
    do {
        slot2 = cursor2;
        fill = blank;
        p = (u8 *)(*slot2);
        for (j = 0; j < 0xac; j++) {
            *p++ = fill;
        }
        kind = 0xc;
        rt = alloc_cursor_80190478;
        rt->field_08 = kind;
        cur = *slot2;
        *slot2 = (Object *)((Block172 *)cur + 1);
        table_80197f20[i] = (Block172 *)cur;
        i++;
    } while (i < 40);
    i = 0;
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    do {
        cursor3 = &alloc_cursor_80190478;
        data_801a4fec = 0xf;
        *cursor3 = (Object *)data_801ac888;
        do {
            slot3 = cursor3;
            fill = blank;
            p = (u8 *)(*slot3);
            for (j = 0; j < 0xac; j++) {
                *p++ = fill;
            }
            kind = 0x10;
            rt = alloc_cursor_80190478;
            rt->field_08 = kind;
            cur = *slot3;
            *slot3 = (Object *)((Slab172 *)cur + 1);
            data_801a68f0[i] = cur;
            i++;
        } while (i < 16);
        {
            p = (u8 *)data_8018e598;
            for (i = 0; i < 256; i++) {
                u8 w = blank;
                for (j = 0; j < 16; j++)
                    *p++ = w;
            }
        }
        {
            u32 base = 0x801e0000;
            for (i = 0; i < 256; i++) {
                data_8018e598[i].field_04 = base;
                base += 0x80;
                data_8018e598[i].field_00 = 0;
                data_801abf10[i] = &data_8018e598[i];
            }
        }
        {
            /* The original loads the first entry of data_801ac30c and stores the same word back (below, *q = t); kept as read from the listing. */
            q = data_801ac30c;
            t = *q;
            p = (u8 *)data_8018f5e4;
            data_801ac618 = 0xff;
            data_801abefc = 0;
            data_801a4fe4 = q;
            *q = t;
            for (i = 0; i < 16; i++) {
                u8 w = blank;
                for (j = 0; j < 16; j++)
                    *p++ = w;
            }
        }
        for (i = 0; i < 16; i++) {
            data_8018f5e4[i].field_00 = 0;
            data_801ad358[i] = &data_8018f5e4[i];
        }
        q2 = &data_801ad358[15];
        t2 = *q2;
    } while (0);
    data_801ad348 = 0xf;
    data_801a6980 = q2;
    data_8018db14 = 0;
    data_801a27d4 = 8;
    data_801a27cc = 8;
    /* Likewise the last entry of data_801ad358: the original stores back the word it loaded. */
    *q2 = t2;
}
