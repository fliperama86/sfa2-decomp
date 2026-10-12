/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1000_slot04_08(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0 && obj->remaining_count == 0) return 0;
    if (func_801417cc(obj)) {
        r++;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}
