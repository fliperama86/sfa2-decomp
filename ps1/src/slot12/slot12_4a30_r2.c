/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80014dd0_slot12(Object *obj, int a);
u8 func_80014b80_slot12(Object *obj);

void func_80014b20_slot12(Object *obj) {
    if (func_80014b80_slot12(obj) & 0xff) {
        obj->field_05++;
        obj->pos_y = obj->field_70;
        func_80014dd0_slot12(obj, 0);
    } else {
        func_80131094(obj);
    }
}

u8 func_80014b80_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    *(s32 *)&o->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    *(s32 *)&o->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    return o->pos_y >= o->field_70;
}

void func_80014bd4_slot12(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_46 = 10;
        obj->field_05++;
        func_80014dd0_slot12(obj, 0);
    } else {
        func_80131094(obj);
    }
}
