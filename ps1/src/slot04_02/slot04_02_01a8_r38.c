/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4d28_slot04_02(Object *obj);

void func_801b4c34_slot04_02(Object *obj) {
    obj->field_07 = 9;
    obj->field_159 = 1;
    func_801307e0(obj, (obj->field_12a >> 1) + 0x52);
}

void func_801b4c6c_slot04_02(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_17b = 0;
        obj->field_38 = 1;
        func_801307e0(obj, 0x50);
    } else {
        if ((u8)obj->field_3a == 0) {
            func_80130efc(obj);
        }
        func_801b4d28_slot04_02(obj);
    }
}
