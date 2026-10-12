/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800422e4_slot28[];
extern void (*data_80042308_slot28[])(Object *);

void func_8002036c_slot28(Object *obj) {
    data_800422e4_slot28[obj->table_index](obj);
}

void func_800203ac_slot28(Object *obj) {
    data_80042308_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
    obj->field_01 = 1;
}
