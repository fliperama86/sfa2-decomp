/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void put_other_in_hit_state_on_step_flag_a_slot04_07(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 5, 0, 0, 1, 0);
    }
    update_sequence(obj);
}

void put_other_in_hit_state_then_update_b_slot04_07(Object *obj) {
    set_other_hit_reaction_state_0(obj, 0, 5, 0, 0, 0, 1);
    obj->state_3 = obj->state_3 + 1;
    update_sequence(obj);
}
