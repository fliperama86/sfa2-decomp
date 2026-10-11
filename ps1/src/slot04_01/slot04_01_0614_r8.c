/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3ad8_slot04_01(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    }
}
