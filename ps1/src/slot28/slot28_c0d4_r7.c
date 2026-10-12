/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001ca7c_slot28(Object *o) {
    o->field_01 = 0;
    if (((Slot28Obj *)o)->field_3a < 0) {
        o->state_0++;
    }
    advance_sequence_checked(o);
    advance_sequence_checked(o);
    o->field_48 = o->field_48 ^ 1;
    if (o->field_48 != 0) {
        o->field_01 = 1;
    }
}
