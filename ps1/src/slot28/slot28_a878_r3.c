/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80037448_slot28[];
extern void (*data_8003746c_slot28[])(Object *);

void func_8001abbc_slot28(Object *obj) {
    data_80037448_slot28[obj->field_03](obj);
}

void func_8001abfc_slot28(Object *obj) {
    data_8003746c_slot28[obj->field_05](obj);
    obj->field_01 = 1;
}
