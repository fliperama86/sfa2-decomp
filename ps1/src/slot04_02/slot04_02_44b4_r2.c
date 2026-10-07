/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c64e8_slot04_02[];

void func_801b482c_slot04_02(Object *obj);
void func_801b494c_slot04_02(Object *obj);

void func_801b46d4_slot04_02(Object *obj) {
    u16 a;

    func_80130efc(obj);
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y >= obj->field_70) {
        obj->field_157 = 1;
        obj->field_159 = 1;
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        a = obj->field_12a >> 1;
        obj->field_54 = data_801c64e8_slot04_02[a];
        if (obj->field_49 != 0 && obj->field_cd == 0) {
            a += 0x6d;
        } else {
            a += 0x4d;
        }
        func_801307e0(obj, a);
        func_80120554(obj, obj->side, 0x324);
    } else if (obj->field_cd != 0) {
        func_801b494c_slot04_02(obj);
    } else {
        func_801b482c_slot04_02(obj);
    }
}
