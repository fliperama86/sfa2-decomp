/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The form of func_801b1548_slot04_07 of another character file, with four
   other constants and three of its statements in another order. */
void func_801b1008_slot04_01(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 8;
    obj->facing_side = obj->saved_facing;
    obj->field_157 = 0;
    obj->active_flag_3 = 1;
    obj->field_6b = 0;
    obj->other->field_6b = 0x19;
    obj->other_lock = 0x1d;
    func_801307e0(obj, 0x41);
}
