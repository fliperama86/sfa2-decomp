/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b22a8_slot04_0a(Object *obj) {
    u16 t = obj->step_flags;

    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
        obj->active_flag_4 = 0;
    }
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}
