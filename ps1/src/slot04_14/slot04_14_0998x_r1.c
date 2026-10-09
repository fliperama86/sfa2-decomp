/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0998_slot04_14(Object *obj);

void func_801b0998_slot04_14(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    if (obj->field_70 <= (u16)obj->pos_y) {
        func_801209c4(obj);
        obj->pos_y = (u16)obj->field_70;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
