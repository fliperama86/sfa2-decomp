/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c0424_slot04_0a[][21];

void func_801b1964_slot04_0a(Object *obj) {
    Object *o;
    int t;

    obj->field_17b = 1;
    obj->field_67 = 0;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    o = obj->other;
    t = *(s32 *)&o->field_10 - *(s32 *)&obj->field_10;
    if (t < 0) {
        t += -0xc0000;
    } else {
        t += 0xc0000;
    }
    obj->field_4c = t >> 5;
    obj->field_54 = 0;
    obj->field_50 = 0x80000;
    obj->field_58 = 0xffff7000;
    if (o->field_45 != 0 || o->field_157 != 0) {
        t = 0;
    } else {
        t = 1;
    }
    t = data_801c0424_slot04_0a[t][o->kind];
    t <<= 16;
    t += *(s32 *)&obj->field_14 - *(s32 *)&o->field_14;
    if (t > 0x900000) {
        t = 0x900000;
    }
    t >>= 5;
    obj->field_50 = obj->field_50 + t;
    if (obj->field_49 != 0) {
        func_801307e0(obj, 0x55);
    } else {
        func_801307e0(obj, 0x54);
    }
}
