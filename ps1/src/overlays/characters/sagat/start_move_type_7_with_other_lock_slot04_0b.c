/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_move_type_7_with_other_lock_slot04_0b(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->field_15a = 7;
    obj->active_flag_3 = 1;
    obj->active_flag_4 = 1;
    obj->direction_index = 4;
    obj->other_lock = 0x17;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->facing_side = obj->saved_facing;
    obj->other->field_6b = 0x13;
    spawn_offset_effect_and_play_sounds(obj);
    set_sequence_by_side_and_mode(obj, 0x2b);
}
