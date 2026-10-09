/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4608_slot04_02(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        obj->pos_y = obj->pos_y - 0x28;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_46 = (u8)obj->field_46 | 0xa00;
    }
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 += obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 -= obj->field_4c;
    }
    obj->field_4c += obj->field_54;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
}
