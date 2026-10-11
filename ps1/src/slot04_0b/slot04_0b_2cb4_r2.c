/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2ebc_slot04_0b(Object *obj) {
    obj->vel_x = 0x98000;
    obj->vel_y = 0x90000;
    obj->accel_x = -0x8000;
    obj->accel_y = -0x6000;
    obj->state_3++;
}

void func_801b2ef0_slot04_0b(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0xff00) {
        obj->step_flags = t & 0xff;
        func_801204f4(obj, obj->side, 0xf);
        func_801204f4(obj, obj->side, 4);
    }
    if (*(u8 *)&obj->step_flags == 0) {
        obj->field_45 = 1;
        obj->state_3++;
    }
    func_80130efc(obj);
}
