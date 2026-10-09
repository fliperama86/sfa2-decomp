/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130dc0(Object *object);
u8 func_8013f8c4(Object *object, int a, int b);

void func_801b038c_slot04_03(Object *obj) {
    int one = 1;

    obj->field_159 = one;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
        if (func_8013f8c4(obj, -0x18, 0x10) != 0) {
            obj->field_04 = one;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
            obj->field_128 = 0;
        } else if (obj->field_12a == 4 && (obj->field_130 & 0x8000) != 0) {
            func_80141f28(obj, 2);
            func_80130ec0(obj);
            func_801307e0(obj, 0x1b);
        } else {
            func_80130dc0(obj);
        }
    } else {
        func_80130dc0(obj);
    }
}
