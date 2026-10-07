/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);

void func_801b0b74_slot04_0a(Object *obj) {
    int a;

    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    a = 0x15;
    if (obj->field_48 == 0) {
        a = 0xf;
    }
    if (obj->field_129 == 0) {
        a -= 3;
        if (obj->field_12a != 0 && obj->pos_y < obj->field_70 - 0x30 && (obj->field_130 & 0xe000) != 0 && (u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
            obj->field_128 = 4;
            return;
        }
    }
    func_801307e0(obj, (s16)((obj->field_12a >> 1) + a));
}
