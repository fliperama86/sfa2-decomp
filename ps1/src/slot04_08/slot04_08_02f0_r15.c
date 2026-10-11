/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3d6c_slot04_08[];

void func_801b372c_slot04_08(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b3770_slot04_08(Object *obj) {
    data_801c3d6c_slot04_08[obj->state_3](obj);
}
