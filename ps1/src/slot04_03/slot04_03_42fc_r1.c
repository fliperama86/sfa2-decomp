/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013f8c4(Object *object, int a, int b);

void func_801b42fc_slot04_03(Object *obj) {
    obj->field_159 = 1;
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0) {
        if (obj->field_218 != 0 && func_8013f8c4(obj, -0x18, 0x14) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
            obj->field_128 = 0;
        } else if (obj->field_12a != 2) {
            if (obj->field_219 != 0 && obj->field_21a == 0) {
                obj->field_07 = 2;
                *(s16 *)&obj->field_46 = -1;
                func_80141f28(obj, 2);
                func_801307e0(obj, 0x1c);
            } else {
                func_80130dc0(obj);
            }
        } else if (obj->field_219 != 0) {
            obj->field_07 = 3;
            obj->field_50 = 0xfffc0000;
            obj->field_58 = 0x6000;
            if (obj->field_0b != (obj->field_21a ^ 1)) {
                obj->field_4c = 0xfffca000;
            } else {
                obj->field_4c = 0x36000;
            }
            func_80141f28(obj, 1);
            func_801307e0(obj, 0x1d);
        } else {
            func_80130dc0(obj);
        }
    } else {
        func_80130dc0(obj);
    }
}
