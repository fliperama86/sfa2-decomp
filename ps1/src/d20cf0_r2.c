/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

void call_kind_pair_handler(void);

/* What it does (inferred): walks the pairs of objects of the object table
   (count_8018f59c entries): the outer object takes all entries, the inner one
   all but the last. A pair that cannot collide is skipped; otherwise the box
   of each object's current frame is looked up in its box tables, a's box and
   both table pointers are published in global slots, and func_801397d0 is
   called with a, b and b's box. A non-zero result ends the scan for this outer
   object; otherwise call_kind_pair_handler is called. */
void func_8013b558(void) {
    Box6 *box_a;
    Box6 *box_b;
    int index_a;
    s16 last;
    s16 i;
    s16 j;
    s16 m;
    s16 count;
    Object **outer;
    ObjectStackRef *ref_slot;
    Object **inner;
    Object *b;
    Object *a;

    last = count_8018f59c;
    if (last == 0)
        return;
    last--;
    if (last == 0)
        return;
    ((ObjectStackRef *)&ref_other)->pp = (Object **)data_8018f5e0;
    outer = (Object **)data_8018f5e0;
    for (i = 0; i <= last; i++) {
        ref_slot = (ObjectStackRef *)&ref_other;
        ref_slot->pp = outer - 1;
        ref_third.p = *outer;
        outer--;
        count = count_8018f59c;
        if (count == 0)
            return;
        ref_slot->pp = (Object **)data_8018f5e0;
        inner = (Object **)data_8018f5e0;
        m = count - 1;
        for (j = 0; j < m; j++) {
            ref_slot->pp = inner - 1;
            ref_second.p = *inner;
            b = ref_second.p;
            inner--;
            a = ref_third.p;
            if (b == a)
                continue;
            if (a->field_02 == 0x17)
                break;
            if (b->field_02 == 0x17)
                continue;
            if (a->active_count != 1)
                break;
            if (b->active_count != a->active_count)
                continue;
            if (a->team_id == b->team_id)
                continue;
            if (a->frame->box_a == 0)
                break;
            if (b->frame->box_a == 0)
                continue;
            box_a = (Box6 *)a->box_tables;
            data_80190458.p = (Object *)box_a;
            /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
            do {
                index_a = a->frame->box_a;
                box_a = *(Box6 **)box_a;
                ref_first.p = (Object *)(box_a + index_a);
            } while (0);
            box_b = (Box6 *)b->box_tables;
            data_80190414.p = (Object *)box_b;
            box_b = *(Box6 **)box_b;
            if (func_801397d0((s32)a, b, box_b + b->frame->box_a))
                break;
            call_kind_pair_handler();
        }
    }
}
