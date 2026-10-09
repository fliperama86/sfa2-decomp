/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_801b1138_slot04_01(Object *obj);

void func_801b44f4_slot04_01(Object *obj) {
    int a = 0xc;
    int one;

    if (obj->field_211 & 1) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 3;
        obj->field_159 = 1;
        func_801b1138_slot04_01(obj);
        return;
    }
    obj->field_07 = 3;
    one = 1;
    obj->field_159 = one;
    if (obj->field_218 != 0 && obj->pos_y - obj->field_70 < -0x2f
        && (func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) & 0xff)) {
        obj->field_05 = 2;
        obj->field_04 = one;
        obj->field_06 = 0;
        obj->field_07 = 0;
        obj->field_128 = 4;
        return;
    }
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_48 != 0) {
        a = 0x12;
    }
    if (obj->field_129 != 0) {
        a += 3;
    }
    func_801307e0(obj, (s16)((obj->field_12a >> 1) + a));
}
