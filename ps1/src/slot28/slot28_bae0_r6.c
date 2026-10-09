/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8003a160_slot28[];
extern void (*data_8003a174_slot28[])(Object *);

void func_8001c68c_slot28(Object *obj) {
    data_8003a160_slot28[obj->field_03](obj);
}

void func_8001c6cc_slot28(Object *obj) {
    data_8003a174_slot28[obj->field_05](obj);
    obj->field_01 = 1;
}
