/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8003f9d4_slot28[];
extern void (*data_8003f9e0_slot28[])(Object *);

void func_8001f094_slot28(Object *obj) {
    data_8003f9d4_slot28[obj->field_03](obj);
}

void func_8001f0d4_slot28(Object *obj) {
    data_8003f9e0_slot28[obj->field_05](obj);
    obj->field_01 = 1;
}
