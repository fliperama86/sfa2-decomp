/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b3a40_slot04_04(Object *obj);
void func_801b0904_slot04_04(Object *obj);
void func_801b09ac_slot04_04(Object *obj);

void func_801b0790_slot04_04(Object *obj) {
    obj->field_157 = 0;
    if (func_801b3a40_slot04_04(obj) < 0 && obj->pos_y >= obj->field_70) {
        obj->pos_y = obj->field_70;
        obj->field_14 = 0;
        obj->field_45 = 0;
        if (obj->field_7e == 0) {
            obj->field_0b = obj->field_0b ^ 1;
        }
        func_801209c4(obj);
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b0820_slot04_04(Object *obj) {
    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80130504(obj);
    if (obj->field_129 != 0) {
        func_801b0904_slot04_04(obj);
    } else if (obj->field_12a != 0 && (obj->field_130 & 0xe000) != 0 && obj->pos_y < obj->field_70 - 0x30 && (u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        func_801b09ac_slot04_04(obj);
    }
}
