/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b359c_slot04_02(Object *obj);
void func_801b33fc_slot04_02(Object *obj);

void func_801b32a8_slot04_02(Object *obj) {
    int n;

    obj->field_17b = 1;
    obj->field_07 = 1;
    obj->field_29c = 2;
    obj->field_252 = 0xff;
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    obj->field_54 = -0x3000;
    if (obj->field_129 != 0) {
        obj->field_4c = 0x80000;
    } else {
        obj->field_4c = 0xb8000;
    }
    if (obj->field_49 != 0) {
        n = 0x64;
        obj->field_252 = 0;
    } else {
        n = 0x2d;
    }
    func_801307e0(obj, n);
    if (obj->field_7e == 0) {
        func_80157380(obj);
        func_801b359c_slot04_02(obj);
    }
}

void func_801b3368_slot04_02(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_07 = obj->field_07 + 1;
        func_801204f4(obj, obj->side, 0xc);
        func_801b33fc_slot04_02(obj);
    } else {
        obj->pos_x = obj->pos_x + (s8)*(u8 *)&obj->field_3a;
        func_80130efc(obj);
    }
    if (obj->field_7e == 0) {
        func_801b359c_slot04_02(obj);
    }
}
