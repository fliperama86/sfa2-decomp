/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b223c_slot04_03(Object *obj);

void func_801b2148_slot04_03(Object *obj) {
    s16 y;

    if (*(u8 *)&obj->field_3a != 0) {
        func_80130efc(obj);
        return;
    }
    obj->field_45 = 1;
    func_801b223c_slot04_03(obj);
    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    y = obj->field_70;
    if (y < obj->pos_y) {
        obj->pos_y = y;
        obj->field_45 = 0;
        func_801209c4(obj);
        obj->field_07++;
        func_801307e0(obj, 0x28);
    } else {
        func_80130efc(obj);
    }
}
