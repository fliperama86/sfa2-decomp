/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bef6c_slot04_01[];

void func_801b11cc_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int h;
    int a = 0x1d;

    o->field_17b = 1;
    o->field_07++;
    func_801204f4(o, obj->field_a6, 0xb);
    func_80141f28(o, 9);
    func_80138ae8(&game_state, o);
    o->field_54 = -0x8000;
    o->field_58 = -0x6000;
    h = o->field_12a >> 1;
    o->field_4c = data_801bef6c_slot04_01[h * 2];
    o->field_50 = data_801bef6c_slot04_01[h * 2 + 1];
    if (o->field_49 != 0) {
        a = 0x42;
    }
    func_801307e0(o, h + a);
}

void func_801b1298_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_3a != 0) {
        func_80130efc(o);
    } else {
        o->field_45 = 1;
        o->field_07++;
    }
}
