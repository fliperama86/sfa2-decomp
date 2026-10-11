/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b010c_slot04_05(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        func_80130678(obj, 0);
    } else {
        update_sequence(obj);
    }
}
