/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b18d8_slot04_00(Object *obj) {
    s16 t;

    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        t = obj->countdown;
        if (t != 0) {
            t = t - 1;
            obj->countdown = t;
            if (t != 0) {
                goto tail;
            }
            obj->active_flag_4 = 0;
        }
        func_80142adc(obj);
    tail:
        update_sequence(obj);
    }
}
