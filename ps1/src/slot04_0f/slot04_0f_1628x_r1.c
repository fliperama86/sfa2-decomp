/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80141b28(Object *object);

int func_801b1628_slot04_0f(Object *o) {
    if (!func_80141b28(o)) return 0;
    o->state_0 = 1;
    o->state_1 = 0;
    o->state_3 = 0;
    o->field_157 = 0;
    o->field_6b = 0;
    o->state_2 = 7;
    o->active_flag_3 = 1;
    o->facing_side = o->saved_facing;
    o->field_15a = 9;
    o->other_lock = 0x1d;
    o->other->field_6b = 0x19;
    func_801307e0(o, 0x48);
    return 1;
}
