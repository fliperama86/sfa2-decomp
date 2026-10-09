/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b22a8_slot04_01(Object *obj) {
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y < obj->field_70) {
        if ((u8)obj->field_3a != 0) {
            return;
        }
    } else {
        obj->field_45 = 0;
        obj->field_159 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
    }
    func_80130efc(obj);
}

void func_801b2340_slot04_01(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
