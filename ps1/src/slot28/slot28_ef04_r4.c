/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003f9f0_slot28[])(Object *);

void func_8001f1fc_slot28(Object *obj) {
}

void func_8001f204_slot28(Object *obj) {
    data_8003f9f0_slot28[obj->field_05](obj);
    obj->field_01 = 1;
}
