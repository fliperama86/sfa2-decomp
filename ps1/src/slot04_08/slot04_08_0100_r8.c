/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4864_slot04_08(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            if (func_80149b80(obj) & 0xff) {
                obj->state_3 = 0;
            }
        }
        update_sequence(obj);
    }
}
