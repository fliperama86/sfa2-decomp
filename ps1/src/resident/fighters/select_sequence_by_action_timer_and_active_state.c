/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void select_sequence_by_action_timer_and_active_state(Object *object) {
    int index;
    if (object->action_timer != 0) {
        init_fixed_sequence_c_and_clear_counters(object);
        return;
    }
    object->facing_side = object->saved_facing;
    if (object->active_state != 0) {
        object->field_60 = 3;
        object->active_state = 0xff;
        set_sequence_by_side_and_mode_2(object, 0x12);
        if (object->field_6b == 0) {
            func_8012cca8(object);
        }
        return;
    }
    object->field_60 = 1;
    object->countdown = 0;
    if (object->mode_flag_2 != 0) {
        set_sequence_c_or_e_by_other_field_flag(object);
        return;
    }
    index = 0xe;
    if (object->action_timer != 0) {
        object->field_157 = 0;
    }
    if (object->field_157 == 0) {
        index = 0xc;
    }
    set_sequence_by_side_and_mode_2(object, index);
}
