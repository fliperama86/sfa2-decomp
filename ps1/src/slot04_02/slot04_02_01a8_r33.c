/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c64b8_slot04_02[];

void func_801b452c_slot04_02(Object *obj) {
    int k;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 1;
        obj->field_07++;
        k = obj->field_12a << 1;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_4c = data_801c64b8_slot04_02[(u8)k];
        obj->field_54 = data_801c64b8_slot04_02[(u8)(k | 1)];
        obj->field_50 = data_801c64b8_slot04_02[(u8)(k + 2)];
        obj->field_58 = data_801c64b8_slot04_02[(u8)(k + 3)];
    }
}
