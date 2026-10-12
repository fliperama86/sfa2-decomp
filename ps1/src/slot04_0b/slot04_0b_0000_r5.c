/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0a38_slot04_0b(Object *obj) {
    Object *o = obj;
    if (*(u8 *)&obj->step_flags == 0) {
        o->state_3 = obj->state_3 + 1;
        func_80142a14(o);
    } else {
        if (obj->pressed_bits & 0x20) {
            obj->state_3 = obj->state_3 + 1;
            func_801307e0(obj, 0x49);
        }
        func_80142a14(obj);
    }
}
