/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2994_slot04_07(Object *obj);

void func_801b248c_slot04_07(Object *obj) {
    int t;

    t = (s16)obj->field_70;
    if (t <= obj->pos_y) {
        obj->field_07 = 5;
        obj->pos_y = t;
        obj->field_14 = 0;
        obj->field_45 = 0;
        func_801209c4(obj);
        func_801307e0(obj, 0x39);
    } else {
        if (*(u8 *)&obj->field_3a != 0) {
            func_80130efc(obj);
            return;
        }
        func_801b2994_slot04_07(obj);
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
        obj->field_50 = obj->field_50 + obj->field_58;
        func_80130efc(obj);
    }
}
