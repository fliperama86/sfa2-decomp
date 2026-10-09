/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b19c4_slot04_05(Object *obj) {
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b1be0_slot04_05(Object *obj);

void func_801b1a60_slot04_05(Object *obj) {
    func_801b1be0_slot04_05(obj);
    if (obj->field_70 >= obj->pos_y) {
        if (*(u8 *)&obj->field_3a != 3) {
            func_80130efc(obj);
        }
    } else {
        obj->field_07++;
        func_801209c4(obj);
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        func_80130efc(obj);
    }
}
