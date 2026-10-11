/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b478c_slot04_04(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        func_80130efc(obj);
    }
}
