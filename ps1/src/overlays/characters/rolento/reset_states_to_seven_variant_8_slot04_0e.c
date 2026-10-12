/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void reset_states_to_seven_variant_8_slot04_0e(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 8;
    obj->field_6b = 0;
    obj->other->field_6b = 0x15;
    obj->direction_index = 0;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
}

void reset_states_to_seven_variant_9_slot04_0e(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 9;
    obj->field_6b = 0;
    obj->other->field_6b = 0x15;
    obj->direction_index = 0;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
}
