/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HalfPair data_800517fc_slot28[];

void func_800282c0_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    obj->field_01 = 1;
    obj->field_50 = 0x10000;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->field_04 = obj->field_04 + 1;
    obj->pos_x = data_800517fc_slot28[obj->field_03].field_00;
    obj->pos_y = data_800517fc_slot28[obj->field_03].field_02;
    func_80130768(obj, 0, (SequenceStep **)obj->box_tables);
}

void func_80028348_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    int sum = obj->field_14 + o->field_50;

    obj->field_14 = sum;
    if (sum > 0xffffff) {
        obj->field_14 = 0xff800000;
    }
    func_80131094(o);
}
