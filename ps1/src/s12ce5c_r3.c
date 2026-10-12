/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void set_sequence_c_or_e_by_pressed_bits(Object *object) {
    int index;
    if (object->pressed_bits == 0) {
        index = 0xc;
    } else {
        object->field_157 = object->field_157 + 1;
        object->action_timer = 0;
        index = 0xe;
    }
    set_sequence_by_side_and_mode_2(object, index);
}

void call_meter_action_or_default_sequence(Object *object) {
    if ((s16)object->meter >= 0x30 && (s16)object->health >= 0 && object->field_221 != 0) {
        call_by_other_active_state_and_side(object);
    } else {
        set_sequence_by_action_timer_and_other(object);
    }
}
