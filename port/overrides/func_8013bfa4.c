/*
 * The port's C for func_8013bfa4. It runs in the PC program in place of the
 * C of the unit d3bfa4_r1.c; the PS1 build does not use this file, and the
 * unit keeps the exact C.
 *
 * What is different from the unit's C: the shift count is masked with 31 in
 * the test of the state bytes. The unit shifts by the count as it is.
 *
 * Why: the count is the table object's field_02, a byte, so it can reach
 * 255. The original shifts with a variable shift instruction, and the
 * console takes the count modulo 32 (read from the original's listing, not
 * tested here). In C a shift by 32 or more has no defined result, so
 * compiled for a PC nothing ties the unit's C to the console's result for
 * such a count. With the mask written the count is defined, and for counts
 * below 32 this file and the unit do the same. The readable C that stood
 * for this function before it was exact wrote the mask too. Whether the
 * game gives a table object a field_02 of 32 or more is not known.
 *
 * What it does (inferred): see the comment above the function.
 *
 * Contract:
 *   No argument, no return value.
 *   Reads: count_8018f59c (16 bits; the count less one is kept as a
 *     signed 16-bit bound), the table data_8018f5e0 (words at 0, -4, -8, ...
 *     from its start); the table object's active_count, field_02, field_49,
 *     health, team_id, frame (active, value_a), box_tables (boxes_b),
 *     and, through func_801397d0, its field_08, field_0b, pos_x, pos_y and
 *     wide_boxes; the player's field_163, countdown_4, other_lock, field_295,
 *     field_69, active_state, hit_type, health, boxes_a, boxes_b, boxes_c,
 *     frame (box_a, box_b, box_c); data_80190468 (a pointer to an object
 *     whose bytes at 0x5c to 0x5e it writes).
 *   Writes: ref_other, ref_third, ref_second, ref_first, data_80190458,
 *     data_801904c0, the overlap record at 80188ed0 (its in_ and f_ words,
 *     and the out_ fields through func_801397d0), data_80188f30; the
 *     player's field_241 and hit_type; the table object's active_count,
 *     state_0 to state_3, health; three bytes at data_80190468 + 0x5c.
 *   Callees that run as the original code: func_80139a1c (clears the
 *     overlap record; inferred) and func_801397d0 (computes the overlap).
 *     Callees replaced by recorders, which return 0: func_80139a78 (a, b),
 *     func_8013a3a8 (a, b, box), func_8013af1c (a, b, box), func_80155de0
 *     (box, b). What they do to the objects is outside the test. The
 *     original ignores the result of func_80155de0 (it tests a zero it has
 *     just set; read from the original's listing, not tested), and so does
 *     this C.
 *   Watched by the recorders at every call, whole: the table objects, both
 *     players, the object at data_80190468, the overlap record,
 *     data_80188f30, ref_first to data_80190414 and the word after it,
 *     data_80190458, data_801904c0, ref_other and ref_third.
 *   Aliasing: the table objects, the players, their frames, box tables and
 *     box lists, and the object at data_80190468 are distinct blocks; the
 *     table may repeat an object.
 *   Excluded: a count above the size of the table (the original reads the
 *     memory below the symbol; the table holds at most 12 entries).
 *   Not reached by any input: none expected; see the coverage line.
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

/* data_801904c0 is in no shared header. As a plain pointer (Object *) the unit differs in
   42 instruction lines. */
extern ObjectRef data_801904c0;
/* The overlap record at 0x80188ed0 is written word by word through names of its own, each a
   separate absolute address. externs.h declares the same memory as the struct Rects
   data_80188ed0 and as the array table_80188ed0, which other units use: with either of them
   for the first word alone this unit differs in 31 instruction lines. size_80188ed0 is a
   second name for that first word; the next two sums are named after it. */
extern s16 size_80188ed0, size_80188ed4, size_80188ed8;
extern u16 data_80188edc, data_80188ee0, data_80188ee4, data_80188ee8,
    data_80188eec, data_80188ef0, data_80188ef4, data_80188ef8,
    data_80188efc, data_80188f00, data_80188f04, data_80188f08,
    data_80188f0c, data_80188f10, data_80188f14, data_80188f18;

/* Three state bytes at offset 0x5c of the object that data_80190468 points at. Two u16
   fields of Object cover these bytes; writing them as u16 differs in 23 instruction lines. */
typedef struct {
    u8 pad[0x5c];
    u8 state;
    u8 mode;
    u8 mode2;
} StateBytes;

/* What it does (inferred, not an original name): for each object of the object table
   (data_8018f5e0, read downward, count_8018f59c entries) of type 1 it picks a player object
   (player_left, or player_right when the object's team_id is 0), skips it when the
   player's countdown_4, other_lock or field_163 is not 0, when (the object's field_49 is 0 or
   the player's field_295 is 0) and the object's frame value_a is below the player's
   field_69, or when the object's frame active is 0, and tests the player's three box sets
   (boxes_a, boxes_b, boxes_c, each indexed by the player's frame and tested only when that
   index is not 0) against the box of the table object's current frame (func_801397d0).
   Each box set that overlaps adds its bit (1, 2, 4) to data_80188f30 and records the
   overlap's corners and size in the record at data_80188ed0. When any overlapped, the object hits the
   player: the callees apply the hit, the player and an effect object get state bytes, and
   the table object counts the hit (active_count + 1, a countdown in health with two state
   bytes sets). */
void func_8013bfa4(void) {
    s16 i;
    Box32 *box;
    s16 size;
    s16 last;
    int limit;
    /* Reading count_8018f59c once into last and decrementing it afterwards: the plain
       "last = count_8018f59c - 1" after the test differs in 33 instruction lines. */
    last = count_8018f59c;
    if (last == 0) {
        return;
    }
    last--;
    ((ObjectStackRef *)&ref_other)->pp = (Object **)data_8018f5e0;
    for (i = 0; i <= last; i++) {
        ref_third.p = *((ObjectStackRef *)&ref_other)->pp--;
        if (ref_third.p->active_count != 1) {
            continue;
        }
        ref_second.p = &player_left;
        if (ref_third.p->team_id == 0) {
            ref_second.p++;
        }
        if (ref_second.p->countdown_4 != 0) {
            continue;
        }
        if (ref_second.p->other_lock != 0) {
            continue;
        }
        if (ref_second.p->field_163 != 0) {
            continue;
        }
        /* A continue inside the wrapper leaves the wrapper, not the for body; the wrapper is the last statement of the body, so the behaviour is the same. */
        /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        do {
            if (ref_third.p->field_49 == 0 || ref_second.p->field_295 == 0) {
                /* Without the local limit the unit differs in 1 instruction line. */
                limit = ref_second.p->field_69;
                if (ref_third.p->frame->value_a < limit) {
                    continue;
                }
            }
            if (ref_third.p->frame->active == 0) {
                continue;
            }
            /* The box table is read back from data_80190458; taking it from the object again
               differs in 41 instruction lines. */
            data_80190458.p = (Object *)ref_third.p->box_tables;
            box = (Box32 *)((BoxTables *)data_80190458.p)->boxes_b;
            data_801904c0.p = data_80190458.p;
            box += ref_third.p->frame->active;
            ref_first.p = (Object *)box;
            func_80139a1c();
            if (ref_second.p->frame->box_a != 0) {
                if (func_801397d0((s32)ref_third.p, ref_second.p, ref_second.p->boxes_a + ref_second.p->frame->box_a) == 0) {
                    data_80188f30++;
                    size = data_80188f0c - data_80188f10;
                    data_80188edc = data_80188f0c;
                    data_80188ef4 = data_80188f10;
                    if (size < 0) {
                        size = -size;
                    }
                    size_80188ed0 = size;
                    size = data_80188f14 - data_80188f18;
                    data_80188ee0 = data_80188f14;
                    data_80188ef8 = data_80188f18;
                    if (size < 0) {
                        size = -size;
                    }
                    size_80188ed0 += size;
                }
            }
            if (ref_second.p->frame->box_b != 0) {
                if (func_801397d0((s32)ref_third.p, ref_second.p, ref_second.p->boxes_b + ref_second.p->frame->box_b) == 0) {
                    data_80188f30 += 2;
                    size = data_80188f0c - data_80188f10;
                    data_80188ee4 = data_80188f0c;
                    data_80188efc = data_80188f10;
                    if (size < 0) {
                        size = -size;
                    }
                    size_80188ed4 = size;
                    size = data_80188f14 - data_80188f18;
                    data_80188ee8 = data_80188f14;
                    data_80188f00 = data_80188f18;
                    if (size < 0) {
                        size = -size;
                    }
                    size_80188ed4 += size;
                }
            }
            if (ref_second.p->frame->box_c != 0) {
                if (func_801397d0((s32)ref_third.p, ref_second.p, ref_second.p->boxes_c + ref_second.p->frame->box_c) == 0) {
                    data_80188f30 += 4;
                    size = data_80188f0c - data_80188f10;
                    data_80188eec = data_80188f0c;
                    data_80188f04 = data_80188f10;
                    if (size < 0) {
                        size = -size;
                    }
                    size_80188ed8 = size;
                    size = data_80188f14 - data_80188f18;
                    data_80188ef0 = data_80188f14;
                    data_80188f08 = data_80188f18;
                    if (size < 0) {
                        size = -size;
                    }
                    size_80188ed8 += size;
                }
            }
            if (data_80188f30 == 0) {
                continue;
            }
            ref_second.p->field_241 = 1;
            func_80139a78(ref_third.p, ref_second.p);
            if (ref_third.p->field_02 == 0x17) {
                ref_second.p->field_241 = 0;
                if (ref_second.p->hit_type != 0xff) {
                    if (ref_second.p->active_state == 0) {
                        ref_second.p->hit_type = 0;
                    }
                }
            }
            func_8013a3a8(ref_third.p, ref_second.p, box);
            /* health is u16; the signed test is the original's (a compare with 0x8000 differs in 17 instruction lines). */
            if (ref_second.p->hit_type != 0xff && (s16)ref_second.p->health >= 0) {
                /* The override: the count is masked, as the console's shift does it. */
                if (0x800000 >> (ref_third.p->field_02 & 31)) {
                    ((StateBytes *)data_80190468.p)->state = 0xf;
                    ((StateBytes *)data_80190468.p)->mode = 2;
                    ((StateBytes *)data_80190468.p)->mode2 = 2;
                }
                if (ref_third.p->field_49 != 0) {
                    ((StateBytes *)data_80190468.p)->state = 7;
                    ((StateBytes *)data_80190468.p)->mode = 1;
                    ((StateBytes *)data_80190468.p)->mode2 = 1;
                }
            }
            func_8013af1c(ref_third.p, ref_second.p, box);
            func_80155de0((Actor *)box, ref_second.p);
            /* The original's instructions here: move v0,zero; a load; bnez v0. The mask lies outside
               the width of the byte field. Without the test the unit d3bfa4_r1.c has 448 instructions
               instead of 450 and differs in 12 instruction lines. */
            /* The test below is a stand-in, not recovered source: the original has a branch here on a value its compiler reduced to the constant 0, so it is never taken. The bytes do not say which condition the source had; this one builds the original's instructions. */
            if (ref_third.p->field_02 & 0x100) {
                continue;
            }
            ref_third.p->active_count += 1;
            ref_third.p->state_0 = 1;
            ref_third.p->state_1 = 1;
            ref_third.p->state_2 = 0;
            ref_third.p->state_3 = 0;
            ref_third.p->health -= 1;
            if ((s16)ref_third.p->health < 0) {
                ref_third.p->state_0 = 2;
                ref_third.p->state_1 = 0;
                ref_third.p->state_2 = 0;
                ref_third.p->state_3 = 0;
            }
        } while (0);
    }
}
