/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_move_with_dash_or_sequence_slot04_07(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 4) {
        set_sequence_and_tables_from_direction(obj);
    } else if (obj->attack_param_d == 0) {
        obj->state_3 = 2;
        play_sound_at(obj, obj->side, 0x324);
        obj->vel_x = 0x90000;
        obj->accel_x = -0x8000;
        set_sequence_and_tables_from_direction(obj);
    } else {
        obj->state_3 = 3;
        obj->field_157 = 0;
        add_to_meter_clamped(obj, 2);
        set_sequence_by_side_and_mode(obj, 0x2d);
    }
}
