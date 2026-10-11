/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);

void func_801b29f8_slot04_09(Object *obj) {
    Object *p;
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        p = obj->other;
        obj->state_3 = 6;
        p->field_6b = 0;
        func_801307e0(obj, 0x2a);
    } else {
        p = obj->other;
        p->field_6b = 8;
        if (func_8014048c(obj, -0x40, 0x40, 0x30, 0x31) != 0) {
            obj->state_3++;
            obj->step_timer = 0;
            obj->step_flags = obj->step_flags & 0xff00;
            obj->field_45 = 1;
            obj->vel_x = 0x20000;
            obj->vel_y = 0x60000;
            obj->accel_x = 0;
            obj->accel_y = -0x6000;
            if (obj->field_0b == 0) {
                obj->vel_x = 0xfffe0000;
            }
            func_801307e0(obj, 0x29);
        }
        func_801b2858_slot04_09(obj);
    }
}
