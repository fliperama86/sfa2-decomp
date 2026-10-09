/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c4180_slot04_04[];
int func_801b3a40_slot04_04(Object *obj);
void func_801b0adc_slot04_04(Object *obj);

void func_801b09ac_slot04_04(Object *obj) {
    s16 a = 0xc;

    func_80141f28(obj, *(s16 *)((u8 *)data_801c4180_slot04_04 + (obj->field_12a & 0xfe)));
    if (obj->field_48 != 0) {
        a = 0x12;
    }
    if (obj->field_129 != 0) {
        a += 3;
    }
    func_801307e0(obj, (s16)(a + (obj->field_12a >> 1)));
}

void func_801b0a40_slot04_04(Object *obj) {
    if (obj->field_12f == 0) {
        if (obj->field_67 != 0) {
            func_801b0adc_slot04_04(obj);
        } else if (func_801b3a40_slot04_04(obj) < 0 && !(obj->pos_y < obj->field_70)) {
            func_801209c4(obj);
            func_80131638(obj);
        } else {
            func_80130efc(obj);
        }
    }
}
