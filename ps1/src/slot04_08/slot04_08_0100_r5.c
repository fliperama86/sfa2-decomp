/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);

void func_801b2958_slot04_08(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 + obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_70 < obj->pos_y) {
        obj->pos_y = obj->field_70;
        obj->field_45 = 0;
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 & 0xffff0000;
        func_801209c4(obj);
        obj->field_07 = obj->field_07 + 1;
        func_80130678(obj, 0x11);
    } else {
        func_80130efc(obj);
    }
}
