/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b4be8_slot04_04(Object *obj);

void func_801b47f4_slot04_04(Object *obj) {
    obj->field_157 = 0;
    if (func_801b4be8_slot04_04(obj) < 0 && obj->field_70 <= obj->pos_y) {
        obj->pos_y = obj->field_70;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_0b = obj->field_0b ^ 1;
        func_801209c4(obj);
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
