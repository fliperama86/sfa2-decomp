/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b20a8_slot04_04(Object *obj) {
    s16 t = obj->countdown;

    if (t & 0xff00) {
        t = t - 0x100;
        obj->countdown = t;
        if ((t & 0xff00) == 0) {
            obj->active_flag_4 = 0;
        }
    }
    if ((s16)obj->step_flags & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}
