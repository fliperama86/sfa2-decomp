/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void func_800205b4_slot28(Object *obj);
void apply_velocity_and_accel_slot28(Object *o);

/* The call of advance_sequence_checked passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_800205b4_slot28(Object *obj) {
    apply_velocity_and_accel_slot28(obj);
    if (obj->pos_y >= 0xd0) {
        obj->state_0++;
    }
    ((void (*)(void))advance_sequence_checked)();
}
