/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c260c_slot04_0b[];

void func_801b28c0_slot04_0b(Object *obj) {
    int k;

    if ((u8)obj->field_3a == 6) {
        obj->field_07++;
        k = (obj->field_12a >> 1) * 3 + obj->field_a0;
        obj->field_4c = data_801c260c_slot04_0b[k];
        obj->field_50 = data_801c260c_slot04_0b[k + 1];
        obj->field_54 = -0x8000;
        obj->field_58 = data_801c260c_slot04_0b[k + 2];
        obj->field_45 = 1;
    }
    func_80130efc(obj);
}

void func_801b296c_slot04_0b(Object *obj) {
    if (obj->field_4c >= 0) {
        *(s32 *)&obj->field_14 -= obj->field_50;
        obj->field_50 += obj->field_58;
        if (obj->field_0b == 0) {
            *(s32 *)&obj->field_10 -= obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 += obj->field_4c;
        }
        obj->field_4c += obj->field_54;
    } else {
        obj->field_4c = 0;
        obj->field_54 = 0;
        obj->field_07++;
    }
    func_80130efc(obj);
}
