/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c98_slot04_09(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if ((t & 1) == 0) {
            obj->active_flag_4 = 0;
            func_80142adc(obj);
        }
        update_sequence(obj);
    }
}
