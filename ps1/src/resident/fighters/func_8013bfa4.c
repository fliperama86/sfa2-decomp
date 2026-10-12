/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

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
   index is not 0) against the box of the table object's current frame (is_box_separated_2).
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
            reset_hit_results();
            if (ref_second.p->frame->box_a != 0) {
                if (is_box_separated_2((s32)ref_third.p, ref_second.p, ref_second.p->boxes_a + ref_second.p->frame->box_a) == 0) {
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
                if (is_box_separated_2((s32)ref_third.p, ref_second.p, ref_second.p->boxes_b + ref_second.p->frame->box_b) == 0) {
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
                if (is_box_separated_2((s32)ref_third.p, ref_second.p, ref_second.p->boxes_c + ref_second.p->frame->box_c) == 0) {
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
            apply_hit_box_to_target(ref_third.p, ref_second.p);
            if (ref_third.p->field_02 == 0x17) {
                ref_second.p->field_241 = 0;
                if (ref_second.p->hit_type != 0xff) {
                    if (ref_second.p->active_state == 0) {
                        ref_second.p->hit_type = 0;
                    }
                }
            }
            apply_hit_damage_to_object(ref_third.p, ref_second.p, box);
            /* health is u16; the signed test is the original's (a compare with 0x8000 differs in 17 instruction lines). */
            if (ref_second.p->hit_type != 0xff && (s16)ref_second.p->health >= 0) {
                /* Masking the shift count with 31 differs in 16 instruction lines. */
                if (0x800000 >> ref_third.p->field_02) {
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
            select_min_placement_and_test(ref_third.p, ref_second.p, box);
            add_table_score_to_player_on_hit((Actor *)box, ref_second.p);
            /* The original's instructions here: move v0,zero; a load; bnez v0. The mask lies outside
               the width of the byte field. Without the test the unit has 448 instructions instead of
               450 and differs in 12 instruction lines. */
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


