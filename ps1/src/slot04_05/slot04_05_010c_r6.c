/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0e90_slot04_05(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142ba0(obj);
    }
    return r;
}
