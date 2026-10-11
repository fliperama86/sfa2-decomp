/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1f20_slot04_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
