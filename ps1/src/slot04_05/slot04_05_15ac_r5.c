/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1b44_slot04_05(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3 = 1;
        obj->vel_x = 0xa0000;
        obj->accel_x = -0x8000;
        obj->vel_y = 0x80000;
        obj->accel_y = -0x6000;
        func_801307e0(obj, 0x1b);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            func_80146998(obj);
            obj->other->field_6b = 0x14;
        }
        update_sequence(obj);
    }
}

void func_801b1be0_slot04_05(Object *obj) {
    if (obj->field_49 != 0 && (obj->vel_y & 0x8000)) {
        obj->accel_y = 0xffff0000;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}
