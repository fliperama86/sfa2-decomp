/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_hit_other_on_flag_1_a_slot04_0b(Object *obj) {
    u8 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t == 1) {
        set_other_hit_reaction_state_0(obj, 0, 5, obj->offset_index == 0 ? 9 : 0, 0, 0, t);
        obj->state_3 = 5;
    }
}

void update_sequence_hit_other_on_flag_1_b_slot04_0b(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 1) {
        set_other_hit_reaction_state_0(obj, 0, 5, obj->offset_index == 0 ? 9 : 0, 0, 0, 0);
        obj->state_3 = 5;
    }
}
