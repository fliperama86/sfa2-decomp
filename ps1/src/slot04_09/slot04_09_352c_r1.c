/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3590_slot04_09(Object *obj);

void func_801b352c_slot04_09(Object *obj) {
    obj->state_3++;
    *(s32 *)&obj->vel_y = 0;
    *(s32 *)&obj->accel_y = 0;
    if (obj->field_0b != 0) {
        obj->vel_x = 0x40000;
        obj->accel_x = 0xa000;
    } else {
        obj->vel_x = -0x40000;
        obj->accel_x = 0xffff6000;
    }
    func_801b3590_slot04_09(obj);
}

void func_801b3590_slot04_09(Object *obj) {
    u16 t;
    u8 b;
    func_80130efc(obj);
    t = obj->step_flags;
    b = t;
    if (b == 0) {
        obj->state_3++;
        obj->field_27b = 0;
        obj->field_165 = 0;
        obj->step_flags = obj->step_flags & 0xff00;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            obj->field_27b = 0xc;
        }
    } else if (b != 2) {
        obj->step_flags = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x2c, 0x40);
    }
}
