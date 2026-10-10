/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build keeps the original bytes of the resident executable and does not use
 * this file. The differential test next to it (difftest.py, with
 * func_8013b558.py) compares the behavior of this C with the original code
 * on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): walks the pairs of objects
 * of the object table (data_8018f5e0, filled downward from its first word,
 * count_8018f59c entries): the outer object a takes all count entries, the
 * inner object b only the first count - 1. For each pair it skips pairs
 * that cannot collide, and otherwise looks up the box of each object's
 * current frame in the object's box tables, publishes a's box and both
 * table pointers in global slots (b's box is only passed on), and calls
 * func_801397d0 with a, b and b's box (a test of the two boxes). If that
 * returns non-zero the scan for this outer object ends; otherwise
 * func_8013b7b8 is called (it dispatches on the two objects' kinds;
 * inferred).
 * Per pair, in this order: same object: skip; a of kind 0x17: end the scan;
 * b of kind 0x17: skip; a of a type other
 * than 1: end the scan for a; b of another type, equal field_65, or a
 * frame without a box (box_a == 0): skip b (a without a box ends the scan).
 *
 * Contract (the roles named for the fields are inferred):
 *   No argument, no return value.
 *   Reads: count_8018f59c (a count, 16 bits, signed), the object table
 *     data_8018f5e0 (words at 0, -4, -8, ... from its start), the objects'
 *     field_00, field_02, field_65, box_tables, frame; the frame's box_a;
 *     each box_tables' first word; the global ref_third after the callee
 *     returns.
 *   Writes: ref_other (a pointer into the table), ref_third (the outer
 *     object), ref_second (the inner object), data_80190458 (the box_tables
 *     of a), ref_first (the box found for a), data_80190414 (the box_tables
 *     of b); all as pointer words.
 *   Replaced callees: func_801397d0 (three arguments: a, b, the box of b;
 *     returns a byte) is a recorder whose result is 0 in three cases of
 *     seven, 0x100 (low byte 0) in one and a value with a non-zero low byte
 *     in three; func_8013b7b8 (no argument) is a recorder.
 *   Watched by the recorders at every call: the words from ref_first to the
 *     one after data_80190414, data_80190458, ref_other and ref_third.
 *   Aliasing: the table, the objects, their frames and box tables are
 *     distinct blocks; the table's entries may repeat an object.
 *   Excluded: a count above 12, the number of table entries the setup fills
 *     (the original would read past the table; inferred).
 *   Not reached by any input: none expected; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Globals as the original uses them (inferred types; not original
   declarations). They hold the object and box pointers that the callees read. */
extern ObjectRef ref_third;
extern ObjectRef data_80190414;
extern ObjectRef data_80190458;
void func_8013b7b8(void);

void func_8013b558(void) {
    s16 last;
    s16 count;
    s16 m;
    s16 i;
    s16 j;
    Object **outer;
    Object **inner;
    Object *a;
    Object *b;
    Box6 *box_a;
    Box6 *box_b;

    last = count_8018f59c;
    if (last == 0) return;
    last = last - 1;
    if (last == 0) return;
    ((ObjectStackRef *)&ref_other)->pp = (Object **)data_8018f5e0;
    outer = (Object **)data_8018f5e0;
    for (i = 0; i <= last; i++) {
        ((ObjectStackRef *)&ref_other)->pp = outer - 1;
        ref_third.p = *outer;
        outer--;
        count = count_8018f59c;
        if (count == 0) return;
        ((ObjectStackRef *)&ref_other)->pp = (Object **)data_8018f5e0;
        inner = (Object **)data_8018f5e0;
        m = count - 1;
        for (j = 0; j < m; j++) {
            ((ObjectStackRef *)&ref_other)->pp = inner - 1;
            ref_second.p = *inner;
            b = ref_second.p;
            inner--;
            a = ref_third.p;
            if (b == a) continue;
            if (a->field_02 == 0x17) break;
            if (b->field_02 == 0x17) continue;
            if (a->field_00 != 1) break;
            if (b->field_00 != a->field_00) continue;
            if (a->field_65 == b->field_65) continue;
            if (a->frame->box_a == 0) break;
            if (b->frame->box_a == 0) continue;
            data_80190458.p = (Object *)a->box_tables;
            box_a = a->box_tables->boxes_a + a->frame->box_a;
            ref_first.p = (Object *)box_a;
            data_80190414.p = (Object *)b->box_tables;
            box_b = b->box_tables->boxes_a + b->frame->box_a;
            if (func_801397d0((s32)a, b, box_b)) break;
            func_8013b7b8();
        }
    }
}
