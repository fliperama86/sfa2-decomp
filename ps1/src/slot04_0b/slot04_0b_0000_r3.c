/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b05bc_slot04_0b(Object *obj);

void func_801b04e8_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        func_801b05bc_slot04_0b(obj);
        func_80130678(obj, 0);
    } else {
        update_sequence(obj);
    }
}
