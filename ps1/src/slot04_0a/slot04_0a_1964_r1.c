/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1aa4_slot04_0a(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->field_45 = 1;
        obj->field_67 = 0;
        obj->state_3++;
        func_801307e0(obj, 0x22);
    }
}
