/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80048d3c_slot28[];

void func_80022f28_slot28(Object *obj) {
    data_80048d3c_slot28[obj->state_1](obj);
    obj->field_01 = 1;
    advance_sequence_checked(obj);
}
