/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1378_slot04_04(Object *obj);

int func_801b1328_slot04_04(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b1378_slot04_04(obj);
    }
    return r;
}

void func_801b1378_slot04_04(Object *obj) {
    ref_other.p = obj->other;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 9;
    obj->active_flag_3 = 1;
    obj->field_157 = 0;
    obj->direction_index = 4;
    obj->field_6b = 0;
    obj->facing_side = obj->saved_facing;
    ref_other.p->field_6b = 0x14;
    ref_other.p->other_lock = 0x18;
    func_80146998(obj);
    func_801307e0(obj, 0x1e);
}
