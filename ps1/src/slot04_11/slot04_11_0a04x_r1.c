/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80142b3c(Object *object);

/* functions of other units of this module */

int func_801b0a04_slot04_11(Object *obj);

int func_801b0a04_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    if (func_801417cc(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142b3c(obj);
        return 1;
    } else {
        return 0;
    }
}
