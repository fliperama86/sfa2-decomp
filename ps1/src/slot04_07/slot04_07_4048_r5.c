/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013f8c4(Object *object, int a, int b);

void func_801b470c_slot04_07(Object *obj) {
    u8 one = 1;

    obj->field_159 = one;
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0) {
        if (obj->field_218 != 0 && (func_8013f8c4(obj, -0x12, 0x14) & 0xff) != 0) {
            obj->field_04 = one;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
        } else if (obj->field_12a == 4) {
            obj->field_07 = 2;
            if (obj->field_219 != 0) {
                if (obj->field_0b == 0) {
                    obj->pos_x = obj->pos_x - 0x20;
                } else {
                    obj->pos_x = obj->pos_x + 0x20;
                }
            }
            func_801307e0(obj, 0x2f);
        } else {
            func_80130dc0(obj);
        }
    } else {
        func_80130dc0(obj);
    }
}
