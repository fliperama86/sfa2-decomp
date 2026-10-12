/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b11e4_slot04_0b(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0) {
        if (obj->link_flags != 0) {
            return 0;
        }
    }
    if (func_801417cc(obj)) {
        r++;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 2;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142ba0(obj);
    }
    return r;
}
