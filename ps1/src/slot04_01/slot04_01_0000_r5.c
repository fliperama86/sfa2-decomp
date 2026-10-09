/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130504(Object *object);
void func_80130dc0(Object *object);
void func_80142a14(Object *object);
int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);

void func_801b0690_slot04_01(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_07 == 0) {
        obj->field_159 = 1;
        obj->field_07 = obj->field_07 + 1;
        func_80130dc0(obj);
    } else {
        func_80142a14(obj);
    }
}

void func_801b06dc_slot04_01(Object *obj) {
    int t = 0xc;
    int one = 1;

    obj->field_07 = 3;
    obj->field_159 = one;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_48 != 0) {
        t = 0x12;
    }
    if (obj->field_129 != 0) {
        t += 3;
    } else if (obj->field_12a != 0) {
        if (obj->pos_y - obj->field_70 < -0x2f && (obj->field_130 & 0xe000) != 0) {
            if (func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) & 0xff) {
                obj->field_05 = 2;
                obj->field_04 = one;
                obj->field_06 = 0;
                obj->field_07 = 0;
                obj->field_128 = 4;
                return;
            }
        }
    }
    func_801307e0(obj, (s16)((obj->field_12a >> 1) + t));
}
