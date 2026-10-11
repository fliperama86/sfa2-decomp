/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b54f8_slot04_02(Object *obj) {
    Object *p;
    int a;
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        p = obj->other;
        p->field_15b = 1;
        func_80140770(obj, 6, 0xf, -0x200, 0, 1, 0);
        if ((s16)p->field_5c < 0) {
            a = obj->field_49;
            if (a != 0) {
                a = 0x12;
            } else {
                a = 2;
            }
            obj->field_167 = a;
        }
        obj->vel_x = 0x28000;
        obj->vel_y = 0x70000;
        obj->accel_x = 0;
        obj->accel_y = -0x5400;
    }
}
