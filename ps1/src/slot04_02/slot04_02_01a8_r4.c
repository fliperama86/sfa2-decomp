/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b06e4_slot04_02(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b0714_slot04_02(Object *obj) {
    u16 x;

    obj->field_128 = 4;
    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    x = 0xc;
    if (obj->field_48 != 0) {
        x = 0x12;
    }
    if (obj->field_129 != 0) {
        x += 3;
    }
    x += obj->field_12a >> 1;
    func_801307e0(obj, x);
    if ((u16)obj->pos_y < (u16)(obj->field_70 - 0x50) && obj->field_48 != 0 && (obj->field_48 & 0x80) == 0 && obj->field_129 != 0 && obj->field_12a == 2 && (obj->field_130 & 0x4000) != 0) {
        func_801b631c_slot04_02(obj, 1, 0, 5, 0);
        func_801307e0(obj, 0x29);
    }
}
