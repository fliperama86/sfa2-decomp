/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3b4c_slot04_01(Object *obj);
extern ObjectFn data_801bf128_slot04_01[];

int func_801b388c_slot04_01(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
    return obj->field_50 < 0;
}

void func_801b38c4_slot04_01(Object *obj) {
    data_801bf128_slot04_01[obj->field_07](obj);
    func_801b3b4c_slot04_01(obj);
}
