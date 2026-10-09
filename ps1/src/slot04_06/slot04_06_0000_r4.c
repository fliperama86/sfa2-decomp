/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130dc0(Object *object);
u8 func_8013f8c4(Object *object, int a, int b);

void func_801b0510_slot04_06(Object *obj) {
    int one;

    obj->field_07 = obj->field_07 + 1;
    if ((obj->field_130 & 0xa000) != 0 && func_8013f8c4(obj, -0x18, 0x14) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        one = 1;
        obj->field_159 = one;
        func_80130dc0(obj);
        obj->field_278 = one;
    }
}

void func_801b0598_slot04_06(Object *obj) {
    int d = -0x10;

    if ((u8)obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_0b != 0) {
            d = 0x10;
        }
        obj->pos_x = d + obj->pos_x;
    }
    func_80130efc(obj);
}
