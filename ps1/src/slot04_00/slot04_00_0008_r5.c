/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0614_slot04_00(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        } else if (obj->field_12a == 2 && (obj->field_130 & 0x8000) != 0) {
            obj->state_3 = 2;
            obj->field_159 = 1;
            obj->field_157 = 0;
            func_80141f28(obj, 1);
            if (obj->field_0b == 0) {
                obj->vel_x = 0xfffca000;
            } else {
                obj->vel_x = 0x36000;
            }
            obj->vel_y = 0x40000;
            obj->accel_y = -0x6000;
            func_80130ec0(obj);
            obj->field_278 = 1;
            func_801307e0(obj, 0x2f);
        } else {
            obj->field_159 = 1;
            func_80130dc0(obj);
        }
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
