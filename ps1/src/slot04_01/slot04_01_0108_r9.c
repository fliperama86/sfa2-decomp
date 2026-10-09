/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3b4c_slot04_01(Object *obj);
extern ObjectFn data_801bef84_slot04_01[];

void func_801b1540_slot04_01(Object *obj) {
    if (obj->field_49 != 0 && obj->field_50 < 0) {
        obj->field_58 = 0xffff0000;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
}

void func_801b158c_slot04_01(Object *obj) {
    data_801bef84_slot04_01[obj->field_07](obj);
    func_801b3b4c_slot04_01(obj);
}
