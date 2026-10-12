/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_object_states_0_to_3_2_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void start_state_hit_or_diagonal_jump_slot04_02(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if (func_8013f8c4(obj, -0x14, 0x14) & 0xff) {
            set_object_states_0_to_3_2_slot04_02(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->direction_index == 2 && obj->attack_param_d != 0) {
            obj->active_flag_3 = 1;
            obj->state_3 = 2;
            obj->field_157 = 0;
            add_to_meter_clamped(obj, 1);
            if (obj->facing_side != 0) {
                obj->vel_x = 0x36000;
            } else {
                obj->vel_x = -0x36000;
            }
            obj->vel_y = 0x40000;
            obj->accel_y = -0x6000;
            set_sequence_by_side_and_mode(obj, 0x40);
            return;
        }
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}
