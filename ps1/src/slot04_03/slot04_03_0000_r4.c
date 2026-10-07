/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130dc0(Object *object);
u8 func_8013f8c4(Object *object, int a, int b);

void func_801b04e8_slot04_03(Object *obj) {
    int one = 1;

    obj->field_159 = one;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
        if (func_8013f8c4(obj, -0x18, 0x10) != 0) {
            obj->field_04 = one;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
            obj->field_128 = 2;
        } else if (obj->field_12a != 2) {
            if ((obj->field_130 & 0xa000) != 0) {
                obj->field_07 = 2;
                *(s16 *)&obj->field_46 = -1;
                obj->field_48 = obj->field_0b ^ 1;
                func_80141f28(obj, 2);
                func_80130ec0(obj);
                obj->field_278 = one;
                func_801307e0(obj, 0x1c);
            } else {
                func_80130dc0(obj);
            }
        } else if ((obj->field_130 & 0xa000) != 0 && obj->field_262 != 0) {
            obj->field_50 = -0x40000;
            obj->field_58 = 0x6000;
            if (obj->field_0b != 0) {
                obj->field_4c = 0x36000;
            } else {
                obj->field_4c = -0x36000;
            }
            obj->field_14 = 0;
            obj->field_07 = 3;
            func_80141f28(obj, 1);
            func_80130ec0(obj);
            obj->field_278 = 1;
            obj->field_29a = 1;
            func_801307e0(obj, 0x1d);
        } else {
            func_80130dc0(obj);
        }
    } else {
        func_80130dc0(obj);
    }
}
