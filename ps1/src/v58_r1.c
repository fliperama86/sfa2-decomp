/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: original ends "bnez v0,end; clear v0; ori v0,1" with early exits
 * jumping to the epilogue (no separate return-0 block), and keeps the first
 * box in $a3; this build uses $a1 and adds a return-0 block. */

/* Form found by automatic permutation search, then cleaned by hand. */
int func_8013c970(Object *object) {
    u8 index = object->frame->active;
    Box32 *mine;
    Box32 *other;

    if (index != 0) {
        mine = (Box32 *)object->box_tables->boxes_b;
        mine += index;
        if (object->field_65 != 0) {
            ref_second.p = &player_left;
        } else {
            ref_second.p = &player_right;
        }
        index = ref_second.p->frame->active;
        if (index != 0) {
            other = ref_second.p->box_tables->wide_boxes;
            ref_first.p = (Object *)mine;
            other += index;
            if (!func_80139928(object, ref_second.p, other)) {
                return 1;
            }
        }
    }
    return 0;
}
