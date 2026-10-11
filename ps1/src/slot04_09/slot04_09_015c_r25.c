/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7dc4_slot04_09[];

void func_801b47c8_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b480c_slot04_09(Object *obj) {
    data_801c7dc4_slot04_09[obj->state_3](obj);
}
