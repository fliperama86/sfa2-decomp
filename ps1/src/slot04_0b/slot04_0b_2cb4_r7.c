/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b36d8_slot04_0b(Object *obj) {
    u8 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t == 1) {
        func_80140770(obj, 0, 5, obj->offset_index == 0 ? 9 : 0, 0, 0, t);
        obj->state_3 = 5;
    }
}

void func_801b3740_slot04_0b(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 1) {
        func_80140770(obj, 0, 5, obj->offset_index == 0 ? 9 : 0, 0, 0, 0);
        obj->state_3 = 5;
    }
}
