/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c542c_slot04_06[];

void func_801b3004_slot04_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b3044_slot04_06(Object *obj) {
    data_801c542c_slot04_06[obj->state_3](obj);
}
