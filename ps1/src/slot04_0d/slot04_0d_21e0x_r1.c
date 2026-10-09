/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b21e0_slot04_0d(Object *obj) {
    s32 a;

    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 + obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_70 <= obj->pos_y) {
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->field_07++;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x34);
    } else {
        if (obj->field_0b != 0) {
            a = obj->field_4c;
        } else {
            a = -obj->field_4c;
        }
        *(s32 *)&obj->field_10 = a + *(s32 *)&obj->field_10;
        obj->field_4c = obj->field_4c + obj->field_54;
        func_80130efc(obj);
    }
}
