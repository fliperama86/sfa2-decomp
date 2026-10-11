/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b14a0_slot04_01(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3 = 1;
        obj->vel_x = 0xb0000;
        obj->accel_x = -0x8000;
        obj->vel_y = 0xa0000;
        obj->accel_y = -0x6000;
        func_801204f4(obj, obj->side, 0xb);
        func_801307e0(obj, 0x30);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            func_80146998(obj);
        }
        update_sequence(obj);
    }
}

