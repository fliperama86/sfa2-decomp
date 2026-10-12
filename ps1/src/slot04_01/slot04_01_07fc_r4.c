/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0ce0_slot04_01(Object *obj) {
    int r = 0;

    if (func_801417cc(obj) != 0) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->facing_side = obj->saved_facing;
        func_80142b3c(obj);
    }
    return r;
}
