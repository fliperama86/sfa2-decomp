/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b39ec_slot04_07(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        func_80140770(obj, 0, 5, 0xf, 0, 1, 0);
    }
    update_sequence(obj);
}

void func_801b3a50_slot04_07(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
