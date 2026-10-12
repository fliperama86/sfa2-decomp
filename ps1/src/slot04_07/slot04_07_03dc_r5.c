/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_start_meter_move_state_8_type_8_slot04_07(Object *obj) {
    int r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state == 0) {
                if ((u8)is_not_state_2_five(obj)) {
                    if ((u8)func_80141788(obj)) {
                        r = 1;
                        obj->state_0 = 1;
                        obj->state_1 = 0;
                        obj->state_2 = 8;
                        obj->state_3 = 0;
                        obj->field_15a = 8;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            } else if (obj->field_7e == 0) {
                if ((u8)try_action_unless_state_bytes_match(obj)) {
                    r = 1;
                    obj->state_0 = 1;
                    obj->state_1 = 0;
                    obj->state_2 = 8;
                    obj->state_3 = 0;
                    obj->field_15a = 8;
                }
            }
        }
    }
    return r;
}
