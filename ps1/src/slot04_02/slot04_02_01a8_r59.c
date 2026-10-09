/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6a84_slot04_02(Object *obj);
void func_801b68b4_slot04_02(Object *obj);
void func_801b6cb0_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b6a40_slot04_02(Object *obj) {
    if (obj->field_211 & 1) {
        func_801b68b4_slot04_02(obj);
    } else {
        func_801b6a84_slot04_02(obj);
    }
}

void func_801b6a84_slot04_02(Object *obj) {
    int t;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80120af8(obj);
    t = obj->field_12a >> 1;
    func_80141f28(obj, t);
    if (obj->field_48 != 0) {
        t += 0x12;
    } else {
        t += 0xc;
    }
    if (obj->field_129 != 0) {
        t += 3;
    }
    func_801307e0(obj, (s16)t);
    func_80120af8(obj);
    if (obj->pos_y < (u16)(((Slot04bObj *)obj)->field_70 - 0x50) && obj->field_48 != 0 && (obj->field_48 & 0x80) == 0 && obj->field_129 != 0 && obj->field_12a == 2 && obj->field_219 != 0) {
        func_801b6cb0_slot04_02(obj, 1, 0, 5, 0);
        func_801307e0(obj, 0x29);
    }
}
