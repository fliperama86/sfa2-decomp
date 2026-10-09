/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HalfPair data_80051820_slot28[];

void func_800283f4_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    obj->field_01 = 1;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->field_04 = obj->field_04 + 1;
    obj->pos_x = data_80051820_slot28[obj->field_03].field_00;
    obj->pos_y = data_80051820_slot28[obj->field_03].field_02;
    if (obj->field_03 == 0) {
        func_80130768(obj, 1, (SequenceStep **)obj->box_tables);
    } else {
        func_80130768(obj, 2, (SequenceStep **)obj->box_tables);
    }
}

void func_80028490_slot28(Object *obj) {
    obj->field_01 = 1;
    func_80131094(obj);
}
