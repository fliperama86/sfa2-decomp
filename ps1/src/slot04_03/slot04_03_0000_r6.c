/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b079c_slot04_03(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    if (obj->field_70 > obj->pos_y) {
        func_80130efc(obj);
    } else {
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        func_80130efc(obj);
        func_80130ec0(obj);
        func_801307e0(obj, 0x1e);
    }
}

void func_801b084c_slot04_03(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->other->field_249 = 8;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
