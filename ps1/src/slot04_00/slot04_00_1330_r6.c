/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);

extern s32 data_801bfe40_slot04_00[];

void func_801b1998_slot04_00(Object *o) {
    u8 t = 1;
    s32 v;

    o->field_17b = t;
    o->field_07++;
    func_80141f28(o, 6);
    func_80138ae8(&game_state, o);
    o->field_14 = 0;
    o->field_50 = 0x40000;
    o->field_45 = t;
    if (o->field_49 == 0) {
        o->field_58 = -0x6000;
    } else {
        o->field_58 = 0xfffe8000;
    }
    v = data_801bfe40_slot04_00[o->field_12a >> 1];
    if (o->field_0b == 0) {
        v = -v;
    }
    o->field_4c = v;
    func_801307e0(o, 0x1e);
}

void func_801b1a54_slot04_00(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int a = 0x20;

    *(s32 *)&o->field_10 = *(s32 *)&o->field_10 + o->field_4c;
    *(s32 *)&o->field_14 = *(s32 *)&o->field_14 - o->field_50;
    o->field_50 = o->field_50 + o->field_58;
    if (o->field_50 >= 0) {
        func_80130efc(o);
    } else {
        o->field_07++;
        func_80120554(o, o->side, 0x320);
        obj->field_1a4 = o->field_12a >> 1;
        if (o->field_49 != 0) {
            a = 0x42;
        }
        func_801307e0(o, (o->field_12a >> 1) + a);
    }
}
