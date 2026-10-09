/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4bc4_slot04_04(Object *obj);

void func_801b46b8_slot04_04(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a == 4 && obj->field_219 != 0) {
        obj->field_159 = 1;
        obj->field_07++;
        func_80141f28(obj, 2);
        obj->field_50 = 0x78000;
        obj->field_54 = 0;
        obj->field_58 = -0x5000;
        if (obj->field_0b != 0) {
            obj->field_4c = 0x18000;
        } else {
            obj->field_4c = 0xfffe8000;
        }
        obj->field_45 = 1;
        obj->field_157 = 0;
        obj->pos_y -= 0x10;
        func_801307e0(obj, 0x1b);
    } else {
        func_801b4bc4_slot04_04(obj);
    }
}
