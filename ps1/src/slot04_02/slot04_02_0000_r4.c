/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, int a, int b, int c, int d);
u8 func_8013f8c4(Object *object, int a, int b);
void func_80130dc0(Object *object);

void func_801b0464_slot04_02(Object *obj) {
    int one = 1;

    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
        if (func_8013f8c4(obj, -0x14, 0x14) != 0) {
            func_801b631c_slot04_02(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->field_12a == 2 && (obj->field_130 & 0x8000) != 0) {
            obj->field_159 = one;
            obj->field_07 = 2;
            obj->field_157 = 0;
            func_80141f28(obj, 1);
            if (obj->field_0b != 0) {
                obj->field_4c = 0x36000;
            } else {
                obj->field_4c = -0x36000;
            }
            obj->field_50 = 0x40000;
            obj->field_58 = -0x6000;
            func_80130ec0(obj);
            obj->field_29a = one;
            obj->field_278 = one;
            func_801307e0(obj, 0x40);
            return;
        }
    }
    obj->field_159 = one;
    func_80130dc0(obj);
}
