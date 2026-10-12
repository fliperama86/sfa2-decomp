/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b104c_slot04_06(Object *obj) {
    if (!(u8)func_80141c4c(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 6;
    obj->active_flag_3 = 1;
    obj->direction_index = 2;
    return 1;
}

int func_801b10b4_slot04_06(Object *obj) {
    if (obj->field_7e != 0 || obj->remaining_count != 0) {
        if (func_801417cc(obj) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 7;
            obj->state_3 = 0;
            obj->field_15a = 7;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            return 1;
        }
    }
    return 0;
}
