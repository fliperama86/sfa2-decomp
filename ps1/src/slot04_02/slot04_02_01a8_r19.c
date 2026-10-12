/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1f10_slot04_02(Object *obj) {
    s16 t;

    if (!((s16)obj->step_flags & 0x8000)) {
        t = obj->countdown;
        if (t & 0xff00) {
            t = t - 0x100;
            obj->countdown = t;
            if (!(t & 0xff00)) {
                obj->active_flag_4 = 0;
                func_80142adc(obj);
            }
        } else {
            func_80142adc(obj);
        }
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}
