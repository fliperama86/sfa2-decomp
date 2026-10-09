/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013f8c4(Object *obj, int a, int b);
void func_801b6cb0_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b6630_slot04_02(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if (func_8013f8c4(obj, -0x14, 0x14) & 0xff) {
            func_801b6cb0_slot04_02(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->field_12a == 2 && obj->field_219 != 0) {
            obj->field_159 = 1;
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
            func_801307e0(obj, 0x40);
            return;
        }
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}
