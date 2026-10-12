/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_active_flag_3_and_call_direction_sequence_slot04_04(Object *obj);

void start_leap_if_direction_4_and_param_d_slot04_04(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index == 4 && obj->attack_param_d != 0) {
        obj->active_flag_3 = 1;
        obj->state_3++;
        add_to_meter_clamped(obj, 2);
        obj->vel_y = 0x78000;
        obj->accel_x = 0;
        obj->accel_y = -0x5000;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x18000;
        } else {
            obj->vel_x = 0xfffe8000;
        }
        obj->active_state = 1;
        obj->field_157 = 0;
        obj->pos_y -= 0x10;
        set_sequence_by_side_and_mode(obj, 0x1b);
    } else {
        set_active_flag_3_and_call_direction_sequence_slot04_04(obj);
    }
}
