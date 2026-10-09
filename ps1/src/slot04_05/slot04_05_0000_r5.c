/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0654_slot04_05(Object *obj) {
    int one;

    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x17, 0x11) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
        } else if (obj->field_12a == 2 && (obj->field_130 & 0x8000) != 0 && obj->field_262 != 0) {
            one = 1;
            obj->field_159 = one;
            obj->field_07 = 2;
            obj->field_157 = 0;
            func_80141f28(obj, 1);
            obj->field_4c = 0x68000;
            obj->field_54 = -0x8000;
            func_80130ec0(obj);
            obj->field_278 = one;
            obj->field_29a = one;
            func_801307e0(obj, 0x34);
        } else {
            obj->field_159 = 1;
            func_80130dc0(obj);
        }
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
