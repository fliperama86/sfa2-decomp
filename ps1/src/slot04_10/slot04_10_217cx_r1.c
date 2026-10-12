/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b217c_slot04_10(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 2;
    obj->active_flag_3 = 1;
    obj->active_flag_4 = 1;
    obj->direction_index = 4;
    obj->other_lock = 0x11;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->facing_side = obj->saved_facing;
    ref_other.p = obj->other;
    ref_other.p->field_6b = 0xd;
    func_801307e0(obj, 0x20);
}
