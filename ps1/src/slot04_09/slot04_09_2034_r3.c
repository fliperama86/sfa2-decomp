/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b231c_slot04_09(Object *obj) {
    if (!((s16)obj->step_flags & 0x8000)) {
        update_sequence(obj);
    } else {
        obj->field_45 = 1;
        obj->vel_x = 0x80000;
        obj->vel_y = 0x80000;
        obj->accel_x = -0x8000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        if (obj->field_0b == 0) {
            obj->vel_x = 0xfff80000;
            obj->accel_x = 0x8000;
        }
        func_801307e0(obj, 0x47);
    }
}
