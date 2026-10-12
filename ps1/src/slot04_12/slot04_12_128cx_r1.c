/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80146998(Object *object);
/* functions of other units of this module */
void func_801b128c_slot04_12(Object *obj);

void func_801b128c_slot04_12(Object *obj) {
    ref_other.p = obj->other;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 8;
    obj->active_flag_3 = 1;
    obj->field_157 = 0;
    obj->direction_index = 2;
    obj->field_6b = 0;
    obj->facing_side = obj->saved_facing;
    ref_other.p->field_6b = 0x14;
    ref_other.p->other_lock = 0x18;
    func_80146998(obj);
    func_801307e0(obj, 0x1f);
}
