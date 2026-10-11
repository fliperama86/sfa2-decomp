/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2d00_slot04_00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
