/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800353cc_slot28[])(Object *);

void func_80019444_slot28(Object *obj) {
}

void func_8001944c_slot28(Object *object) {
    data_800353cc_slot28[object->field_05](object);
}
