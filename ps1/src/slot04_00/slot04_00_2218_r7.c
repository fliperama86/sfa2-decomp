/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b291c_slot04_00(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3++;
        func_801307e0(obj, 0x3a);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            func_80146998(obj);
        }
        update_sequence(obj);
    }
}
