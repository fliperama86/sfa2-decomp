/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);

void func_801b1068_slot04_07(Object *obj) {
    int a;

    obj->field_07 = 3;
    obj->field_159 = 1;
    obj->field_128 = 4;
    func_80130504(obj);
    a = 0xc;
    if (obj->field_129 == 0) {
        if (obj->field_12a == 2 && (obj->field_130 & 0x4000)) {
            obj->field_4c >>= 1;
            func_801204f4(obj, obj->side, 7);
            func_801307e0(obj, 0x1f);
            return;
        }
        if (obj->field_12a != 0 && (obj->field_130 & 0xe000) && obj->pos_y < obj->field_70 - 0x30) {
            if ((u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x20)) {
                obj->field_04 = 1;
                obj->field_05 = 2;
                obj->field_06 = 0;
                obj->field_07 = 0;
                return;
            }
        }
    }
    if (obj->field_48 != 0) {
        a = 0x12;
    }
    if (obj->field_129 != 0) {
        a += 3;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, (s16)a);
    func_80141f28(obj, obj->field_12a >> 1);
}
