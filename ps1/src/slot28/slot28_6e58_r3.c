/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800306c4_slot28[])(Object *);

void func_80017018_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
    if (obj->field_47 != 0) {
        if (obj->field_50 >= 0) {
            obj->field_50 = 0x4000;
            obj->field_47 = 0;
            obj->field_58 = -0x200;
        }
    } else if (obj->field_50 < 0) {
        obj->field_50 = -0x4000;
        obj->field_58 = 0x200;
        obj->field_47 = 0xff;
    }
    obj->field_01 = 1;
}

void func_80017090_slot28(Object *obj) {
    func_80131094(obj);
    obj->field_01 = 1;
}

void func_800170c0_slot28(Object *obj) {
    data_800306c4_slot28[obj->field_05](obj);
    func_80131094(obj);
    obj->field_01 = 1;
}

void func_8001711c_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_05 += 1;
    }
}
