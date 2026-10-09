/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c650c_slot04_02[];

void func_801b5090_slot04_02(Object *obj) {
    func_80130efc(obj);
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y >= obj->field_70) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_49 != 0) {
            obj->field_17b = 0;
            obj->field_45 = 0;
        }
        obj->field_4c = 0x20000;
        obj->pos_y = obj->field_70;
        obj->field_54 = data_801c650c_slot04_02[obj->field_12a >> 1];
        func_801307e0(obj, 0x55);
        func_80120554(obj, obj->side, 0x324);
    }
}
