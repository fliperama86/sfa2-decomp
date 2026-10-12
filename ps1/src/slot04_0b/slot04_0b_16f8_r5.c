/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1b4c_slot04_0b(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->active_flag_4 = 0;
        func_80142adc(obj);
    }
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
