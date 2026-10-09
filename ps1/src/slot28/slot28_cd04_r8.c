/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003c2ec_slot28[])(Object *);

void func_8001d93c_slot28(Object *obj) {
}

void func_8001d944_slot28(Object *object) {
    data_8003c2ec_slot28[object->field_05](object);
}
