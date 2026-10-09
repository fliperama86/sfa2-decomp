/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b25a8_slot04_08(Object *object);

void func_801b249c_slot04_08(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 + obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_70 < obj->pos_y) {
        obj->pos_y = obj->field_70;
        obj->field_45 = 0;
        func_801209c4(obj);
        if ((u8)func_8013f8c4(obj, -0x20, 0x30) != 0) {
            obj->field_07++;
            obj->field_46 = 0;
            func_80120554(obj, obj->side ^ 1, 0x31a);
            func_80145f98(obj);
            func_801204f4(obj, obj->side, 0x12);
            func_801b25a8_slot04_08(obj);
        } else {
            obj->field_07 = 9;
            func_801307e0(obj, 0x33);
        }
    } else {
        func_80130efc(obj);
    }
}
