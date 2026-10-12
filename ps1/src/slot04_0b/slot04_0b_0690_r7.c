/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b184c_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 5;
        func_80131468(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}
