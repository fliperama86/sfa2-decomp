/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_801bf000_slot04_01[];

void func_801b21fc_slot04_01(Object *o) {
    int t;

    *(s32 *)&o->pos_y_frac -= o->vel_y;
    o->vel_y += o->accel_y;
    if (o->vel_y < 0) {
        o->state_3++;
        func_801307e0(o, 0x2b);
    } else {
        if (o->field_cd == 0) {
            t = o->field_134 & 0xfc;
        } else {
            t = func_8014a170(o, data_801bf000_slot04_01);
        }
        if (t != 0) {
            o->step_timer = 1;
        }
        func_80130efc(o);
    }
}
