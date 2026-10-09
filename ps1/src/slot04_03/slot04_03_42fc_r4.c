/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4788_slot04_03(Object *obj) {
    int k = 0xc;

    obj->field_07 = 3;
    obj->field_159 = 1;
    if (obj->field_218 != 0 && obj->field_70 - obj->pos_y >= 0x30 && func_8013ffe4(obj, -0x30, 0x10, 0x20, 0x10) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
        obj->field_128 = 1;
    } else {
        func_80141f28(obj, obj->field_12a >> 1);
        if (obj->field_48 != 0) {
            k = 0x12;
        }
        if (obj->field_129 != 0) {
            k += 3;
        }
        func_801307e0(obj, (s16)((obj->field_12a >> 1) + k));
    }
}
