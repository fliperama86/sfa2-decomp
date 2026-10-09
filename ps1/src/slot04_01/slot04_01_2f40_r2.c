/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3098_slot04_01(Object *obj) {
    s32 t = obj->field_50 + obj->field_58;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - t;
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    obj->field_50 = t;
    if (obj->pos_y < obj->field_70) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04aObj *)obj)->field_70;
        func_801209c4(obj);
        func_80130678(obj, 0x11);
    }
}
