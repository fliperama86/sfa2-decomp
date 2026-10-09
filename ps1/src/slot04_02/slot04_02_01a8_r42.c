/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5290_slot04_02(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_0b = obj->field_0b ^ 1;
        obj->pos_y = obj->pos_y + 0x27;
        if (obj->field_0b != 0) {
            obj->pos_x = obj->pos_x - 0x11;
        } else {
            obj->pos_x = obj->pos_x + 0x11;
        }
    }
}

void func_801b531c_slot04_02(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_50 < 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_50 = 0;
        obj->field_54 = 0;
        obj->field_4c = 0;
        obj->field_58 = -0x9000;
        func_801307e0(obj, 0x58);
    } else {
        func_80130efc(obj);
    }
}
