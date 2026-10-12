/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1058_slot04_0b(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->field_15a = 7;
    obj->active_flag_3 = 1;
    obj->active_flag_4 = 1;
    obj->direction_index = 4;
    obj->other_lock = 0x17;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->facing_side = obj->saved_facing;
    obj->other->field_6b = 0x13;
    func_80146998(obj);
    func_801307e0(obj, 0x2b);
}
