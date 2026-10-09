/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b16a0_slot04_00(Object *obj);
void func_801b1554_slot04_00(Object *obj);

void func_801b1438_slot04_00(Object *obj) {
    func_801b16a0_slot04_00(obj);
    if (obj->pos_y >= obj->field_70) {
        func_801b1554_slot04_00(obj);
    } else {
        if (obj->field_0b == 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        }
        obj->field_4c = obj->field_4c + obj->field_54;
        if (obj->field_4c < 0) {
            obj->field_07++;
        }
        func_80130efc(obj);
    }
}

void func_801b14ec_slot04_00(Object *obj) {
    func_801b16a0_slot04_00(obj);
    if (obj->pos_y >= obj->field_70) {
        func_801b1554_slot04_00(obj);
    } else if (*(u8 *)&obj->field_3a == 0) {
        func_80130efc(obj);
    }
}

void func_801b1554_slot04_00(Object *obj) {
    obj->field_07++;
    obj->field_45 = 0;
    obj->field_159 = 0;
    obj->field_17b = 0;
    obj->pos_y = (u16)obj->field_70;
    func_801209c4(obj);
    func_80130efc(obj);
}
