/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1f10_slot04_07[];

void func_801b1a50_slot04_07(Object *obj) {
    u8 one = 1;
    s32 *p;
    s32 a;
    s32 b;

    obj->field_17b = one;
    obj->field_07++;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    if (obj->field_49 != 0) {
        obj->field_225 = one;
    }
    p = data_801c1f10_slot04_07 + (obj->field_12a >> 1) * 4;
    a = *p++;
    b = *p++;
    obj->field_50 = *p;
    obj->field_58 = p[1];
    if (obj->field_0b == 0) {
        obj->field_4c = -a;
        obj->field_54 = -b;
    } else {
        obj->field_4c = a;
        obj->field_54 = b;
    }
    func_801307e0(obj, 0x20);
}

void func_801b1b2c_slot04_07(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_45 = 1;
        obj->field_07++;
        obj->field_46 = 0x1000;
    }
    func_80130efc(obj);
}
