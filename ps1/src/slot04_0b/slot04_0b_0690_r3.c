/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_start_meter_move_type_a_slot04_0b(Object *obj) {
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
                        obj->field_15a = 0xa;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            } else if (obj->field_7e == 0) {
                if ((u8)func_801418bc(obj)) {
                    r = 1;
                    obj->state_0 = 1;
                    obj->state_1 = 0;
                    obj->state_2 = 8;
                    obj->state_3 = 0;
                    obj->field_15a = 0xa;
                }
            }
        }
    }
    return r;
}
