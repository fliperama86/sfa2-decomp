/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801befa4_slot04_01[];

void func_801b1870_slot04_01(Object *obj) {
    s32 v;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    func_801204f4(obj, obj->side, 0xc);
    obj->field_50 = 0x40000;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 & 0xffff0000;
    obj->field_45 = 1;
    if (obj->field_49 == 0) {
        obj->field_58 = -0x6000;
    } else {
        obj->field_58 = -0x18000;
    }
    v = data_801befa4_slot04_01[obj->field_12a >> 1];
    if (obj->field_0b == 0) {
        obj->field_4c = -v;
    } else {
        obj->field_4c = v;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + 0x32);
}

void func_801b1954_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int a = 0x22;

    *(s32 *)&o->field_10 = *(s32 *)&o->field_10 + o->field_4c;
    *(s32 *)&o->field_14 = *(s32 *)&o->field_14 - o->field_50;
    o->field_50 = o->field_50 + o->field_58;
    if (o->field_50 < 0) {
        o->field_07++;
        func_80120554(o, o->side, 0x320);
        obj->field_1a4 = (o->field_12a >> 1) + 1;
        if (o->field_49 != 0) {
            a = 0x4a;
        }
        func_801307e0(o, obj->field_1a4 + a - 1);
    } else {
        func_80130efc(o);
    }
}
