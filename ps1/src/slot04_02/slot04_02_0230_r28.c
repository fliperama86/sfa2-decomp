/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c65bc_slot04_02[];

void func_801b6800_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b6844_slot04_02(Object *obj) {
    data_801c65bc_slot04_02[obj->state_3](obj);
}
