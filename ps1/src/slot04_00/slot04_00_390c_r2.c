/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b39dc_slot04_00(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if (func_8013f8c4(obj, -0x14, 0x14) & 0xff) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            return;
        }
        if (obj->field_12a == 2 && obj->field_219 != 0) {
            obj->state_3 = 2;
            func_80141f28(obj, 1);
            if (obj->field_0b == 0) {
                obj->vel_x = -0x36000;
            } else {
                obj->vel_x = 0x36000;
            }
            obj->vel_y = 0x40000;
            obj->accel_y = -0x6000;
            func_801307e0(obj, 0x2f);
            return;
        }
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}
