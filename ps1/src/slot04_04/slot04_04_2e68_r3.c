/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_0_on_sequence_loop_flag_slot04_04(Object *obj);

void update_linked_object_state_slot04_04(Object *obj) {
    if (obj->state_1 != 0) {
        advance_state_0_on_sequence_loop_flag_slot04_04(obj);
    } else {
        obj->state_1 = obj->state_1 + 1;
        obj->parent->linked_object_3 = 0;
        set_sequence_by_sound_side(obj, 3);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}
