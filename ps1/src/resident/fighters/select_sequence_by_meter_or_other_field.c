/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Historical note, from before this unit was exact or about a function that is no longer in this unit: NOT EXACT (200 vs 208 bytes). Residual: the original keeps two separate
 * 'a1 = 0xc; j' blocks (for action_timer and for field_157 == 0); the compiler
 * cross-jumps them into one. Tried explicit calls, goto, ternary, merged tests. */
void select_sequence_by_meter_or_other_field(Object *o) {
    int state;
    if ((s16)o->meter >= 0x30 && (s16)o->health >= 0 && o->field_221) {
        call_by_other_active_state_and_side(o);
        return;
    }
    o->field_157 = 0;
    if (o->action_timer) {
        set_sequence_by_side_and_mode_2(o, 0xc);
        return;
    }
    ref_other.p = o->other;
    if (ref_other.p->active_state != 0) {
        set_sequence_by_side_and_mode_2(o, 0xc);
        return;
    }
    if (ref_other.p->field_157 == 0) {
        set_sequence_by_side_and_mode_2(o, 0xc);
        return;
    }
    o->field_157++;
    set_sequence_by_side_and_mode_2(o, 0xe);
}
