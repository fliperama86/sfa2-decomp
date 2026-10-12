/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0e20_slot04_00(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->active_flag_3 = 1;
    obj->field_15a = 7;
    obj->field_6b = 0;
    obj->facing_side = obj->saved_facing;
    obj->other->field_6b = 0x16;
    obj->other_lock = 0x1a;
    func_801307e0(obj, 0x1a);
}
